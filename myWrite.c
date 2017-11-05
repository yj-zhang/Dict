#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
int main()
{
	printf("执行成功\n");
	return 0;
}
int myWrite(int fd, void *buffer, int length)
{
	int bytes_len;
	int writ_bytes;
	char *ptr;
	ptr = buffer;
	bytes_len = length;
	while(bytes_len > 0)
	{
		writ_bytes = write(fd, ptr, bytes_len);
		if(writ_bytes <= 0)
		{
			if(errno == EINTR)
			   	writ_bytes = 0;
			else
				return -1;
		}
		bytes_len -= writ_bytes;
		ptr += writ_bytes;
	}
	return 0;
}
