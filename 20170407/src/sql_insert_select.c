#include "head.h"

int sql_insert(char* usm,char* pwd)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char username[10]={0};
	char tmp[10]={0};
	char passwd[200]={0};
	strncpy(tmp,usm,strlen(usm)-1);
	sprintf(username,"%s%s%s","'",tmp,"'");
	sprintf(passwd,"%s%s%s","'",pwd,"'");
	char query[300]={0};
	char* server="localhost";
	char* user="root";
	char* password="1122";
	char* database="ftp_project";
	sprintf(query,"%s%s%s%s%s%s","insert into login(username,password)values","(",username,",",passwd,")");
	int t,r;
	conn=mysql_init(NULL);
	
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
     {
         printf("Error connecting to database:%s\n",mysql_error(conn));
     }else{
//         printf("Connected...\n");
     }
     t=mysql_query(conn,query);
     if(t)
     {
         printf("Error making query,insert:%s\n",mysql_error(conn));
 	     mysql_close(conn);
 	     return -1;
     }else{
	        printf("insert successsql\n");
   		    mysql_close(conn);
   		    return 0;
     }
}
int sql_select(char* username,char* passwd)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="1122";
	char* database="ftp_project";
	char query[300]={0};
	bzero(query,sizeof(query));
//	char* query="select from login where username=username and password=passwd";
	sprintf(query,"%s%s%s%s%s%s%s%s","select * from login where username=","'",username,"' ","and password=","'",passwd,"'");
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
     {
         printf("Error connecting to database:%s\n",mysql_error(conn));
     }else{
  //       printf("Connected...\n");
     }
     t=mysql_query(conn,query);
     if(t)
     {
		printf("Error making query:%s\n",mysql_error(conn));
		mysql_close(conn);
		return -1;
	}
	 else{
		res=mysql_use_result(conn);
		if(res)
		{
			if(mysql_fetch_row(res)==NULL)
			{
			mysql_free_result(res);
  			mysql_close(conn);
  			return -1;
			}
			mysql_free_result(res);
			mysql_close(conn);
			return 0;
		}else{
			mysql_free_result(res);
			mysql_close(conn);
			return -1;
			}
		}
}
