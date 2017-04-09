#include "head.h"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in s;
	bzero(&s,sizeof(s));
	s.sin_family=AF_INET;
	s.sin_port=htons(atoi(argv[2]));
	s.sin_addr.s_addr=inet_addr(argv[1]);
	int ret=connect(sfd,(struct sockaddr*)&s,sizeof(s));
	if(-1==ret)
	{
		perror("connect");
		close(sfd);
		return -1;
	}
	system("clear");
	printf("connect success\n");
//	login(sfd);
	client_login(sfd);
	int efd=epoll_create(1);
	struct epoll_event en,ev[2];
	bzero(&en,sizeof(en));
	en.events=EPOLLIN;
	en.data.fd=STDIN_FILENO;
	ret=epoll_ctl(efd,EPOLL_CTL_ADD,0,&en);
	if(-1==ret)
	{
		perror("epoll_ctl");
		close(sfd);
		close(efd);
		return -1;
	}
	bzero(&en,sizeof(en));
	en.events=EPOLLIN;
	en.data.fd=sfd;
	ret=epoll_ctl(efd,EPOLL_CTL_ADD,sfd,&en);
	if(-1==ret)
	{
		perror("epoll_ctl sfd");
		close(sfd);
		close(efd);
		return -1;
	}
	int len;
	int i;
	int ret1;
	char buf[500]={0};
	while(1)
	{
		bzero(ev,sizeof(ev));
		ret=epoll_wait(efd,ev,2,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(ev[i].data.fd==0)
				{
					system("clear");
					bzero(buf,100);
					ret1=read(0,buf,100);
					if(ret1>0)
					{
						if(strncmp(buf,"end",strlen(buf)-1)==0)
						{
							exit(0);
						}
						input_handle(sfd,buf);
					}
				}
				if(ev[i].data.fd==sfd)
				{
					bzero(buf,sizeof(buf));
					recv_n(sfd,(char*)&len,sizeof(int));
					recv_n(sfd,buf,len);
					printf("%s\n",buf);
				}
			}
		}
	}
	close(sfd);
	close(efd);
	return 0;
}
