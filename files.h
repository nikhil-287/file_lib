#include<sys/stat.h>
#define BUFSIZE 512
#define nread 100
#define nreadplus 150
#define nwrite 200
#define nwriteplus 250
#define nappend 300
#define nappendplus 350
#define nfwrite 500
#define nfread 510
#define nfseek 520
#define MYSEEK_END 600
#define MYSEEK_SET 610
#define MYSEEK_CUR 630

typedef struct MYFILE {
	int fd;			//file descriptor
	char *buffer;		//actual buffer
	char *bufferptr;	//pointer to actual position in buffer
	char *bufferend;	//self explainatory
	struct stat st;		//to calculate sizeoffile
	int mode;		//the mode in which file is opened
	int sizeoffile;		//size of file using stat
	const char *nameoffile;	//copy of pathname from fopen
	int count;    		//no of buffers
	int bytesread;		//no of bytesread in buffer
	int countfull; 		// no of full buffers
	int lastfuncall;	//last called function
	int fileptrpos;		//our file pointer position
}MYFILE;

typedef struct myfpos_t {
	int position;
	char key[20];
}myfpos_t;

typedef unsigned long long int mysize_t;
MYFILE *myfopen(const char *pathname, const char *mode);
mysize_t myfread(void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream);
int myfeof(MYFILE *stream);
int refill(MYFILE *stream);
int myfseek(MYFILE *stream, long offset, int whence);
mysize_t myfwrite(const void *ptr, mysize_t size, mysize_t nmemb, MYFILE *stream);
int myfgetpos(MYFILE *stream, myfpos_t *pos);
int myfsetpos(MYFILE *stream, const myfpos_t *pos);
long myftell(MYFILE *stream);
int myfclose(MYFILE *stream);


 
 
 
 
 
 
 
 
 
 

