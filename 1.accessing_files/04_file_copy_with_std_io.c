#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define BSIZE 16384 // this defines the total byte size

int main(void)
{
	FILE *fin, *fout;
	char buffer[BSIZE];
	int count;

	if(!(fin=fopen("fin.txt", "r")))
	{
		printf("error number : %d \n", errno);
		perror("fin.txt");
		exit(1); // terminates the program with 1 returned to the OS.
	}
	if(!(fout=fopen("fout.txt", "w")))
	{
		printf("error number : %d \n", errno);
		perror("fout.txt");
		exit(2); // terminates the program with 1 returned to the OS.
	}
	while((count = fread(buffer, 1,BSIZE, fin))>0)
	{
		fwrite(buffer, 1, count, fout);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
