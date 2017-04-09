#include <mysql/mysql.h>
#include <stdio.h>

int main()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="luke";
	char* database="zhongwen";
	char* query="update test2 set num=30 where name='中文'";
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}
