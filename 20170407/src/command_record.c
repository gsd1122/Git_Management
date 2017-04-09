#include "head.h"

void sql_command(char* command,char* thour)
{
	{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="1122";
	char* database="ftp_project";
	char query[100]={0};
	sprintf(query,"%s%s%s%s%s%s%s","insert into command_handle(command,time)values(","'",command,"',","'",thour,"')");
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
//		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
//		printf("insert success\n");
	}
	mysql_close(conn);
	return;
}
}
void sql_login_command(char* ip,int port,char* thour)
{
	{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="1122";
	char* database="ftp_project";
	char query[100]={0};
	sprintf(query,"%s%s%s%s%s%d%s%s%s%s","insert into computer_land_time(Ip,Port,Time)values(","'",ip,"',","'",port,"',","'",thour,"')");
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
//		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
//		printf("insert success\n");
	}
	mysql_close(conn);
	return;
}
}
