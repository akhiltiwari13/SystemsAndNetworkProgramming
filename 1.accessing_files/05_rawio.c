#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("usage: %s <block_size> <block_count>\n", argv[0]);
		exit(1);
	}
	else
	{
		int fd, blk_cnt , blk_size;
		blk_size=atoi(argv[1]);
		blk_cnt=atoi(argv[2]);
		char *buffer= malloc(blk_size); // buffer of the block size to read and write.

		fd = open("rawio.out", O_WRONLY | O_CREAT | O_TRUNC, 0600);

		for(int i=0; i<=blk_cnt; i++)
		{
			write(fd, buffer, blk_size);
		}

		close(fd);
	}
}
