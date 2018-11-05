#include<stdio.h>
#include<sys/stat.h>
int main() {
	FILE *fp;
	fp = fopen("data.txt", "r");
	int i;
	char arr[111];
	i = fread(arr, 1, 5, fp);
	arr[5] = '\0';
	printf("i = %d\n", i);
	puts(arr);
	i = fread(arr, 1, 1, fp);
	arr[1] = '\0';
	printf("i = %d\n", i);
	puts(arr);
	i = fread(arr, 1, 1, fp);
	arr[1] = '\0';
	printf("i = %d\n", i);
	puts(arr);
	return 0;
}

