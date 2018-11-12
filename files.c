#include"files.h"
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>

// Writes the Current Buffer

void writebuffer(MYFILE *stream) {
	lseek(stream->fd, (stream->count - 1) * 512, SEEK_SET);
	write(stream->fd, stream->buffer, 512);
}

//Refills the current buffer with '\0's

void refillw0(MYFILE *stream) {
	int i = 0;
	while(i < 512) {
		stream->buffer[i] = '\0';
		i++;
	}
}

//Refills the Buffer in Chunks of 512 bytes depending upon current bufcount

int refill(MYFILE *stream) {
	if(myfeof(stream) == 1 && stream->mode != nappend) {
		stream->bytesread = 0;
		stream->count++;
		return 0;
	}
	lseek(stream->fd, (stream->count) * 512, SEEK_SET);
	stream->bytesread = read(stream->fd, stream->buffer, 512);
	lseek(stream->fd, -stream->bytesread, SEEK_CUR);
	stream->bufferptr = stream->buffer;
	stream->count++;
	if(stream->bytesread == 512)
		stream->countfull++;
	if(stream->bytesread != 512)
		stream->bytesread--;
	return 1;	
}

/* Mallocs memory for the FILE structure, initialises all its elements 
 * and returns a pointer to structure
 */ 

MYFILE *init(const char *pathname) {
	MYFILE *f;
	f = (MYFILE*)malloc(sizeof(MYFILE));
	stat(pathname, &(f->st));
	f->sizeoffile = f->st.st_size - 1;
	if(f->sizeoffile == -1)
		f->sizeoffile++;
	f->nameoffile = pathname;
 	f->buffer = (char *)malloc(512);
 	f->bufferptr = f->buffer;
 	f->bufferend = f->buffer + 512;
 	f->count = 0;
 	f->countfull = 0;
 	f->fileptrpos = 0;
 	return f;
}

/*Opens the file in the mode that is passed as argument by setting appropriate
 * flags to open function call and returns NULL if unable to open the file and appropriately
 * sets errno
 */
 
