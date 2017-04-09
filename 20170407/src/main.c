#include "head.h"

void* entry(void* p)
{
	pThd pt=(pThd)p;
	pQue e=&pt->qu;
	pNode pcur;
	while(1)
	{
		pthread_mutex_lock(&e->lock);
		if(e->size==0)
		{
		pthread_cond_wait(&pt->cond,&e->lock);
		}
		que_get(e,&pcur);
		pthread_mutex_unlock(&e->lock);
		request_handle(pcur);
		free(pcur);
		pcur=NULL;
	}	
}
int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int num=1;
	int capa=10;
	Thd p;
	bzero(&p,sizeof(p));
	pth_init(&p,num,capa,entry);
	pth_start(&p);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in s,c;
	bzero(&s,sizeof(s));
	s.sin_family=AF_INET;
	s.sin_port=htons(atoi(argv[2]));
	s.sin_addr.s_addr=inet_addr(argv[1]);
	int ret=bind(sfd,(struct sockaddr*)&s,sizeof(s));
	if(-1==ret)
	{
		perror("bind");
		close(sfd);
		return -1;
	}
	listen(sfd,10);
	bzero(&c,sizeof(c));
	int len=sizeof(c);
	int new_fd;
	char buf[20]={0};
	pQue q=&p.qu;
	int i;
	char record[200]={0};
	char tmp[50]={0};
	while(1)
	{
	time_t t;
	time(&t);
	new_fd=accept(sfd,(struct sockaddr*)&c,&len);
	printf("client ip=%s,port=%d\n",inet_ntoa(c.sin_addr),ntohs(c.sin_port));
//	login(new_fd);
	server_login_use_sql(new_fd);
	memset(tmp,0,sizeof(tmp));
	memset(record,0,sizeof(record));
	strncpy(tmp,ctime(&t),strlen(ctime(&t))-1);
//	sprintf(record,"login=ip:%-16sport:%-8d%-30s",inet_ntoa(c.sin_addr),ntohs(c.sin_port),tmp);
//	syslog(LOG_INFO,"%s",record);
	sql_login_command(inet_ntoa(c.sin_addr),ntohs(c.sin_port),tmp);
	pNode pnew=(pNode)calloc(1,sizeof(Node_t));
	pnew->fd=new_fd;
	pnew->path=(char*)calloc(1,200);
	strcpy(pnew->path,getcwd(NULL,0));
	pthread_mutex_lock(&q->lock);
	que_add(q,pnew);
	pthread_mutex_unlock(&q->lock);
	pthread_cond_signal(&p.cond);
	}
}
