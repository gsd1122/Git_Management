#include "head.h"

void input_handle(int sfd,char* buf)
{
	psp head=NULL,tail=NULL;
	tdata t;
	bzero(&t,sizeof(t));
	list_creat(&head,&tail);
//	list_print(head);
	t.len=strlen(buf)-1;
	char *c=(char*)calloc(1,10);
	char tmp[20]={0};
	short flag;
	strncpy(t.buf,buf,strlen(buf)-1);
	while(head!=NULL)
	{
		if(strncmp(buf,head->bf,2)==0)
		{
			switch(head->num){
					case 0:send_n(sfd,(char*)&t,4+t.len);break;
					case 1:send_n(sfd,(char*)&t,4+t.len);break;
					case 2:send_n(sfd,(char*)&t,4+t.len);
						   strcpy(c,t.buf);
						   while(*(c++)!=' '){}
						   strcpy(tmp,c);	
						   client_recv_file(sfd,tmp);	
						   break;
					case 3:send_n(sfd,(char*)&t,4+t.len);
						   strcpy(c,t.buf);
						   while(*(c++)!=' '){}
						   strcpy(tmp,c);	
						   client_send_file(sfd,tmp);
						   break;
					case 4:send_n(sfd,(char*)&t,4+t.len);break;
					case 5:send_n(sfd,(char*)&t,4+t.len);break;
			}
		}
		head=head->next;
	}	
}
