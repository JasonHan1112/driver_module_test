#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>

int main(int argc,char *argv[])
{
	int fd;
	int ret;
//设备文件节点
	char* helloMiscDevNode = "/dev/hello_misc_device";
	//char* helloMiscDevNode = "/dev/hello_ctl";
//以只读文件打开
	if((fd = open(helloMiscDevNode,O_RDWR | O_NDELAY)) < 0)
	{
		printf("Open devFile: %s failed\n",helloMiscDevNode);
		return -1;
	}
	else
	{
		printf("Open devFile: %s succeed!,fd =%d \n",helloMiscDevNode,fd);
		ret = ioctl(fd,1,1);
		printf("%d\n",ret);
		sleep(3);
		ret = ioctl(fd,0,0);
		printf("%d\n",ret);
		sleep(3);
		ret = ioctl(fd,1,1);
		printf("%d\n",ret);
		sleep(3);
	}
	close(fd);
	return 0;
}
