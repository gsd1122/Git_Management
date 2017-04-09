#include "head.h"

void login(int sfd)
{
	//loading
	char username[11]={0};
	char* first_passwd=(char*)calloc(1,21);
	char* final_passwd=(char*)calloc(1,100);
	char* salt=(char*)calloc(1,30);
	short check;
	int len;
	tdata t;
	begin:
	printf("Input Username:\n");
	bzero(username,sizeof(username));
	bzero(first_passwd,sizeof(final_passwd));
	read(0,username,11);
	first_passwd=getpass("Input Password:\n");
	if(strlen(username)>10)
	{
		system("clear");
		printf("Username's length too long\n");
		goto begin; 
	}
	if(strlen(first_passwd)>20||strlen(first_passwd)<0)
	{
		system("clear");
		printf("Password's length is between 1 and 20\n");
		goto begin;
	}
	bzero(&t,sizeof(t));
	t.len=strlen(username);
	strcpy(t.buf,username);
	send_n(sfd,(char*)&t,4+t.len);
	int ret=recv(sfd,&check,sizeof(short),0);
	if(ret<=0)
	{
		printf("server close1\n");
		return;
	}
	if(2==check)
	{
		system("clear");
		printf("User Doesn't Exit\n");
		goto begin;
	}else{
	ret=recv(sfd,&len,sizeof(int),0);
	if(ret<=0)
	{
		printf("server close2\n");
		return;
	}
	bzero(salt,30);
	ret=recv_n(sfd,salt,len);
	if(ret<=0)
	{
		printf("server close3\n");
		return;
	}
	bzero(final_passwd,sizeof(final_passwd));
	bzero(&t,sizeof(t));
	final_passwd=crypt(first_passwd,salt);
	t.len=strlen(final_passwd);
	strcpy(t.buf,final_passwd);
	send_n(sfd,(char*)&t,4+t.len);
	recv(sfd,&check,sizeof(short),0);
	if(0==check)
	{
		system("clear");
		printf("Password Mistake\n");
		goto begin;
	}else{
		system("clear");
		printf("Log in Successfully\n");
		return;
		}
	}
}
