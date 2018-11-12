#include"files.h"
#include"testcode.h"
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
void test1() {
	printf("\n*This test demonstrates fsetpos and fgetpos\n\n");
	MYFILE *fp;
	myfpos_t pos;
	char arr[32] = "DSAPROJECT";
	fp = myfopen("test1.txt", "r+");
	if(fp == NULL) {
		perror("Error : ");
		exit(errno);
	}
	myfgetpos(fp, &pos);			//pos structure has stored current position i.e., beginning of file
	printf("Remembered the start of file with fgetpos\n\n");
	myfwrite(arr, 10, 1, fp);		//array has been written in the file
	myfsetpos(fp, &pos);			
	/*Print the file*/
	printf("Wrote DSAPROJECT in an empty file\n\n Now contents of file\n\n");
	while(!myfeof(fp)) {
		myfread(arr, 1, 1, fp);
		printf("%c", arr[0]);
	}
	printf("\n");
	myfsetpos(fp , &pos);
	printf("Seeked to start of file with fsetpos and replaced first 3 letters with SLS\n\n");	
	//Now file ptr position has been set to position pointed by variable in pos structure i.e., beginning of file
	arr[0] = 'S';
	arr[1] = 'L';
	arr[2] = 'S';	
	myfwrite(arr, 3, 1, fp);
	myfsetpos(fp , &pos);
	/*Print the file*/
	printf("Current content of file\n\n");
	while(!myfeof(fp)) {
		myfread(arr, 1, 1, fp);
		printf("%c", arr[0]);
	}
	myfclose(fp);
}

void test2() {
	printf("\n*This test demonstrates fread function\n\n");
	MYFILE *fp;
	int i;
	char arr[32];
	fp = myfopen("test2.txt", "r");
	printf("The file has 24 bytes in it\n\n");
	if(fp == NULL) {
		perror("Error : ");
		exit(errno);
	}
	printf("Reading 16 bytes\n\n");
	i = myfread(arr, 1, 16, fp);
	printf("Myfread returned %d\n\n", i);
	arr[i] = '\0';
	printf("Read Data\n\n");
	puts(arr);
	printf("\n");
	i = myftell(fp);
	printf("Ftell returned %d\n\n", i);
	myfseek(fp, 0, MYSEEK_SET);
	i = myftell(fp);
	printf("After Seeking to start of file Ftell returned %d\n\n", i);
	printf("Now reading 30 bytes which is more than sizeoffile (24)\n\n");
	i = myfread(arr, 1, 30, fp);
	printf("Myfread returned %d\n\n", i);
	arr[i] = '\0';
	printf("Read Data\n\n");
	puts(arr);
	printf("\n");
	i = myftell(fp);
	printf("Ftell returned %d\n", i);
	myfclose(fp);
}

void test3() {
	printf("\n*This test demonstrates Copying contents of 1 file to another\n\n");
	MYFILE *fp1, *fp2;
	char arr[32];
	fp1 = myfopen("test3.txt", "r");
	if(fp1 == NULL) {
		perror("Error : ");
		exit(errno);
	}
	fp2 = myfopen("copy.txt", "w+");
	if(fp2 == NULL) {
		perror("Error : ");
		exit(errno);
	}
	/*Print the file to be copied*/
	printf("File which is to be copied\n");
	while(!myfeof(fp1)) {
		myfread(arr, 1, 1, fp1);
		myfwrite(arr, 1, 1, fp2);
		printf("%c", arr[0]);
	}
	printf("\n");
	myfclose(fp1);
	myfclose(fp2);
	printf("Check copy.txt\n");
}

void test4() {
	printf("\n*This test demonstrates seeking beyond filesize and writing\n\n");
	MYFILE *fp1;
	int i;
	char arr[256] = "Nikhilkoli";
	fp1 = myfopen("test4.txt", "r+");
	if(fp1 == NULL) {
		perror("Error : ");
		exit(errno);
	}
	i = myftell(fp1);
	printf("Ftell returned %d\n\n", i);
	printf("Seeking to byte no 10 which is greater than filesize\n\n");
	// 1st byte has filptrpos as 0 hence 9
	myfseek(fp1, 9, MYSEEK_SET);
	i = myftell(fp1);
	printf("After Seeking to the location Ftell returned %d\n\n", i);
	printf("Now writing 10 bytes\n\n");
	i = myfwrite(arr, 1, 10, fp1);
	printf("Myfwrite returned %d\n\n", i);
	printf("Check file test4.txt\n");
	myfclose(fp1);
}

void test5() {
	printf("\n*This test demonstrates append mode\n\n");
	MYFILE *fp1;
	char arr[32];
	fp1 = myfopen("test5.txt", "r");
	if(fp1 == NULL) {
		perror("Error : ");
		exit(errno);
	}
	printf("File before appending\n\n");
	while(!myfeof(fp1)) {
		myfread(arr, 1, 1, fp1);
		printf("%c", arr[0]);
	}
	printf("\n\n");
	myfclose(fp1);
	fp1 = myfopen("test5.txt", "a");
	strcpy(arr, "modeisworking");
	myfwrite(arr, 1, 13, fp1);
	myfclose(fp1);
	printf("Check file test5.txt\n");
}

