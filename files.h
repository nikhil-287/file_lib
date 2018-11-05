#include<sys/stat.h>
#define nread 100
#define nreadplus 150
#define nwrite 200
#define nwriteplus 250
#define nappend 300
#define nappendplus 350
typedef struct MYFILE {
	int fd;			//file descriptor
	char *buffer;		//actual buffer
	char *bufferptr;	//pointer to actual position in buffer
	char *bufferend;
	struct stat st;		//to calculate sizeoffile
	int mode;		//the mode in which file is opened
	int sizeoffile;		//size of file using stat
	char *readptr;		
	char *writeptr;
	int count;    		//no of buffers
	int bytesread;		//no of bytesread in buffer
	int countfull; 		// no of full buffers
}MYFILE;
typedef unsigned long long int mysize_t;
MYFILE *myfopen(const char *pathname, const char *mode);/*1*/
mysize_t myfread(void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream);
int myfeof(MYFILE *stream);
int refill(MYFILE *stream);
mysize_t myfwrite(const void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream);
/*int myfgetpos(MYFILE *stream, fpos_t *pos);
int myfsetpos(MYFILE *stream, const fpos_t *pos);
int myfseek(MYFILE *stream, long offset, int whence);
long myftell(MYFILE *stream);
int myfclose(MYFILE *stream);*/


 
 
 
 
 
 
 
 
 
 

