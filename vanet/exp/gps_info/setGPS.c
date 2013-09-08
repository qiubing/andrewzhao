#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<termios.h>
#include<fcntl.h>
#include<string.h>

int fd,i;

int set_port(int fd,int speed)
{
	struct termios newtio,oldtio; 
	if(tcgetattr(fd,&oldtio)!=0) //tcgetattr get current termal in oldtio
	{
		perror("setupserial");
		return -1;	
	}
	bzero(&newtio,sizeof(newtio));
	newtio.c_cflag |= CLOCAL|CREAD;
	newtio.c_cflag &=~CSIZE;
	newtio.c_cflag |= CS8;
	newtio.c_cflag &= ~PARENB;
	switch(speed)
	{
	case 9600:
		cfsetispeed(&newtio,B9600);
		cfsetospeed(&newtio,B9600);
		break;
	case 38400:
		cfsetispeed(&newtio,B38400);
		cfsetospeed(&newtio,B38400);
		break;
	case 115200:
		cfsetispeed(&newtio,B115200);
		cfsetospeed(&newtio,B115200);
		break;
	}
	newtio.c_cflag &= ~CSTOPB;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 6;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set");
		return -1;
	}
	return 0;
}

int open_port(int fd)
{
	fd = open("/dev/ttyUSB0",O_RDWR);
	if(-1 == fd)
	{
		perror("can not open serial");
		return -1;
	}
	printf("fd_open = %d\n",fd);
	return fd;
}

void write_port(char *cmd)
{
	int nwrite;
	char *gps_cmd;
	gps_cmd = cmd;
	printf("fd = %d\n",fd);
	nwrite= write(fd,gps_cmd,strlen(gps_cmd));
	printf("nwrite = %d,gpd_cmd = %s\n",nwrite,gps_cmd);
	return ;
}

void read_port(void)
{
	int nread;
	char msg[25];
	nread = read(fd,msg,25);
	printf("nread = %d,msg = %s\n",nread,msg);
	return;
}

int main()
{
	char *cmd[3] = {"$PMTK000*32\r\n","$PMTK251,115200*1F\r\n","$PMTK300,100,0,0,0,0*2C\r\n"};
	int K;
	int key = 0;
	if((fd = open_port(fd))<0)
	{
		perror("open_port error");
		return;
	}
	if((i=set_port(fd,9600))<0)
	{
		perror("set_opt error");
		return;	
	}
	for(K=0; K<3; K++)
	{	
		if(key == 0)
		{		
			write_port(cmd[0]);
			//usleep(50);
			read_port();
			read_port();
			key ++;
			//usleep(5000);
		}
		if(key == 1)
		{
			write_port(cmd[1]);	
			read_port();	
			read_port();	
			read_port();		
			if((i=set_port(fd,115200))<0)
			{
				perror("set_opt error");
				return;	
			}	
			read_port();
			read_port();
			key++;
			//usleep(5000);	
		}
		if(key == 2)
		{
			write_port(cmd[2]);
			usleep(50);
			read_port();
			read_port();
		}
	}	

	close(fd);
	return 0;
}