void test6() {
	printf("\n*This test demonstrates that reading works when seeked into another bufferno than current bufferno\n\n");
	MYFILE *fp1;
	int i;
	char arr[256];
	fp1 = myfopen("test6.txt", "r");
	if(fp1 == NULL) {
		perror("Error : ");
		exit(errno);
	}
	i = myftell(fp1);
	printf("Ftell returned %d\n\n", i);
	printf("Reading 16 bytes\n\n");
	i = myfread(arr, 1, 16, fp1);
	printf("Myfread returned %d\n\n", i);
	arr[i] = '\0';
	printf("Read Data\n\n");
	puts(arr);
	printf("\n");
	i = myftell(fp1);
	printf("Ftell returned %d\n\n", i);
	printf("Seeking to byte no 1031\n\n");
	// 1st byte has filptrpos as 0 hence 1030
	myfseek(fp1, 1030, MYSEEK_SET);
	i = myftell(fp1);
	printf("After Seeking to the location Ftell returned %d\n\n", i);
	printf("Now reading 30 bytes\n\n");
	i = myfread(arr, 1, 30, fp1);
	printf("Myfread returned %d\n\n", i);
	arr[i] = '\0';
	printf("Read Data\n\n");
	puts(arr);
	printf("\n");
	printf("Seeking to start of file\n\n");
	// 1st byte has filptrpos as 0 hence 1030
	myfseek(fp1, 0, MYSEEK_SET);
	i = myftell(fp1);
	printf("After Seeking to the location Ftell returned %d\n\n", i);
	printf("Now reading 30 bytes\n\n");
	i = myfread(arr, 1, 30, fp1);
	printf("Myfread returned %d\n\n", i);
	arr[i] = '\0';
	printf("Read Data\n\n");
	puts(arr);
	printf("\n");
	myfclose(fp1);
}

void test7() {
	printf("\n*This test demonstrates working of fopen\n\n");
	MYFILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6;
	fp1 = myfopen("test7a.txt", "r");
	if(fp1 == NULL) {
		perror("Error : ");
	}
	else {
		printf("File opened in read mode with file descriptor = %d\n\n", fp1->fd);
	}	
	fp2 = myfopen("test7b.txt", "r+");
	if(fp2 == NULL) {
		perror("Error : ");
	}
	else {
		printf("File opened in read+ mode with file descriptor = %d\n\n", fp2->fd);
	}
	fp3 = myfopen("test7c.txt", "w");
	if(fp3 == NULL) {
		perror("Error : ");
	}
	else {
		printf("File opened or created if not present in write mode with file descriptor = %d\n\n", fp3->fd);
	}
	fp4 = myfopen("test7d.txt", "w+");
	if(fp4 == NULL) {
		perror("Error : ");
	}
	else {
		printf("File opened or created if not present in write+ mode with file descriptor = %d\n\n", fp4->fd);
	}
	fp5 = myfopen("test7e.txt", "a");
	if(fp5 == NULL) {
		perror("Error : ");
	}
	else {
		printf("File opened or created if not present in append mode with file descriptor = %d\n\n", fp5->fd);
	}
	fp6 = myfopen("test7f.txt", "a+");
	if(fp6 == NULL) {
		perror("Error : ");
	}
	else {
		printf("File opened or created if not present in append+ mode with file descriptor = %d\n\n", fp6->fd);
	}
	myfclose(fp1);
	myfclose(fp2);
	myfclose(fp3);
	myfclose(fp4);
	myfclose(fp5);
	myfclose(fp6);
}

void test8() {
	printf("\n*This test demonstrates that writing works when seeked into another bufferno than current bufferno\n\n");
	MYFILE *fp1;
	int i;
	char arr[256] = "Nikhilkoli";
	fp1 = myfopen("test8.txt", "r+");
	if(fp1 == NULL) {
		perror("Error : ");
		exit(errno);
	}
	printf("Trying to seek behind the file");
	myfseek(fp1, -6, MYSEEK_SET);
	i = myftell(fp1);
	printf("Ftell returned %d\n\n", i);
	printf("Writing 10 bytes\n\n");
	i = myfwrite(arr, 1, 10, fp1);
	printf("Myfwrite returned %d\n\n", i);
	i = myftell(fp1);
	printf("Ftell returned %d\n\n", i);
	printf("Seeking to byte no 1031\n\n");
	// 1st byte has filptrpos as 0 hence 1030
	myfseek(fp1, 1030, MYSEEK_SET);
	i = myftell(fp1);
	printf("After Seeking to the location Ftell returned %d\n\n", i);
	printf("Now writing 10 bytes\n\n");
	i = myfwrite(arr, 1, 10, fp1);
	printf("Myfwrite returned %d\n\n", i);
	printf("Seeking to start of file\n\n");
	// 1st byte has filptrpos as 0 hence 1030
	myfseek(fp1, 0, MYSEEK_SET);
	i = myftell(fp1);
	printf("After Seeking to the location Ftell returned %d\n\n", i);
	printf("Now reading 30 bytes\n\n");
	i = myfread(arr, 1, 30, fp1);
	printf("Myfread returned %d\n\n", i);
	arr[i] = '\0';
	printf("Read Data\n\n");
	puts(arr);
	printf("\n");
	myfclose(fp1);
}
