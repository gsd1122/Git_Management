#include "head.h"

void server_send_file(pNode p ,char* buf)
{
   int fd;
   short flag;
   long cli_file_size;
   long ser_file_size;
   char nm[50]={0};
   sprintf(nm,"%s/%s",p->path,buf);
   fd=open(nm,O_RDWR);
   if(-1==fd)
   {
      flag=0;
//    perror("opensend");
      send(p->fd,&flag,sizeof(short),0);
      return;
   }else{
	flag=1;
	send(p->fd,&flag,sizeof(short),0);
	recv(p->fd,&cli_file_size,sizeof(long),0);
	tdata t;
	bzero(&t,sizeof(t));
	if(0==cli_file_size)
	{
	t.len=strlen(buf);
	strcpy(t.buf,buf);
	send_n(p->fd,(char*)&t,4+t.len);
	while((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	   {
		int ret=send_n(p->fd,(char*)&t,4+t.len);
		if(-1==ret){goto end;}
	   }  
	t.len=0;
	send_n(p->fd,(char*)&t.len,4);
	close(fd);
	return;
	}else{
	struct stat ser;
	long pos;
	stat(nm,&ser);
	ser_file_size=ser.st_size;
	pos=ser_file_size-cli_file_size;
	if(pos<=0)
	  {
	    flag=2;
	    send(p->fd,&flag,sizeof(short),0);
	    return;
	  }else{
	    flag=1;
	    send(p->fd,&flag,sizeof(short),0);
	    char* psen=mmap(0,ser_file_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	    if(-1==*psen)
	    {
	    	perror("mmap");
	    }
	    bzero(&t,sizeof(t));
	    char* dsen=psen+cli_file_size-1;
	    strncpy(t.buf,dsen,1000);
	    t.len=strlen(t.buf);
	    send_n(p->fd,(char*)&t,4+t.len);
	    while(1)
	    {
	    if(t.len<1000)
	    {
	    t.len=0;
	    send_n(p->fd,(char*)&t.len,4);
	    close(fd);
	    return;
	    }else{
	    strncpy(t.buf,dsen,1000);
	    t.len=strlen(t.buf);
	    send_n(p->fd,(char*)&t,4+t.len);
	    dsen=dsen+1000;
	    }
	    }
	    }
	}
}
end:
	close(fd);
}
void server_recv_file(pNode p)
{
	char buf[1000]={0};
	int len=0;
	long flag;
	recv(p->fd,&flag,sizeof(long),0);
	if(flag<=0)
	{
		return;
	}
	bzero(buf,sizeof(buf));
	recv_n(p->fd,(char*)&len,4);
	recv_n(p->fd,buf,len);
	char tmp[20]={0};
	strcpy(tmp,buf);
	char nm[50]={0};
	sprintf(nm,"%s/%s",p->path,buf);
	int fd=open(nm,O_CREAT|O_RDWR,0600);
	if(-1==fd)
	{
		perror("openrecv");
		return;
	}
	while(1)
	{
		recv_n(p->fd,(char*)&len,4);
		if(len>0)
		{
			bzero(buf,sizeof(buf));
			recv_n(p->fd,buf,len);
			write(fd,buf,len);
		}else{
			break;
			}
	}
		printf("download %s success\n",tmp);
		close(fd);
}