MYFILE *myfopen(const char *pathname, const char *mode) {
 	MYFILE *f;
 	if(strcmp(mode, "r") == 0) {
 		f = init(pathname);
 		f->fd = open(pathname, O_RDONLY, S_IRUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else {
			f->mode = nread;
			return f;
		}
 	}
 	else if(strcmp(mode, "w") == 0) {
 		f = init(pathname);
 		f->fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
		f->sizeoffile = 0;
		if(f->fd == -1) {
			return NULL;
		}
		else {
	 		f->mode = nwrite;
			return f;
		} 	
 	}
 	else if(strcmp(mode, "a") == 0) {
 		f = init(pathname);
 		f->fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, S_IWUSR);
 		f->fileptrpos = f->sizeoffile;
		if(f->fd == -1) {
			return NULL;
		}
		else {
	 		f->mode = nappend;	
			return f;
		} 	
 	}
 	else if(strcmp(mode, "r+") == 0) {
 		f = init(pathname);
 		f->fd = open(pathname, O_RDWR, S_IRUSR | S_IWUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else {
	 		f->mode = nreadplus;	
			return f;
 		}
 	}
 	else if(strcmp(mode, "w+") == 0) {
 		f = init(pathname);
 		f->fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
		f->sizeoffile = 0;
		if(f->fd == -1) {
			return NULL;
		}
		else {
 			f->mode = nwriteplus;
			return f;
 		}
 	}
 	else if(strcmp(mode, "a+") == 0) {
 		f = init(pathname);
 		f->fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else {
	 		f->mode = nappendplus;
			return f;
		} 	
 	}
	else
		return NULL;
		
}

/* Reads nmemb items of data, each size bytes long, in the buffer and writes in actual file only when buffer is full
 * Returns the number of items read*/
 
mysize_t myfread(void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream) {
	mysize_t toread, toreadcpy, copy;
	char *p = (char *)ptr;
	int checkeof, flag = 0;
	toread = size * nmemb;
	toreadcpy = toread;
	if(stream->mode == nwrite || stream->mode == nappend)
		return 0;
	if (myfeof(stream) || toread == 0 || p == NULL)
    		return 0;
	if(stream->count == 0) {
		refill(stream);
	}
	stream->lastfuncall = nfread;
	//Dealing with residue in buffer
	myfilelabel:;
	copy = 0;
	if(stream->bufferptr < stream->bufferend) {
		flag = 1;
		copy = stream->bufferend - stream->bufferptr;
		if(copy > toread) {
			copy = toread;
		}
		if(stream->bytesread >= copy) {
			memcpy(p, stream->bufferptr, copy);
			stream->bufferptr = stream->bufferptr + copy;
			stream->fileptrpos = stream->fileptrpos + copy;
			toread = toread - copy;
		}
		else {
			memcpy(p, stream->bufferptr, stream->bytesread);
			stream->bufferptr = stream->bufferptr + stream->bytesread;
			stream->fileptrpos = stream->fileptrpos + stream->bytesread;
			toread = toread - stream->bytesread;
			return (toreadcpy - toread) / size;
		}
	}
	if(toread == 0) {
		return nmemb;
	}
	if(stream->bufferptr != stream->bufferend) {
		p = p + copy;
	}
	if(stream->bufferptr == stream->bufferend) {
		lseek(stream->fd, (stream->count - 1) * 512, SEEK_SET);
		write(stream->fd, stream->buffer, 512);
		if(flag == 1) {
			p = p + copy;
			flag = 0;
		}
	}
	checkeof = refill(stream);
	if(checkeof == 0) {
		checkeof = 1;
		return (toreadcpy - toread) / size;
	}
	goto myfilelabel;
	return (toreadcpy - toread) / size;
}

/* Writes nmemb items of data, each size bytes long, to the buffer and writes in actual file only when buffer is full
 * Returns the number of items written*/
 
mysize_t myfwrite(const void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream) {
	mysize_t towrite, towritecpy , copy;
	char *p, *bufferptrcpy;
	p = (char *)ptr;
	int rembufspace;
	towrite = size * nmemb;
	towritecpy = towrite;
	if(stream->mode == nread)
		return 0;
	if (towrite == 0 || p == NULL)
    		return 0;
	if(stream->count == 0) {
		refill(stream);
	}
	stream->lastfuncall = nfwrite;
	if(stream->mode == nwrite || stream->mode == nwriteplus || stream->mode == nreadplus) {
		mylabel2:;
		rembufspace = stream->bufferend - stream->bufferptr;
		// If the current buffer is full write it in the file and load the next buffer
		if(rembufspace == 0) {
			lseek(stream->fd, (stream->count - 1) * 512, SEEK_SET);
			write(stream->fd, stream->buffer, 512);
			refill(stream);
			goto mylabel2;
		}
		if(rembufspace >= towrite) {
			copy = towrite;
			rembufspace = rembufspace - copy;
			towrite = towrite - copy;
			bufferptrcpy = stream->bufferptr;
			memcpy(stream->bufferptr, p, copy);
			stream->bufferptr = stream->bufferptr + copy;
			stream->fileptrpos = stream->fileptrpos + copy;
			if(stream->fileptrpos > stream->sizeoffile) {
				//if anything is overwritten
				//Here the subtracted part i.e, after copy - is the overwritten part
				if((bufferptrcpy - stream->buffer) + 1 <= stream->bytesread) {
					stream->sizeoffile = stream->sizeoffile + copy - (stream->bytesread -(bufferptrcpy - stream->buffer));
				}
				//like append mode
				else {
					stream->sizeoffile = stream->sizeoffile + copy;
				}
			}
			if(copy > (stream->bytesread - (bufferptrcpy - stream->buffer))) {
				stream->bytesread = stream->bytesread + copy - (stream->bytesread -(bufferptrcpy - stream->buffer));
			}
			if(towrite == 0)
				goto done;
		
		}
		else if(rembufspace < towrite) {
			copy = rembufspace;
			rembufspace = rembufspace - copy;
			towrite = towrite - copy;
			bufferptrcpy = stream->bufferptr;
			memcpy(stream->bufferptr, p, copy);
			stream->bufferptr = stream->bufferptr + copy;
			stream->fileptrpos = stream->fileptrpos + copy;
			if(stream->fileptrpos > stream->sizeoffile) {
				//if anything is overwritten
				//Here the subtracted part i.e, after copy - is the overwritten part
				if((bufferptrcpy - stream->buffer) + 1 <= stream->bytesread) {
					stream->sizeoffile = stream->sizeoffile + copy - (stream->bytesread -(bufferptrcpy - stream->buffer));
				}
				//like append mode
				else {
					stream->sizeoffile = stream->sizeoffile + copy;
				}
			}
			stream->bytesread = stream->bytesread + copy - (stream->bytesread -(bufferptrcpy - stream->buffer));
			goto mylabel2;              
		}
	}
	// If file is opened in append or appendplus mode
	else if(stream->mode == nappend || stream->mode == nappendplus) {
		mylabel4:;
		rembufspace = stream->bufferend - stream->bufferptr;
		// If the current buffer is full write it in the file and load the next buffer
		if(rembufspace == 0) {
			lseek(stream->fd, 0 , SEEK_END);
			write(stream->fd, stream->buffer, 512);
			refill(stream);
			goto mylabel2;
		}
		if(rembufspace >= towrite) {
			copy = towrite;
			rembufspace = rembufspace - copy;
			towrite = towrite - copy;
			memcpy(stream->bufferptr, p, copy);
			stream->bufferptr = stream->bufferptr + copy;
			stream->fileptrpos = stream->fileptrpos + copy;
			stream->sizeoffile = stream->sizeoffile + copy;
			if(towrite == 0)
				goto done;
		}
		else if(rembufspace < towrite) {
			copy = rembufspace;
			rembufspace = rembufspace - copy;
			towrite = towrite - copy;
			memcpy(stream->bufferptr, p, copy);
			stream->bufferptr = stream->bufferptr + copy;
			stream->fileptrpos = stream->fileptrpos + copy;
			stream->sizeoffile = stream->sizeoffile + copy;
			goto mylabel4;              
		}
	}
	done:;
	return (towritecpy - towrite) / size;
}

// Returns 1 if ENDOFFILE is reached else returns 0

int myfeof(MYFILE *stream) {
	//Self Explainatory
	if(stream->fileptrpos >= stream->sizeoffile) {
		return 1;
	}
	else {
		return 0;
	}
}

int myfseek(MYFILE *stream, long offset, int whence) {
	long int finalbyteno, bufferno;
	char *bufferptrcpy;
	int flag = 0;
	if(stream->mode == nappend) {
		return 0;
	}
	if(stream->count == 0) {
		refill(stream);
	}
	stream->lastfuncall = nfseek;
	switch(whence) {
		case MYSEEK_SET:
			finalbyteno = 0 + offset;
			break;
		case MYSEEK_CUR:
			finalbyteno = stream->fileptrpos + offset;
			break;
		case MYSEEK_END:
			if(stream->fileptrpos > stream->sizeoffile) {
				finalbyteno = stream->fileptrpos + offset;
			}
			else {
				finalbyteno = stream->sizeoffile + offset; 
			}
			break;
		default :
			return -1;
	}
	if(finalbyteno < 0) {
		return -1;
	}
	bufferno = (finalbyteno / 512) + 1;
	/* Dealing with the case when you seek beyond filesize*/
	if(finalbyteno > stream->sizeoffile) {
		if(bufferno > stream->count) {
			if((stream->bufferptr - stream->buffer) > stream->bytesread) {
				// this means some data is written after reading 
				bufferptrcpy = stream->bufferptr;
				while(bufferptrcpy != stream->bufferend) {
					bufferptrcpy[0] = '\0';
					stream->sizeoffile++;
					bufferptrcpy++;
				}
			}
			else {
				//take bufferptr to bytesread and add \0s
				bufferptrcpy = stream->buffer + stream->bytesread;
				while(bufferptrcpy != stream->bufferend) {
					stream->sizeoffile++;
					bufferptrcpy[0] = '\0';
					bufferptrcpy++;
				}
			}
			while(bufferno > stream->count) {
				writebuffer(stream);
				refillw0(stream);
				flag = 1;
				refill(stream);
			}
		}
		bufferptrcpy = stream->bufferptr;
		stream->bufferptr = stream->buffer + (finalbyteno - 512 * (stream->count - 1)); 
		stream->fileptrpos = finalbyteno;
		if(flag == 1) {
			stream->sizeoffile = 512 * (bufferno - 1) + (stream->bufferptr - stream->buffer);
		}
		if(bufferno == 1) {
			bufferptrcpy = stream->buffer + stream->bytesread;
			while(bufferptrcpy != stream->bufferptr) {
				bufferptrcpy[0] = '\0';
				stream->bytesread++;
				stream->sizeoffile++;
				bufferptrcpy++;
			}
		}
	}
	/*Seeking within the limit of filesize*/
	else {
		/*write current buffer;
		load the right buffer;
		set bufferptr right;*/
		if(stream->count != bufferno) {
				writebuffer(stream);
				stream->count = bufferno - 1;
				refill(stream);
		}
		stream->bufferptr = stream->buffer + (finalbyteno - 512 * (stream->count - 1)); 
		stream->fileptrpos = finalbyteno;
	}
	return 0;
}

long myftell(MYFILE *stream) {
	if(stream->mode == 100 || stream->mode == 150 || stream->mode == 200 || stream->mode == 250 || stream->mode == 300 || stream->mode == 350)
		return stream->fileptrpos;
	else
		return -1;
}

// Closes the file but before it writes any pending data and frees any malloced memory in the program

int myfclose(MYFILE *stream) {
	if(stream == NULL)
		return 0;
	if(stream->count != 0) {
		if(stream->mode == 150 || stream->mode == 200 || stream->mode == 250 || stream->mode == 350) {
			lseek(stream->fd, (stream->count - 1) * 512, SEEK_SET);
			write(stream->fd, stream->buffer, stream->bytesread);
		}
		else if(stream->mode == 300) {
			lseek(stream->fd, 0, SEEK_END);
			write(stream->fd, stream->buffer, (stream->bufferptr - stream->buffer));
		}
	}
	if(stream->mode == 100 || stream->mode == 150 || stream->mode == 200 || stream->mode == 250 || stream->mode == 300 || stream->mode == 350) {
		free(stream->buffer);
		free(stream);
	}
	return 1;
}
 
int myfgetpos(MYFILE *stream, myfpos_t *pos) {
	char arr[10];
	if(stream->mode == 100 || stream->mode == 150 || stream->mode == 200 || stream->mode == 250 || stream->mode == 300 || stream->mode == 350) {
		sprintf(arr, "%d", stream->fd);
		pos->position = myftell(stream);
		strcpy(pos->key, "nikhil");
		strcat(pos->key, arr);
		return 0;
	}
	return -1;
}

int myfsetpos(MYFILE *stream, const myfpos_t *pos) {
	char arr[10] ,arr1[20] = "nikhil";
	if(stream->mode == 100 || stream->mode == 150 || stream->mode == 200 || stream->mode == 250 || stream->mode == 350) {
		sprintf(arr, "%d", stream->fd);
		strcat(arr1, arr);
		if(strcmp(pos->key, arr1) == 0) {
			myfseek(stream, pos->position, MYSEEK_SET);
			return 0;
		}
	}
	return -1;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
