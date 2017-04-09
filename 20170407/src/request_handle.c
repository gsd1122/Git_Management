#include "head.h"
void mode_file(int mode,char str[])
{
    if(S_ISDIR(mode))   {str[0]='d';}
    else if(S_ISCHR(mode))   {str[0]='c';}
    else if(S_ISBLK(mode))   {str[0]='b';}
	else {str[0]='-';}
    if ((mode & S_IRUSR)){str[1] = 'r';}
	else {str[1]='-';}
    if ((mode & S_IWUSR)){str[2] = 'w';}
	else {str[2]='-';}
    if ((mode & S_IXUSR)){str[3] = 'x';}
	else {str[3]='-';}
    if ((mode & S_IRGRP)){str[4] = 'r';}
	else {str[4]='-';}
    if ((mode & S_IWGRP)){str[5] = 'w';}
	else {str[5]='-';}
    if ((mode & S_IXGRP)){str[6] = 'x';}
	else {str[6]='-';}
    if ((mode & S_IROTH)){str[7] = 'r';}
	else {str[7]='-';}
    if ((mode & S_IWOTH)){str[8] = 'w';}
	else {str[8]='-';}
    if ((mode & S_IXOTH)){str[9] = 'x';}
	else {str[9]='-';}
	str[10]='\0';
}

void ls_c(pNode *p)
{
	DIR *dir;
	tdata t;
	bzero(&t,sizeof(t));
	dir=opendir((*p)->path);
	if(NULL==dir)
	{
		char b[20]="command not found";
		t.len=strlen(b);
		strcpy(t.buf,b);
		send_n((*p)->fd,(char*)&t,4+t.len);
		return;
	}
//	printf("path=%s\n",(*p)->path);
	struct dirent *d;
	struct stat s;
	char *tmp=(char*)calloc(1,20);
	char *buf=(char*)calloc(1,200);
	while((d=readdir(dir))!=NULL)
	{
		stat(d->d_name,&s);
//		mode_file(s.st_mode,tmp);
		sprintf(buf,"%-18s%-10ld",d->d_name,s.st_size);	
		bzero(&t,sizeof(t));
		strcpy(t.buf,buf);
		t.len=strlen(t.buf);
		send_n((*p)->fd,(char*)&t,4+t.len);
	}
	closedir(dir);
}
void pwd_c(pNode *p)
{
	tdata t;
	bzero(&t,sizeof(t));
	strcpy(t.buf,(*p)->path);
	t.len=strlen(t.buf);
	send_n((*p)->fd,(char*)&t,4+t.len);
}
void cd_c(pNode *p,char* buf)
{
	DIR *dir;
	tdata t;
	bzero(&t,sizeof(t));
	char* pf=(char*)calloc(1,100);
	char* sf=(char*)calloc(1,100);
	strcpy(pf,(*p)->path);
	if(strcmp(buf,"..")==0){
		dir=opendir((*p)->path);
		if(NULL==dir)
		{
			char b[20]="command not found";
			t.len=strlen(b);
			strcpy(t.buf,b);
			send_n((*p)->fd,(char*)&t,4+t.len);
			return;
		}
		sf=strrchr(pf,'/');
		int lensf=strlen(sf);
		int lenpf=strlen(pf);
		int len=lenpf-lensf;
		bzero((*p)->path,100);
		strncpy((*p)->path,pf,len);
		dir=opendir((*p)->path);
		if(NULL==dir)
		{
			char b[20]="command not found";
			t.len=strlen(b);
			strcpy(t.buf,b);
			send_n((*p)->fd,(char*)&t,4+t.len);
			return;
		}
	}
	else{
		dir=opendir(buf);
		if(NULL==dir)
		{
			char b[20]="command not found";
			t.len=strlen(b);
			strcpy(t.buf,b);
			send_n((*p)->fd,(char*)&t,4+t.len);
		}else{
			strcpy((*p)->path,buf);
			t.len=strlen(buf);
			strcpy(t.buf,buf);
			send_n((*p)->fd,(char*)&t,4+t.len);
			}
		closedir(dir);
		}
}
void remove_c(pNode *p,char* buf)
{
	DIR *dir;
	tdata t;
	dir=opendir((*p)->path);
	if(NULL==dir)
	{
		char b[20]="command not found";
		t.len=strlen(b);
		strcpy(t.buf,b);
		send_n((*p)->fd,(char*)&t,4+t.len);
		return;
	}
	char tmp[50]={0};
	sprintf(tmp,"%s/%s",(*p)->path,buf);
	if(remove(tmp)!=0)
	{
		char b[]="command not found";
		tdata t;
		bzero(&t,sizeof(t));
		t.len=strlen(b);
		strcpy(t.buf,b);
		send_n((*p)->fd,(char*)&t,4+t.len);
	}
}
void puts_c(pNode* p,char* buf)
{
	server_recv_file(*p);
}
void gets_c(pNode* p,char* buf)
{
	server_send_file(*p,buf);
}
void request_handle(pNode pnew)
{
	psp head=NULL,tail=NULL;
	list_creat(&head,&tail);
	while(1)
	{
//	list_print(head);
	char *buf=(char*)calloc(1,50);
	char tmp[50]={0};
	int len=0;
	int ret=recv(pnew->fd,(char*)&len,sizeof(int),0);
	if(ret<=0)
	{
		printf("client close\n");
		return;
	}
	recv_n(pnew->fd,buf,len);
	printf("command:%s\n",buf);
	char retime[50]={0};
	time_t t;
	time(&t);
//	sprintf(retime,"%s:%s",buf,ctime(&t));
//	syslog(LOG_INFO,"%s",retime);
	sql_command(buf,ctime(&t));
	psp pcur=head;
	while(pcur!=NULL)
	{
	if(strncmp(buf,pcur->bf,2)==0)
	{
	switch(pcur->num)
	{
		case 0:	
				ls_c(&pnew);break;
		case 1:	
				pwd_c(&pnew);break;
		case 2:	
				while(*(buf++)!=' '){}
				strcpy(tmp,buf);
				gets_c(&pnew,tmp);break;
		case 3:	
				while(*(buf++)!=' '){}
				strcpy(tmp,buf);
				puts_c(&pnew,buf);break;
		case 4:
				while(*(buf++)!=' '){}
				strcpy(tmp,buf);
				cd_c(&pnew,tmp);break;
		case 5:	
				while(*(buf++)!=' '){}
				strcpy(tmp,buf);
				remove_c(&pnew,buf);break;
		default:break;
	}
	}
	pcur=pcur->next;
	}
	pcur=head;
	}
}
