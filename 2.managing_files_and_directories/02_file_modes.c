#include <stdio.h> // for printf()
#include <sys/stat.h>
#include <stdlib.h> //for malloc()
#include <fcntl.h> //for open()
#include <time.h> // for ctime(time_t time) --> takes in the time in unix format and returns it in the string form.
#include <errno.h> // for accessing errno.


int main(int argc, char *argv[])
{
	char *file_type[]= {"unknown", "FIFO(pipe)", "character device", "unknown", "directory", "unknown", "block device", "unknown", "regular file", "unknown", "unknown", "unknown", "symbolic link", "unknown", "socket"};
	int fd;
	struct stat *file_stat = (struct stat *)malloc(sizeof(struct stat));

	if(argc !=2)
	{
		printf("usage: %s <path to file>\n", argv[0]);
		exit(1);
	}
	if((fd= open(argv[1], O_RDONLY))<0)
	{
		printf("errno = %d \n", errno);
		perror("can't open the file");
		exit(2);
	}
	// have all the information (stat) of that file.
	if(fstat(fd, file_stat)<0)
	{
		printf("errno = %d \n", errno);
		perror("fstat");
		exit(3);
	}
	/* for debugging --> */ printf("the number= %d \n", ((file_stat->st_mode) >> 12) & 15);
	// how to check the file types...
	/* shifting all the bits to the left by 12 places and then (bitwise) anding them with 15 to mask all but the firts 4 bits. */
	printf("file type:          %s \n", file_type[((file_stat->st_mode) >> 12) & 15]) ;

	printf("                    user  group  others \n");
	printf("file permissions:   %c%c%c   %c%c%c    %c%c%c \n",
			((file_stat->st_mode) & S_IRUSR)?'r':'-', ((file_stat->st_mode) & S_IWUSR)?'w':'-', ((file_stat->st_mode) & S_IXUSR)?'x':'-',
			((file_stat->st_mode) & S_IRGRP)?'r':'-', ((file_stat->st_mode) & S_IWGRP)?'w':'-', ((file_stat->st_mode) & S_IXGRP)?'x':'-',
			((file_stat->st_mode) & S_IROTH)?'r':'-', ((file_stat->st_mode) & S_IWOTH)?'w':'-', ((file_stat->st_mode) & S_IXOTH)?'x':'-'
	      );

	printf("last accessed time: %s", ctime(&(file_stat->st_atime)));
	printf("last modified time: %s", ctime(&(file_stat->st_mtime)));
	printf("last changed  time: %s", ctime(&(file_stat->st_ctime)));
	return 0;
}

