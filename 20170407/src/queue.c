#include "head.h"

void que_init(int capa,pQue p)
{
	p->capacity=capa;
	pthread_mutex_init(&p->lock,NULL);
}

void que_add(pQue p,pNode pnew)
{
	if(p->head==NULL)
	{
		p->head=pnew;
		p->tail=pnew;
	}else{
		p->tail->next=pnew;
		p->tail=pnew;
	}
	p->size++;
}
void que_get(pQue p,pNode *pcur)
{
	*pcur=p->head;
	p->head=p->head->next;
	if(p->head==NULL)
	{
		p->tail=NULL;
	}
	p->size--;
}
