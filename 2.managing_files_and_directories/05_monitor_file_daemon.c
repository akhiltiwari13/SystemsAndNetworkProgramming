#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void err_chk_sys_call(int return_stat, char *err_msg)
{
	if(return_stat<0)
	{
		printf("errno: %d\n", errno);
		perror("system error_message");
		if(err_msg)
			printf("%s\n", err_msg);
		exit(errno);

	}
}

int main(void)
{
	FILE *fd_monitor, *fd_log; // file descriptor for openinng the monitor.conf and the monitor.log files.
	char file_name[PATH_MAX]; // a buffer for storing the name of a file.
	struct stat *file_stat= (struct stat *)malloc(sizeof(struct stat));

	err_chk_sys_call((fd_monitor = fopen("./monitor.conf", "r"))?1:-1, "-->unable to open monitor.conf, exiting...");
	
	// reading the files specified in monitor.conf
	while(fgets(file_name, PATH_MAX, fd_monitor))
	{
		printf("%s", file_name);
		// the last character of the filename is the new line character which needs to be replaced with the terminating null character.
		file_name[strlen(file_name)-1]='\0';
		err_chk_sys_call(stat(file_name, file_stat), "-->unable to stat() the file");
	}

	

	return 0;
}
