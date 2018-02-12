#include <stdio.h> // for printf()
#include <sys/stat.h>
#include <stdlib.h> //for malloc()
#include <dirent.h> //for all the directory related stuff, including the macro PATH_MAX.
#include <unistd.h> // getcwd();
#include <errno.h> // for accessing errno.
#include <fcntl.h> // for open() system call.
#include <time.h>

void err_chk_sys_call(int return_stat)
{
	if(return_stat<0)
	{
		printf("errno: %d\n", errno);
		perror("error_message");
		exit(1);

	}
}

void file_stat(char *file_path)
{
	char *file_type[]= {"unknown", "FIFO(pipe)", "character device", "unknown", "directory", "unknown", "block device", "unknown", "regular file", "unknown", "unknown", "unknown", "symbolic link", "unknown", "socket"};
	int fd;
	struct stat *file_stat = (struct stat *)malloc(sizeof(struct stat));

	err_chk_sys_call((fd= open(file_path, O_RDONLY))<0);
	
	// have all the information (stat) of that file.
	err_chk_sys_call(fstat(fd, file_stat)<0);
	
	// how to check the file types...
	/* shifting all the bits to the left by 12 places and then (bitwise) anding them with 15 to mask all but the firts 4 bits. */
	printf("---------------------------------------------------\n");
	printf("file name:          %s \n", file_path);
	printf("file type:          %s \n", file_type[((file_stat->st_mode) >> 12) & 15]) ;
	printf("                    user  group  others \n");
	printf("file permissions:   %c%c%c   %c%c%c    %c%c%c \n",
			((file_stat->st_mode) & S_IRUSR)?'r':'-', ((file_stat->st_mode) & S_IWUSR)?'w':'-', ((file_stat->st_mode) & S_IXUSR)?'x':'-',
			((file_stat->st_mode) & S_IRGRP)?'r':'-', ((file_stat->st_mode) & S_IWGRP)?'w':'-', ((file_stat->st_mode) & S_IXGRP)?'x':'-',
			((file_stat->st_mode) & S_IROTH)?'r':'-', ((file_stat->st_mode) & S_IWOTH)?'w':'-', ((file_stat->st_mode) & S_IXOTH)?'x':'-'
	      );
	printf("size              : %ld\n", file_stat->st_size);
	printf("last accessed time: %s", ctime(&(file_stat->st_atime)));
	printf("last modified time: %s", ctime(&(file_stat->st_mtime)));
	printf("last changed  time: %s", ctime(&(file_stat->st_ctime)));
	printf("---------------------------------------------------\n");

}

int main(int argc, char *argv[])
{
	if(argc !=2)
	{
		printf("usage: %s <path to a directory>\n", argv[0]);
		exit(1);
	}
	err_chk_sys_call(chdir(argv[1])<0);
	DIR *d= opendir(".");
	struct dirent *info= malloc(sizeof(struct dirent ));
	while((info= readdir(d)))
	{
		file_stat(info->d_name);
	}
	return 0;
}
