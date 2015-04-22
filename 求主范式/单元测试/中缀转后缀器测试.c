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

//符号检测器
int isOperator(char c) 
{
    char OP[] = "+-|&!()";
    int i;
    for(i = 0;i< 7;i++) if(c == OP[i]) return i;
    return ERROR;
}

//优先级判断器  
char Preceder(char OPTR1,char OPTR2)  
{  
    int i,j;  
    char OP[] = "+-|&!";  
    char tab[6][6]={  
        "><<<<",  
        ">><<<",  
        ">>><<",  
        ">>>><",  
        ">>>>>"     
    };  
    for(i = 0; i < 5; i++) if(OP[i] == OPTR1) break;  
    for(j = 0; j < 5; j++) if(OP[j] == OPTR2) break;  
    return tab[j][i];  
}  
 
//中缀转后缀转换器  
void ExpConverter(char* Mid,char* destination)  
{  
    int i;
    char pop_out;
    LinkStack OperatorStack;
    OperatorStack = initStack(OperatorStack);  
    for(i = 0; Mid[i] != 0; i++)//中缀处理开始
    {
        if(isOperator(Mid[i]) == ERROR)//字符直接输出
            strncat(destination,&Mid[i],1);
        else
        {
            if(StackEmpty(OperatorStack) == OK)//为空直接压入
                OperatorStack = Push(OperatorStack,Mid[i]);
            else//不为空则进行判断
            {
                char pop_out;
                OperatorStack = Pop(OperatorStack,&pop_out);//先pop出一个（肯定不会没有的）
                if(pop_out == '(' ||Mid[i] == '(')//左括号特殊处理
                {
                    OperatorStack = Push(OperatorStack,pop_out);
                    OperatorStack = Push(OperatorStack,Mid[i]);
                    continue;
                }
                if(Mid[i] == ')')//右括号特殊处理
                {
                    while(pop_out != '(')//到左括号为止
                    {
                        strncat(destination,&pop_out,1);
                        OperatorStack = Pop(OperatorStack,&pop_out);
                    }
                    continue;
                }
                switch( Preceder(pop_out,Mid[i]) )
                {
                    case '>':
                    {
                        OperatorStack = Push(OperatorStack,pop_out);
                        OperatorStack = Push(OperatorStack,Mid[i]);
                        break;
                    }
                    case '<':
                    {
                        OperatorStack = Push(OperatorStack,Mid[i]);
                        strncat(destination,&pop_out,1);
                        break;
                    }
                    default:
                    {
                        fprintf(stderr,"ExpConverter overflow!");
                        exit(OVERFLOW);
                    }
                }

            }
        }
    }
    while(StackEmpty(OperatorStack) == ERROR)
    {
        OperatorStack = Pop(OperatorStack,&pop_out);
        strncat(destination,&pop_out,1);
    }
}
  
int main()  
{  
    char buffer[81];  
    char *expression,*tobeOperated;    
    int i,length = 1;
    while(length != 0)
    {
        printf("Expression: ");
        scanf("%s",buffer);//获取阶段
        length = strlen(buffer);  
        expression = (char*)malloc(length + 2);  
        tobeOperated = (char*)malloc(length * 2);
        for(i = 0 ; i< length*2-1 ; i++) tobeOperated[i]='\0'; 
        strcpy(expression,buffer);  
        ExpConverter(expression,tobeOperated);//转换为后缀表达式  
        printf("Converted: %s\n", tobeOperated);
        //尾处理
        free(expression);free(tobeOperated);
    }
    return 0;  
}  
