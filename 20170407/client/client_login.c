#include "head.h"
void client_register_interface(int sfd)
{
	char username[11]={0};
	char first_passwd[21]={0};
	char final_passwd[100]={0};
	char salt[20]={0};
	int len=0;
	short flag;
	tdata t;
	begin:	
	printf("Input Username:\n");
	bzero(username,sizeof(username));
	bzero(first_passwd,sizeof(first_passwd));
	read(0,username,sizeof(username));
	strcpy(first_passwd,getpass("Input Password:\n"));
	if(strlen(username)>10|| strlen(username)<1)
	{
		printf("Username's length is between 1 and 10\n");	
		goto begin;
	}
	if(strlen(first_passwd)>20||strlen(first_passwd)<1)
	{
		printf("Password's length is between 6 and 20\n");
		goto begin;
	}
	bzero(&len,sizeof(len));
	bzero(salt,sizeof(salt));
	recv_n(sfd,(char*)&len,sizeof(int));
	recv_n(sfd,salt,len);
	bzero(final_passwd,sizeof(final_passwd));
	strcpy(final_passwd,crypt(first_passwd,salt));
	bzero(&t,sizeof(t));	
	t.len=sizeof(username);
	strcpy(t.buf,username);
	send_n(sfd,(char*)&t,4+t.len);
	bzero(&t,sizeof(t));
	t.len=sizeof(final_passwd);
	strcpy(t.buf,final_passwd);
	send_n(sfd,(char*)&t,4+t.len);
	recv_n(sfd,(char*)&flag,sizeof(short));
	if(flag==1)
	{
	printf("Register Success\n");
	}else{
	printf("Register Failed\n");
	}
}
void client_landing_interface(int sfd)
{
	
	char username[11]={0};
	char first_passwd[21]={0};
	char final_passwd[200]={0};
	char salt[20]={0};
	int len=0;
	char tmp[11]={0};
	short flag=0;
	tdata t;
	begin:	
	printf("Input Username:\n");
	bzero(tmp,sizeof(tmp));
	bzero(first_passwd,sizeof(first_passwd));
	read(0,tmp,sizeof(tmp));
	strcpy(first_passwd,getpass("Input Password:\n"));
	system("clear");
	if(strlen(tmp)>10)
	{
		printf("Username's length is between 1 and 10\n");	
		goto begin;
	}
	if(strlen(first_passwd)>20||strlen(first_passwd)<1)
	{
		printf("Password's length is between 1 and 20\n");
		goto begin;
	}
	bzero(&t,sizeof(t));
	bzero(username,sizeof(username));	
	strncpy(username,tmp,strlen(tmp)-1);
	t.len=strlen(username);
	strcpy(t.buf,username);
	send_n(sfd,(char*)&t,4+t.len);
	recv_n(sfd,(char*)&flag,sizeof(short));
	if(0==flag)
	{
		printf("Error Username\n");
		goto begin;
	}
	bzero(&len,sizeof(len));
	bzero(salt,sizeof(salt));
	recv_n(sfd,(char*)&len,sizeof(int));
	recv_n(sfd,salt,len);
	bzero(final_passwd,sizeof(final_passwd));
	strcpy(final_passwd,crypt(first_passwd,salt));
	bzero(&t,sizeof(t));
	t.len=sizeof(final_passwd);
	strcpy(t.buf,final_passwd);
	send_n(sfd,(char*)&t,4+t.len);
	bzero(&flag,sizeof(flag));
	recv_n(sfd,(char*)&flag,sizeof(flag));
	if(flag==0)
	{
		printf("Password Mistake\n");
		goto begin;
	}else{
		printf("WELCOME\n");
	}
}
void client_login(int sfd)
{
	short flag;
	begin:
//	system("clear");
	printf("\n\n1.用户注册\n");
	printf("\n2.用户登陆\n\n");
	char buf[10]={0};
	bzero(buf,sizeof(buf));
	int ret=read(0,buf,sizeof(buf));
	if(ret<=0)
	{
		printf("输入有误\n");
		goto begin;
	}
	if(buf[0]=='1')
	{
		flag=0;
		send_n(sfd,(char*)&flag,sizeof(short));
		system("clear");
		client_register_interface(sfd);
		printf("login\n\n");
		client_landing_interface(sfd);
	}else if(buf[0]=='2'){
//		system("clear");
		flag=1;
		system("clear");
		send_n(sfd,(char*)&flag,sizeof(short));
		printf("login\n\n");
		client_landing_interface(sfd);
	}else{printf("error input\n");goto begin;}
}
