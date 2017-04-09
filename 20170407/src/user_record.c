#include "head.h"

char* sql_gets_passwd(char* username,char* getsrow)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="1122";
	char* database="ftp_project";
	char query[300]={0};
	sprintf(query,"%s%s%s%s%s","select password from login where username","=","'",username,"'");
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("error connecting to database:%s\n",mysql_error(conn));
	}else{
//		printf("connect.......\n");
	}
	int t;
	t=mysql_query(conn,query);
	if(t)
	{
		printf("error,t=%d\n",t);
		strcpy(getsrow,"username isn't exit");
		return getsrow;
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{
				for(t=0;t<mysql_num_fields(res);t++)
				{
				sprintf(getsrow,"%s",row[t]);
				}
//				printf("%s\n",getsrow);
				mysql_free_result(res);
				mysql_close(conn);
				return getsrow;
			}
			mysql_free_result(res);
			mysql_close(conn);
			strcpy(getsrow,"username isn't exit");
			return getsrow;	
		}else{
				mysql_free_result(res);
				mysql_close(conn);
				strcpy(getsrow,"username isn't exit");
				return getsrow;
				
			}
	}
}
void gets_salt(char* salt,char* passwd)
{
	int i,j;
	for(i=0,j=0;passwd[i]&&j!=3;i++)
	{
		if(passwd[i]=='$')
		{
			j++;
		}
	}
	strncpy(salt,passwd,i-1);
}
void server_user_register(int sfd)
{
	//服务器发送salt值
	char *s="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	srand(time(NULL));
	char tmp[5]={0};
	char salt[20]={0};
	char username[10]={0};
	char passwd[200]={0};
	int len=0;
	short flag;
	strcpy(salt,"$6$");
	int sn=strlen(s);
	for(int i=0;i<8;i++)
	{
		sprintf(tmp,"%c",s[rand()%sn]);
		strcat(salt,tmp);
	}
	tdata t;
	bzero(&t,sizeof(t));
	t.len=strlen(salt);
	strcpy(t.buf,salt);
	send_n(sfd,(char*)&t,4+t.len);
	bzero(username,sizeof(username));
	bzero(passwd,sizeof(passwd));
	int ret=recv_n(sfd,(char*)&len,sizeof(len));
	if(ret<=0)
	{
		return;
	}
	recv_n(sfd,username,len);
	bzero(&len,sizeof(len));
	recv_n(sfd,(char*)&len,sizeof(int));
	recv_n(sfd,passwd,len);
	ret=sql_insert(username,passwd);
	if(ret!=0)
	{
		flag=0;
		send(sfd,&flag,sizeof(short),0);
		printf("insert failed\n");
	}else{
		printf("REGUSTER SUCCESS\n");
		flag=1;
		send(sfd,&flag,sizeof(short),0);
	}
}
void server_user_login(int sfd)
{
	//服务器接收验证
	char username[11]={0};
	char passwd[200]={0};
	int len=0;
	char row[200]={0};
	short flag;
	char salt[20]={0};
	tdata t;
	begin:
	bzero(&len,sizeof(len));
	bzero(username,sizeof(username));
	int ret=recv_n(sfd,(char*)&len,sizeof(int));
	if(ret<=0)
	{
		return;
	}
	recv_n(sfd,username,len);
	bzero(salt,sizeof(salt));
	sql_gets_passwd(username,row);
	if(strncmp(row,"username",4)==0)
	{
		flag=0;
		send_n(sfd,(char*)&flag,sizeof(short));
		goto begin;
	}
	flag=1;
	send_n(sfd,(char*)&flag,sizeof(short));
	gets_salt(salt,row);
	bzero(&t,sizeof(t));
	t.len=strlen(salt);
	strcpy(t.buf,salt);
	send_n(sfd,(char*)&t,4+t.len);
	bzero(passwd,sizeof(passwd));
	bzero(&len,sizeof(len));
	recv_n(sfd,(char*)&len,sizeof(int));
	recv_n(sfd,passwd,len);
	ret=sql_select(username,passwd);
	bzero(&flag,sizeof(short));
	if(ret!=0)
	{
		flag=0;
		send(sfd,&flag,sizeof(flag),0);
		goto begin;
	}else{
		flag=1;
		send(sfd,&flag,sizeof(flag),0);
		printf("%s log in\n",username);
	}
}
void server_login_use_sql(int sfd)
{
	short flag=0;
	int len=0;
	recv_n(sfd,(char*)&flag,sizeof(short));
	if(0==flag)
	{
		//register
		server_user_register(sfd);
		server_user_login(sfd);
	}
	if(1==flag){
		server_user_login(sfd);
	}
}

