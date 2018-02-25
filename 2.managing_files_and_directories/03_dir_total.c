#include <stdio.h> // for printf()
#include <sys/stat.h>
#include <stdlib.h> //for malloc()
#include <dirent.h> //for all the directory related stuff, including the macro PATH_MAX.
#include <unistd.h> // getcwd();
#include <errno.h> // for accessing errno.

void err_chk_sys_call(int return_stat)
{
	if(return_stat<0)
	{
		printf("errno: %d\n", errno);
		perror("error_message");
		exit(1);

	}
}

int main(void)
{
	/* this is not working --> */ //char *buffer = (char *)malloc(sizeof(PATH_MAX));
	char *buffer = NULL;
	char *buffer_changed = NULL;
	printf("current directory:  %s\n", buffer= getcwd( buffer, PATH_MAX ) );
	printf("changing........ \n");
	err_chk_sys_call(chdir( "/home/akhil/")<0);
	printf("current directory: %s\n", getcwd( buffer_changed, PATH_MAX ) );
	printf("reverting....\n");
	err_chk_sys_call(chdir(buffer)<0);
	printf("current directory: %s\n", getcwd( buffer, PATH_MAX ) );

	DIR *d= opendir(".");
	struct dirent *info= malloc(sizeof(struct dirent ));
	struct stat *file_stat=malloc(sizeof(struct stat));
	int total=0;

	while((info= readdir(d)))
	{
		// stat( DIR *, struct stat *), fstat(int fd, struct stat *)
		err_chk_sys_call( stat( info->d_name, file_stat)<0);
		total +=file_stat->st_size;
	}
	printf("total size: %d\n", total);

	return 0;
}
