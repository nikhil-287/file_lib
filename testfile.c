#include"files.h"
#include<stdio.h>
int main() {
	MYFILE *fp;
	int i = 0;
	char arr[1024];
	fp = myfopen("data.txt", "r");
	i = myfread(arr, 1, 1011, fp);
	arr[1011] = '\0';
	printf("i = %d\n", i);
	puts(arr);
	/*i = myfread(arr, 1, 1, fp);
	arr[1] = '\0';
	printf("i = %d\n", i);
	puts(arr);
	i = myfread(arr, 1, 1, fp);
	arr[1] = '\0';
	printf("i = %d\n", i);
	puts(arr);*/
	return 0;
}

