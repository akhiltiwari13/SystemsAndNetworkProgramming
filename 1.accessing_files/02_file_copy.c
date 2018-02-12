#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#define BSIZE 16384 // this defines the total byte size

int main(void)
{
	int fd_in, fd_out;
	int count;
	char buffer[BSIZE];

	// the third parameter of the open() system call is of relevence only when the file is being created.
	if((fd_in=open("infoo", O_RDONLY))==-1) // most system calls return -1 on faliure.
	{
		printf("error number : %d \n", errno);
		perror("foo");
		exit(1); // terminates the program with 1 returned to the OS.
	}
	if((fd_out=open("outfoo", O_WRONLY | O_CREAT, 0644))==-1) // | is the bitwise OR operator and not a pipe.
	{
		printf("error number : %d \n", errno);
		perror("foo");
		exit(2); // terminates the program with 2 returned to the OS.
	}

	while((count=read(fd_in, buffer, BSIZE)) >0) //read() returns 0 when it encouters EOF.
	{
		write(fd_out, buffer, count); //wouldn't the write() system call fail?
	}
	// close the extra file descriptors allocated to the program.
	close(fd_in);
	close(fd_out);

	return 0;
}

