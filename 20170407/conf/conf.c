#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	int fd;
	fd=open("config",O_RDONLY);
	char buf[50]={0};
	read(fd,buf,sizeof(buf));
	printf("%s",buf);
	return 0;
}
