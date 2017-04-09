#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <assert.h>
#include <sys/mman.h>
#include <shadow.h>
#include <errno.h>
#include <crypt.h>
#include <time.h>
#include <syslog.h>
#include <mysql/mysql.h>

typedef void* (*pfunc)(void*);
typedef struct node
{
	int fd;
	char *path;
	struct node* next;
}Node_t,*pNode;
typedef struct
{
	int size;
	int capacity;
	pthread_mutex_t lock;
	pNode tail,head;
}Que,*pQue;
typedef struct
{
	int num;
	pthread_cond_t cond;
	Que qu;
	pthread_t *pthid;
	pfunc thun;
	int flag;
}Thd,*pThd;
typedef struct spq
{
	int num;
	char bf[10];
	struct spq* next;
}sp,*psp;
typedef struct
{
	int len;
	char buf[1000];
}tdata,*pdata;
void pth_init(pThd,int,int,pfunc);
void pth_start(pThd);
void que_init(int,pQue);
void que_add(pQue,pNode);
void que_get(pQue,pNode*);
void request_handle(pNode);
int send_n(int,char*,int);
int recv_n(int,char*,int);
void server_send_file(pNode,char*);
void server_recv_file(pNode);
void client_send_file(int,char*);
void client_recv_file(int,char*);
void input_handle(int,char*);
void list_creat(psp*,psp*);
void list_inerst(psp*,psp*,char*,int);
void list_print(psp);
int stat_file(char*);
void login(int);
void server_user_register(int);
void server_user_login(int);
void server_login_use_sql(int);
void client_login(int);
int sql_select(char*,char*);
int sql_insert(char*,char*);
void sql_login_command(char*,int,char*);
void sql_command(char*,char*);
