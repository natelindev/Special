#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>   
#define OK 1
#define ERROR -1
#define OVERFLOW -1

//堆栈功能实现  
typedef struct node  
{  
    char data;  
    struct node* next;  
}StackNode,*LinkStack;

LinkStack initStack(LinkStack top)
{ 
    top = (LinkStack)malloc(sizeof(StackNode));
    top->data='#';
    top->next= NULL;
    return top;
}
 
int StackEmpty(LinkStack top)  
{  
    if(top->data == '#')  
        return OK;  
    else  
        return ERROR;  
}     
  
LinkStack Push(LinkStack top,char x)
{ 
    StackNode *s;
    s=(LinkStack)malloc(sizeof(StackNode));
    s->data=x;
    s->next=top;
    top=s;
    return top;
}
  
LinkStack Pop(LinkStack top,char *x)
{ 
    StackNode *p;
    if (top->data == '#') 
        return NULL;
    else 
    { 
        *x = top->data;
        p = top;
        top = top->next;
        free (p);
        p = NULL;
        return top;
    }
}
//堆栈功能完结  

int main()
{
	LinkStack a;
	char target;
	a = initStack(a);
	a = Push(a,'a');
	a = Push(a,'b');
	a = Push(a,'&');
	a = Push(a,'(');
	a = Push(a,')');
	a = Push(a,'+');
	a = Push(a,'-');
	
	while(StackEmpty(a) == ERROR)
    {
        a = Pop(a,&target);
        printf("%c",target);
    }
	return 0;
}