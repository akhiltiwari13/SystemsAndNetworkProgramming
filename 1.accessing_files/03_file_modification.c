#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

typedef struct{
	int id;
	char name[10];
}record;

int main(void)
{
	int record_size= sizeof(record);
	int file_descriptor;
	record *buffer = (record *)malloc(sizeof(record));
	printf("record size= %d", record_size);

	if((file_descriptor = open("readfile.txt", O_RDWR))<0) //open the file in read write mode for modification.
	{
		printf("error number : %d \n", errno);
		perror("readfile.txt");
		exit(1); // terminates the program with 1 returned to the OS.
	}
	else
	{
		lseek(file_descriptor, record_size, SEEK_SET);
		read(file_descriptor, buffer, record_size);
		puts("record read: ");
		printf("id = %d \t name = %s \n", buffer->id, buffer->name);
		lseek(file_descriptor, record_size, SEEK_CUR);
		buffer->id=5;
		buffer->name[0] = 'A';
		write(file_descriptor, &buffer, record_size);
		puts("record modified as");
		printf("id = %d \t name = %s \n", buffer->id, buffer->name);
	}

	return 0;
}
