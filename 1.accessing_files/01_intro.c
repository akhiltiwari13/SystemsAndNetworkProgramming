#include <fcntl.h> // stands for "file control"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h> // has the system calls write() and close()

int main(void)
{
	int fd; // file descriptor value for the file to be opened.


	if((fd = open("foo", O_WRONLY|O_CREAT, 0644))<0) //most system calls return -1 on faliure and then set the errno global variable.
	{
		printf("error number : %d \n", errno);
		perror("foo");
		exit(1); // terminates the program with 1 returned to the OS.
	}
	else
	{
		write(fd, "hello world", 11);
		close(fd);
	}
	return 0;
}
