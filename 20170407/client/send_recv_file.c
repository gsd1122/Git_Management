#include "head.h"

void client_send_file(int sfd,char* buf)
{
	int fd;
	long flag;
	fd=open(buf,O_RDWR);
	if(-1==fd)
	{
		flag=0;
		perror("open");
		send(sfd,&flag,sizeof(long),0);
		return;
	}
	flag=1;
	send(sfd,&flag,sizeof(long),0);
	tdata t;
	bzero(&t,sizeof(t));	
	t.len=strlen(buf);
	strcpy(t.buf,buf);
	send_n(sfd,(char*)&t,4+t.len);
	while((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	{
		int ret=send_n(sfd,(char*)&t,4+t.len);
		if(-1==ret)
		{
			goto end;
		}
	}
	t.len=0;
	send_n(sfd,(char*)&t.len,4);
end:
	printf("send success\n");
	close(fd);
}
void client_recv_file(int sfd,char* filename)
{
	char buf[1000]={0};
	int len=0;
	short flag;
	long file_size=stat_file(filename);
	recv(sfd,&flag,sizeof(short),0); //服务器是否存在该文件
 	if(0==flag){
	printf("File Not Exist\n");
	return;
	}else{
		send(sfd,&file_size,sizeof(long),0);
		if(0==file_size)
		{
		bzero(buf,sizeof(buf));
		recv(sfd,&len,4,0);
		recv(sfd,buf,len,0);
		int fd=open(buf,O_CREAT|O_RDWR,0600);
		if(-1==fd)
		{
			perror("recvopen");
			return;
		}
		printf("please wait download...\n");
		while(1)
		{
			recv_n(sfd,(char*)&len,4);
			if(len>0)
			{
				bzero(buf,sizeof(buf));
				recv_n(sfd,buf,len);
				write(fd,buf,len);
			}else{
				close(fd);
				printf("download success\n");
				return;
				}
		}
		}else{
			recv(sfd,&flag,sizeof(short),0);
			if(2==flag)
			{
				printf("File is exist\n");
				return;
			}else{
				int fd=open(filename,O_RDWR);
				lseek(fd,-1,SEEK_END);
				while(1)
				{
					recv(sfd,&len,sizeof(int),0);
					if(len>0)
					{
						bzero(buf,sizeof(buf));
						recv_n(sfd,buf,len);
						write(fd,buf,len);
					}else{printf("download success\n");close(fd);return;}
				}	
			}	
		}
	}
}
