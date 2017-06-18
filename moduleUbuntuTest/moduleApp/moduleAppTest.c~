#include<stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
/*包含errno所需要的头文件*/  
#include <string.h>
#include <errno.h>
/*包含sleep*/
#include <unistd.h>

#define DEBUG(fmt,arg...)  printf(":Function %s, Line %d:[DEBUG]" fmt"\n",__FUNCTION__,__LINE__,##arg)

#define    DEV_NODE    "/dev/HELLO_MODULE0"

#define    MAIN_COUNT  5
int main(void)
{
	char strReadBuff[10];
	char strWriteBuff[10];
	int iRetRead = 0;
	int iRetWrite = 0;
	int iRetIoctl = 0;
	int iForCount = 0;
	int iMainCount = 0;
	int fd;
	char *hello_node = "/dev/HELLO_MODULE0";
	for(iMainCount=0; iMainCount<MAIN_COUNT; iMainCount++)
	{
		fd = open(hello_node,O_RDWR|O_NDELAY);
		if(fd<0)
		{
			/*打印错误信息，新建设备如果权限不修改会出现permission denied的提示*/ 
			DEBUG("APP open %s failed, Message : %s",hello_node,strerror(errno));
			return -1;
		}
		DEBUG("fd = %d\n",fd);
#if 1	
		for(iForCount=0; iForCount<sizeof(strWriteBuff); iForCount++)
		{
			strWriteBuff[iForCount] = iForCount;	
		}
		DEBUG("strWriteBuff = %lu\n",strWriteBuff);
		iRetWrite = write(fd,strWriteBuff,sizeof(strWriteBuff));
		DEBUG("iRetWrite = %d",iRetWrite);
#endif	
#if 0
		memset(strReadBuff,0,sizeof(strReadBuff));
		iRetRead = read(fd,strReadBuff,sizeof(strReadBuff));
		DEBUG("iRetRead = %d",iRetRead);
#endif	
		iRetIoctl = ioctl(fd,1,2);
		DEBUG("iRetIoctl = %d",iRetIoctl);
		sleep(2);
		close(fd);
		
	}
	
	
	return 0;
}
