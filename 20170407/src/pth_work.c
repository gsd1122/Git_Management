#include "head.h"

void pth_init(pThd p,int num,int capa,pfunc ent)
{
	pQue q=&p->qu;
	que_init(capa,q);
	p->thun=ent;
	p->flag=0;
	p->num=num;
	p->pthid=(pthread_t*)calloc(num,sizeof(pthread_t));
	pthread_cond_init(&p->cond,NULL);
}

void pth_start(pThd p)
{
	int i;
	if(p->flag==0)
	{
		for(i=0;i<p->num;i++)
		{
			pthread_create(p->pthid+i,NULL,p->thun,p);
		}
		p->flag=1;
	}else{
			printf("pthread is working\n");
		}
}
