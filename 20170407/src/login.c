#include "head.h"

void get_salt(char* salt,char* passwd)
{
	int i,j;
	for(i=0,j=0;passwd[i]&&j!=3;i++)
	{
		if(passwd[i]=='$')
		j++;
	}
	strncpy(salt,passwd,i-1);
}
void login(int sfd)
{
	char* username=(char*)calloc(1,10);
	char* passwd=(char*)calloc(1,100);
	char* salt=(char*)calloc(1,30);
	char tmp[10]={0};
	short check;
	struct spwd *p;
	int ret;
	int len;
	tdata t;
	begin:
	bzero(&t,sizeof(t));
	ret=recv(sfd,&len,sizeof(int),0);
	if(ret<=0)
	{
		return;
	}
	bzero(tmp,sizeof(tmp));
	bzero(username,10);
	recv_n(sfd,tmp,len);
	strncpy(username,tmp,strlen(tmp)-1);	
	p=getspnam(username);
	if(p!=NULL)
	{
		check=3;
		send(sfd,&check,sizeof(short),0);
		bzero(salt,30);
		bzero(&t,sizeof(t));
		get_salt(salt,p->sp_pwdp);
		t.len=strlen(salt);
		strcpy(t.buf,salt);
		send_n(sfd,(char*)&t,4+t.len);
		bzero(passwd,100);
		bzero(&len,sizeof(len));
		recv(sfd,&len,sizeof(int),0);
		recv_n(sfd,passwd,len);
		if((strcmp(p->sp_pwdp,passwd))==0)
		{
			check=1;
			send(sfd,&check,sizeof(short),0);
			printf("%s log in\n",username);
			return;
		}else{
			check=0;
			send(sfd,&check,sizeof(check),0);
			goto begin;
		}
	}else{
		check=2;
		send(sfd,&check,sizeof(short),0);
//		perror(username);
		goto begin;
	}
}
