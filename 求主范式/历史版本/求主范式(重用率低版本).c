#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <math.h> 
#define OK 1
#define ERROR -1

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

//命题变元数量获取器  
int getNumber(char* exp)  
{  
    int i,j,max = 0;  
    char letters[27] ={"abcdefghijklmnopqrstuvwxyz"};  
    for(i = 0; exp[i] != 0; i++)  
    {  
        for(j = 0; j < 26 ; j++)  
        {  
            if(letters[j] == exp[i])  
                max = j;  
        }  
    }  
    return max+1;  
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
}

//迭代器 1型
void iterator1(char* exp,int *m,int *M)
{
    int i,j;
    char currentNumber[2];
    char *checkList[2]={  
        "0",  
        "1",  
    };
    for(i = 0; i <= 1; i++)
    {
        sprintf(currentNumber,"%d",i);
        for(j = 0; j < 2; j++)
        {
            if(strcmp(currentNumber,checkList[j]) == 0)  
            {  
                if(Operator1(exp,i) == 1)  
                    m[j] = 1;  
                else  
                    M[j] = 1;  
            }
        }  
    }
}  

//迭代器 2型  
void iterator2(char* exp,int *m,int *M)  
{  
    int i,j,k;  
    char currentNumber[3];  
    char *checkList[4]={  
        "00",  
        "01",  
        "10",  
        "11"
    };
    for(i = 0; i <= 1; i++)  
    {  
        for(j = 0; j <= 1; j++)  
        {  
            sprintf(currentNumber,"%d%d",i,j); 
            for(k = 0; k < 4; k++)  
            {  
                if(strcmp(currentNumber,checkList[k]) == 0)  
                {  
                    if(Operator2(exp,i,j) == 1)  
                        m[k] = 1;  
                    else  
                        M[k] = 1;  
                }  
            }  
        }  
    }  
}  

//迭代器 3型  
void iterator3(char* exp,int *m,int *M)  
{  
    int i,j,k,l;  
    char currentNumber[4];  
    char *checkList[8]={  
        "000",  
        "001",  
        "010",  
        "011",
        "100",
        "101",
        "110",
        "111",
    };
    for(i = 0; i <= 1; i++)  
    {  
        for(j = 0; j <= 1; j++)  
        {  
            for(k = 0; k <= 1; k++)  
            {  
                sprintf(currentNumber,"%d%d%d",i,j,k);
                for(l = 0; l < 8; l++)
                {
                    if(strcmp(currentNumber,checkList[l]) == 0)  
                    {  
                        if(Operator3(exp,i,j,k) == 1)  
                            m[l] = 1;  
                        else  
                            M[l] = 1;  
                    }  
                } 
            }  
        }  
    }  
} 

//运算器 1型  
int Operator1(char* exp,int a)  
{  
    LinkStack OperandStack;
    OperandStack = initStack(OperandStack);
    int i,oprand1 = 0,oprand2 = 0;
    char pop_out;
    for(i = 0;exp[i] != 0; i++)
    {
        if(isOperator(exp[i]) == ERROR)//字符的赋值
        {
            switch(exp[i])
            {
                case 'a':
                {
                    OperandStack = Push(OperandStack,a);
                    break;
                }
                default:
                {
                    fprintf(stderr,"OprandFormatExcption: Invalid oprand: %c\n",exp[i]);
                    exit(ERROR);
                }      
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
                    fprintf(stderr,"OperatorFormatExcption: Invalid operator: %c\n",exp[i]);
                    exit(ERROR);
                }
            }
        }
    }
    OperandStack = Pop(OperandStack,&pop_out);//返回最终结果
    return pop_out;
}

//运算器 2型  
int Operator2(char* exp,int a,int b)  
{  
    LinkStack OperandStack;
    OperandStack = initStack(OperandStack);
    int i,oprand1 = 0,oprand2 = 0;
    char pop_out;
    for(i = 0;exp[i] != 0; i++)
    {
        if(isOperator(exp[i]) == ERROR)//字符的赋值
        {
            switch(exp[i])
            {
				case 'a':
                {
                    OperandStack = Push(OperandStack,a);
                    break;
                }
                case 'b':
                {
                    OperandStack = Push(OperandStack,b);
                    break;
                }
                default:
                {
                	fprintf(stderr,"OprandFormatExcption: Invalid oprand: %c\n",exp[i]);
                	exit(ERROR);
                }      
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
                	fprintf(stderr,"OperatorFormatExcption: Invalid operator: %c\n",exp[i]);
                    exit(ERROR);
                }
            }
        }
    }
    OperandStack = Pop(OperandStack,&pop_out);//返回最终结果
    return pop_out;
}    

//运算器 3型  
int Operator3(char* exp,int a,int b,int c)  
{  
    LinkStack OperandStack;
    OperandStack = initStack(OperandStack);
    int i,oprand1 = 0,oprand2 = 0;
    char pop_out;
    for(i = 0;exp[i] != 0; i++)
    {
        if(isOperator(exp[i]) == ERROR)//字符的赋值
        {
            switch(exp[i])
            {
                case 'a':
                {
                    OperandStack = Push(OperandStack,a);
                    break;
                }
                case 'b':
                {
                    OperandStack = Push(OperandStack,b);
                    break;
                }
                case 'c':
                {
                    OperandStack = Push(OperandStack,c);
                    break;
                }
                default:
                {
                    fprintf(stderr,"OprandFormatExcption: Invalid oprand: %c\n",exp[i]);
                    exit(ERROR);
                }      
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
                    fprintf(stderr,"OperatorFormatExcption: Invalid operator: %c\n",exp[i]);
                    exit(ERROR);
                }
            }
        }
    }
    OperandStack = Pop(OperandStack,&pop_out);//返回最终结果
    return pop_out;
}

//有价值信息收集器
void infoCollector(int *m,int *M,char *destm,char *destM)
{
    int i;
    char digit;
    for(i = 0; i < 4; i++)
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
    //转换为后缀表达式 
    ExpConverter(buffer,tobeOperated); 
    //进行二进制加法器运算，记录结果
    switch(numbers)  
    {
        case 1:
        {
            iterator1(tobeOperated,m,M);
            break;
        }
        case 2:
        {
            iterator2(tobeOperated,m,M);
            break;
        }
        case 3:
        {
            iterator3(tobeOperated,m,M);
            break;
        }
        default:
        {
            fprintf(stderr,"NumberFormatException: Invalid number: %d\n",numbers);
            exit(ERROR);
        }
    }
    //输出格式化
    infoCollector(m,M,destm,destM);
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
	free(tobeOperated);
    return 0;  
}  
