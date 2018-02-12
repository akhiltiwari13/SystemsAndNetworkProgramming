#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> // needed for using the library function memcpy()


int main(void)
{

	int fd_in, fd_out;
	char *src, *dest;
	size_t size;


	if((fd_in = open("mmapin.txt", O_RDONLY))<0)
	{
		printf("error number : %d \n", errno);
		perror("fd_in");
		exit(1); // terminates the program with 1 returned to the OS.
	}

	size= lseek(fd_in, 0, SEEK_END); //lseek can be used to determine the size of the file in bytes.

	if((src = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd_in, 0)) == MAP_FAILED)
	{
		printf("error number : %d \n", errno);
		perror("mmap");
		exit(2);
	}

	if((fd_out = open("mmapout.txt", O_RDWR| O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR))<0)
	{
		printf("error number : %d \n", errno);
		perror("fd_out");
		exit(3); // terminates the program with 1 returned to the OS.
	}

	if((ftruncate(fd_out, size))==-1)
	{
		perror("ftruncate");
		exit(4);
	}

	if((dest = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0)) == MAP_FAILED)
	{
		printf("error number : %d \n", errno);
		perror("mmap write");
		exit(5);
	}

	// copy bytes between the memories.
	memcpy(dest, src, size); // pointers to initial addresses of the source and destination arrays and  the number of bytes to be copied.

	// it's a good practice to do a msync, however if not done, it gets done automatically as the program execution completes.
	if(msync(dest, size, MS_SYNC) == -1) 
	{
		printf("error number : %d \n", errno);
		perror("msync");
	}



	return 0;
}
