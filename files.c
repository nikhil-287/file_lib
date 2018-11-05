#include"files.h"
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
int refill(MYFILE *stream) {
	if(myfeof(stream) == 1) {
		return 0;
	}
	stream->bytesread = read(stream->fd, stream->buffer, 512);
	stream->bufferptr = stream->buffer;
	stream->count++;
	if(stream->bytesread == 512)
		stream->countfull++;
	return 1;	
}
MYFILE *init(const char *pathname) {
	MYFILE *f;
	f = (MYFILE*)malloc(sizeof(MYFILE));
	stat(pathname, &(f->st));
	f->sizeoffile = f->st.st_size;
 	f->buffer = (char *)malloc(512);
 	f->bufferptr = f->buffer;
 	f->bufferend = f->buffer + 512;
 	f->count = 0;
 	f->countfull = 0;
 	return f;
}
MYFILE *myfopen(const char *pathname, const char *mode) {
 	MYFILE *f;
 	if(strcmp(mode, "r") == 0) {
 		f = init(pathname);
 		f->mode = nread;
 		f->fd = open(pathname, O_RDONLY, S_IRUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else
			return f;
 	}
 	else if(strcmp(mode, "w") == 0) {
 		f = init(pathname);
 		f->mode = nwrite;
 		f->fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else
			return f;
 	}
 	else if(strcmp(mode, "a") == 0) {
 		f = init(pathname);
 		f->mode = nappend;
 		f->fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, S_IWUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else
			return f;
 	}
 	else if(strcmp(mode, "r+") == 0) {
 		f = init(pathname);
 		f->mode = nreadplus;
 		f->fd = open(pathname, O_RDWR, S_IRUSR | S_IWUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else
			return f;
 	}
 	else if(strcmp(mode, "w+") == 0) {
 		f = init(pathname);
 		f->mode = nwriteplus;
 		f->fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else
			return f;
 	}
 	else if(strcmp(mode, "a+") == 0) {
 		f = init(pathname);
 		f->mode = nappendplus;
 		f->fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
		if(f->fd == -1) {
			return NULL;
		}
		else
			return f;
 	}
	else
		return NULL;
		
 }
mysize_t myfread(void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream) {
	mysize_t toread, toreadcpy, copy;
	char *p = (char *)ptr;
	int checkeof;
	toread = size * nmemb;
	toreadcpy = toread;
	if(stream->mode == nwrite || stream->mode == nappend)
		return 0;
	if (/*myfeof(stream) ||*/ toread == 0 || p == NULL)
    		return 0;
	if(stream->count == 0) {
		checkeof = refill(stream);
	}
	if(checkeof == 0) {
		return 0;
	}
	//Dealing with residue in buffer
	myfilelabel:;
	if(stream->bufferptr < stream->bufferend) {
		copy = stream->bufferend - stream->bufferptr;
		if(copy > toread) {
			copy = toread;
		}
		toread = toread - copy;
		if(stream->bytesread >= copy) {
			memcpy(p, stream->bufferptr, copy);
		}
		else {
			memcpy(p, stream->bufferptr, stream->bytesread);
			/*for(i = 0; i < stream->bytesread - 1; i++)
				p[i] = stream->bufferptr[i];
			printf("%d\n", stream->bytesread);*/
			return (toreadcpy - stream->bytesread) / size;
		}
		stream->bufferptr = stream->bufferptr + copy;
	}
	if(toread == 0) {
		return nmemb;
	}
	p = p + copy;
	
	checkeof = refill(stream);
	if(checkeof == 0) {
		return (toreadcpy - toread) / size;
	}
	goto myfilelabel;
	return (toreadcpy - toread) / size;
	
}
mysize_t myfwrite(const void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream) {
	mysize_t towrite, towritecpy, written, copy;
	char *p = (char *)ptr;
	int checkeof, i, rembufspace;
	written = 0;
	towrite = size * nmemb;
	towritecpy = towrite;
	if(stream->mode == nread)
		return 0;
	if (myfeof(stream) || towrite == 0 || p == NULL;)
    		return 0;
	if(stream->count == 0) {
		checkeof = refill(stream);
	}
	if(checkeof == 0) {
		return 0;
	}
	rembufspace = 512 - (stream->bufferptr - stream->buffer);
	if(rembufspace >= towrite)
	mylabel2:;
	
	
}
int myfeof(MYFILE *stream) {
	//internal file pointer is on total_bytes
	mysize_t total_bytes;
	if(stream->countfull == stream->count) {
		total_bytes = stream->count * 512;
	}
	else {
		total_bytes = stream->countfull * 512 + stream->bytesread;
	}
	if(stream->sizeoffile == total_bytes) {
		return 1;
	}
	else
		return 0;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
