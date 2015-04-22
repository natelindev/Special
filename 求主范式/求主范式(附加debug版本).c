#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <math.h> 
#define OK 1
#define ERROR -1
#define DEBUG 1 
/*
*	潜在BUG: 不可处理超过4个的命题变元，问题处于infoCollector处 
*
*
*
*
*/
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

//全局名称存储
char names[27] = {"\0"};

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

//命题变元数量获取、名称记录器
//名称已按字典序排列  
int getNumber(char* exp)  
{  
    int i,j,found,length,temp;
    //收集名称
    for(i = 0; exp[i] != 0; i++)
    {  
        if(isOperator(exp[i]) == ERROR)
        {
            found = 0;
            for (j = 0;names[j] != 0; j++)
            {   
                if(exp[i] == names[j])
                {
                    found = 1;
                    break;
                }    
            }
            if(!found)
            {
                strncat(names,&exp[i],1);
            }
        }  
    }
    //名称排序
    length = strlen(names);
    for (i = 0; i < length - 1; i++)
    {
        for (j = 0; j < length - 1 - i; j++)
        {
            if (names[j] > names[j + 1]) 
            {
                temp = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp;
            }
        }
    }
    if(DEBUG == 1)
    	printf("The names are: %s\n",names);
    return length;  
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
                    	fprintf(stderr,"PrecederResultExcption: Invalid result: %c"\
                            ,Preceder(pop_out,Mid[i]));
						exit(ERROR);
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
    if(DEBUG == 1)
    	printf("The converted expression is %s\n",destination);
} 

//控制器
void ControlUnit(char* exp,int number,int pows,int *m,int *M)//本模块有全局依赖
{  
    int i,j,temp;
    int *BinarySet;
    //动态BiSet生成
    BinarySet = (int*)malloc(number*sizeof(int)+1);
    //BiSet初始化
    for(i = 0;i < number;i++) BinarySet[i] = 0;
    for(i = 0;i < pows;i++)
    {
        temp = i;
        for (j = number - 1; j >= 0; j--)
        {
            BinarySet[j] = temp%2;
            temp /= 2;
        }
        if(DEBUG == 1)
        {
            printf("The BinarySet is: ");
            for (temp = 0; temp < number; temp++)
            {
                printf("%d",BinarySet[temp]);
            }
            printf("\n");
        }
        //结果记录
        if(ArithmeticUnit(exp,number,BinarySet) == 1)
        {
        	m[i] = 1;
        	if(DEBUG == 1)
                printf("The result is 1\n");
        }
        else
        {
            M[i] = 1;  
            if(DEBUG == 1)
                printf("The result is 0\n");
        }
    }   
}  

//运算器  
int ArithmeticUnit(char* exp,int length,int* BinarySet)  
{  
    LinkStack OperandStack;
    OperandStack = initStack(OperandStack);
    int i,j,oprand1 = 0,oprand2 = 0;
    char pop_out;
    for(i = 0;exp[i] != 0; i++)
    {
        if(isOperator(exp[i]) == ERROR)//字符的赋值
        {
			for (j = 0;j < length; j++)
            {
                if(exp[i] == names[j])
                {
                    OperandStack = Push(OperandStack,BinarySet[j]);
                    break;
                }
            }
            if(j == length)
            {
            	fprintf(stderr,"OprandNotFoundExcption: Invalid oprand: %c\n",exp[i]);
            	exit(ERROR);
            }          
        }
        else
        {
            switch(exp[i])//计算部分
            {
                //双目运算符
                case '&':
                {
                    OperandStack = Pop(OperandStack,&oprand1);
                    OperandStack = Pop(OperandStack,&oprand2);
                    OperandStack = Push(OperandStack,oprand1 && oprand2);
                    break;
                }
                case '|':
                {
                    OperandStack = Pop(OperandStack,&oprand1);
                    OperandStack = Pop(OperandStack,&oprand2);
                    OperandStack = Push(OperandStack,oprand1 || oprand2);
                    break;
                }
                case '+':
                {
                    OperandStack = Pop(OperandStack,&oprand1);
                    OperandStack = Pop(OperandStack,&oprand2);
                    OperandStack = Push(OperandStack,\
					(!oprand1||oprand2) && (oprand1||!oprand2));
                    break;
                }
                case '-':
                {
                    OperandStack = Pop(OperandStack,&oprand1);
                    OperandStack = Pop(OperandStack,&oprand2);
                    OperandStack = Push(OperandStack,!oprand2||oprand1);
                    break;
                }
                //单目运算符
                case '!':
                {
                    OperandStack = Pop(OperandStack,&oprand1);
                    OperandStack = Push(OperandStack,!oprand1);
                    break;
                }
                default:
                {
                	fprintf(stderr,"OperatorNotFoundExcption: Invalid operator: %c\n",exp[i]);
                    exit(ERROR);
                }
            }
        }
    }
    OperandStack = Pop(OperandStack,&pop_out);//返回最终结果
    return pop_out;
}

//有价值信息收集器
void infoCollector(int *m,int *M,char *destm,char *destM,int pows)//存在潜在bug
{
    int i;
    char digit;
    for(i = 0; i < pows; i++)
    {
        if(m[i] == 1)
        {
        	digit = i+48;
            strncat(destm,&digit,1);
        }
        if(M[i] == 1)
        {
            digit = i+48;
            strncat(destM,&digit,1);
        }
    }
    if(DEBUG == 1)
    {
    	printf("destm is: %s\n",destm);
        printf("destM is: %s\n",destM);
    }
    	
}
  
int main()  
{  
    char buffer[10000];
    int i,mi,mj,Mi,Mj,numbers = 0,pows = 0,length;
    //获取阶段
    gets(buffer);
    length = strlen(buffer); 
    numbers = getNumber(buffer); 
    //初始化阶段 
    pows = (int)pow(2,numbers);
    char *tobeOperated;
    tobeOperated = (char*)malloc(length+1); 
    int *m,*M;
    char *destm,*destM;
    m = (int*)malloc(pows*sizeof(int)+1);
    M = (int*)malloc(pows*sizeof(int)+1);
    destm = (char*)malloc(pows*sizeof(char)+1);
    destM = (char*)malloc(pows*sizeof(char)+1);
    for(i = 0 ; i< length ; i++) tobeOperated[i]='\0'; 
	for(i = 0 ; i< numbers ; i++) destm[i]='\0'; 
	for(i = 0 ; i< numbers ; i++) destM[i]='\0'; 
    //转换为后缀表达式 
    ExpConverter(buffer,tobeOperated); 
    //进行二进制加法器运算，记录结果
    ControlUnit(tobeOperated,numbers,pows,m,M);

    //输出格式化
    infoCollector(m,M,destm,destM,pows);
    for(mi = 0;destm[mi] != 0; mi++);
    for(Mi = 0;destM[Mi] != 0; Mi++);

    for(mj = 0;destm[mj] != 0; mj++)//主析取范式输出
    {
        if(mj+1 < mi)
            printf("m%c V ",destm[mj]);
        else
            printf("m%c ; ",destm[mj]);
    }
    if(mi == 0)//永假式处理
    {
        printf("0 ; ");
    }
    for(Mj = 0;destM[Mj] != 0; Mj++)//主合取范式输出
    {
        if(Mj+1 < Mi)
            printf("M%c X ",destM[Mj]);
        else
            printf("M%c\n",destM[Mj]);
    }
    if(Mi == 0)//永真式处理
    {
        printf("1\n");
    }
    //尾处理
	free(tobeOperated);free(m);free(M);free(destm);free(destM);
    return 0;  
}  
