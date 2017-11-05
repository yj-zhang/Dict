#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
int my_read(int,void *,int);
int main()
{
	printf("compile successful\n");
	return 0;
}
int my_read(int fd, void *buffer, int length)
{
	int bytes_len;
	int read_bytes;
	char *ptr;
	ptr = buffer;
	bytes_len = length;
	while(bytes_len > 0)
	{
		read_bytes = read(fd, ptr, bytes_len);
		if(read_bytes < 0)
		{
			if(errno == EINTR)
				read_bytes = 0;
			else
				return -1;
		}
		else if(read_bytes == 0)
			break;
		bytes_len -= read_bytes;
		ptr += read_bytes;
	}
	return (length-bytes_len);
}

