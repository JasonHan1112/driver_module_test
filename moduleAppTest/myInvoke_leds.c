#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

main(){
	int fd;
	char *hello_node = "/dev/myDCdev0";
	char write_buff[128];
	char read_buff[128];
	int i;
	for(i=0;i<128;i++)
	{
		write_buff[i] = i;
		read_buff[i] = 0;
	}
	fd = open(hello_node,O_RDWR|O_NDELAY);
	if(fd < 0)
	{
		printf("APP open %s failed",hello_node);
	}
	else{
		printf("APP open %s success",hello_node);
		ioctl(fd,1,1);
		write(fd,write_buff,128);
		read(fd,read_buff,128);
		for(i=0;i<128;i++)
		{
			printf("read_buff[%d] = %d\n",i,read_buff[i]);
		}
		sleep(3);
		ioctl(fd,0,1);
		write(fd,write_buff,128);
		read(fd,read_buff,128);
		for(i=0;i<128;i++)
		{
			printf("read_buff[%d] = %d\n",i,read_buff[i]);
		}
		sleep(3);
		ioctl(fd,1,1);
		write(fd,write_buff,128);
		read(fd,read_buff,128);
		for(i=0;i<128;i++)
		{
			printf("read_buff[%d] = %d\n",i,read_buff[i]);
		}
		close(fd);
	}
}
