#include "head.h"
void list_creat(psp *head,psp *tail)
{
	sp s[10];
	bzero(&s,sizeof(s));
	int fd;
	fd=open("../conf/command.txt",O_RDONLY);
	if(-1==fd)
	{
		perror("openok");
		return;
	}
	char buf[500]={0};
	char tmp[500]={0};
	read(fd,buf,500);
	int j,i=0,k=0;
	int len=strlen(buf);
	for(j=0;j<len;j++)
	{	
		i=0;
		bzero(tmp,sizeof(500));
		while(buf[j]!='\n')
		{
			tmp[i++]=buf[j];
			j++;
		}
		strcpy(s[k].bf,tmp);
		list_inerst(head,tail,s[k].bf,k);
		k++;
	}
		close(fd);
}
 void list_inerst(psp *head,psp *tail,char* buf,int count)
{
   psp pnew=(psp)calloc(1,sizeof(sp));
   strcpy(pnew->bf,buf);
   pnew->num=count;
   if(NULL==*head)
   {
       *head=pnew;
       *tail=pnew;
   }else{
       (*tail)->next=pnew;
       *tail=pnew;
   }
} 
void list_print(psp head)
{
    while(head!=NULL)
    {
        printf("s.bf=%s,s.count=%d\n",head->bf,head->num);
        head=head->next;
    }
}


  
  
  
  
  
  
  
  
  
  
