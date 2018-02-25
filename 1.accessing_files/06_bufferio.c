#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("usage: %s <block_size> <block_count>\n", argv[0]);
		exit(1);
	}
	else
	{
		FILE *fd;
		int blk_cnt , blk_size;
		blk_size=atoi(argv[1]);
		blk_cnt=atoi(argv[2]);
		char *buffer= malloc(blk_size); // buffer of the block size to read and write.

		fd = fopen("bufferio.out", "w");

		for(int i=0; i<=blk_cnt; i++)
		{
			fwrite(buffer, blk_size, 1, fd);
		}

		fclose(fd);
	}
}
