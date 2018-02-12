#include <stdio.h> // for printf()
#include <sys/stat.h>
#include <stdlib.h> //for malloc()
#include <fcntl.h> //for open()
#include <time.h> // for ctime(time_t time) --> takes in the time in unix format and returns it in the string form.
#include <errno.h> // for accessing errno.


int main(void)
{
	int fd;
	struct stat *file_stat = (struct stat *)malloc(sizeof(struct stat));

	
	if((fd= open("/home/akhil/Files/Books/The C Programming Language.pdf", O_RDONLY))<0)
	{
		printf("errno = %d \n", errno);
		perror("open");
	}
	
	if(fstat(fd, file_stat)<0)
	{
		printf("errno = %d \n", errno);
		perror("fstat");
	}

	printf(" last accessed time: %s ", ctime(&(file_stat->st_atime)));
	printf("last modified time: %s ", ctime(&(file_stat->st_mtime)));
	printf("last changed time: %s ", ctime(&(file_stat->st_ctime)));
	return 0;
}

