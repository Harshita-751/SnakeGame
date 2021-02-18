#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Node{
    int x,y;
    struct Node* next;
} Node;

Node* createNode(){
    Node* newnode=(Node*)malloc(sizeof(Node));
    return newnode;
}

void printList(Node* head)
{
    printf("\n");
    while(head!=NULL)
    {
        printf(" (%d %d) ",head->x,head->y);
        head=head->next;
    }
    printf("\n");
}

void displayBoard(char** board, int m,int n)
{
    for(int i=0;i<=m;i++)
    {
        printf("##");
    }
    for(int i=0;i<m;i++)
    {
        printf("\n#");
        for(int j=0;j<n;j++)
        {
            printf("%c",board[i][j]);
        }
        printf("#");
    }
    printf("\n");
    for(int i=0;i<=m;i++)
    printf("##");
}

int checkOnSnake(Node* head,int x,int y)
{
    while(head!=NULL)
    {
        if(head->x == x && head->y==y)
        {
            return 1;
        }
        head=head->next;
    }
    return 0;
}

void generateFruit(char** board,int m,int n,Node* head)
{
    time_t t;
    srand((unsigned) time(&t));
    int x=0,y=0;
    do{
        x=rand()%m;
        y=rand()%n;
    }while(checkOnSnake(head,x,y));
    board[x][y]='F';
}

void initBoard(char** board,int m,int n)
{
    for(int i=0;i<m;i++)
    {
        *(board+i)=(char*)calloc(n,sizeof(char));
    }
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            board[i][j]=' ';
        }
    }
}

void snake(char** board,Node* head,int length)
{
    head->x=0;
    head->y=0;
    board[0][0]='H';
    for(int i=1;i<length;i++)
    {
        head->next=createNode();
        head=head->next;
        head->x=0;
        head->y=i;
        if(i==length-1)
        board[0][i]='T';
        else
        board[0][i]='-';
    }
}

void removeLast(Node* head,char** board)
{
    while(head->next->next!=NULL)
    {
        head=head->next;
    }
    board[head->next->x][head->next->y]=' ';
    board[head->x][head->y]='T';
    head->next=NULL;
}

void helper(char** board, Node* head,int x1,int y1)
{
    board[head->x][head->y]='-';
    Node* newnode=createNode();
    newnode->x=head->x;
    newnode->y=head->y;
    newnode->next=head->next;
    head->next=newnode;
    head->x=x1;
    head->y=y1;
}

int moveSnake(char** board,int m,int n,char move,Node* head)
{
    int x=0,y=0;
    switch(move)
    {
        case 'w':
            x = head->x == 0 ? m-1 : head->x -1;
            y = head->y;
            break;
        case 'a':
            x=head->x;
            y=head->y==0 ? n-1 : head->y-1;
            break;
        case 's':
            x=(head->x+1)%m;
            y=head->y;
            break;
        case 'd':
            x=head->x;
            y=(head->y+1)%n;
            break;
        default:
            return 0;
    }
    if(x==head->next->x && y==head->next->y) 
    return 0;
    helper(board,head,x,y);
    int score=board[x][y]=='F'?1:0;
    if(!score) removeLast(head,board);
    else generateFruit(board,m,n,head);
    board[x][y]='H';
    if(checkOnSnake(head->next,x,y)) return -1;
    return score;
}

int main()
{
    char restart='n';
        int m=15,n=30,score=0;
        char** board=(char**)calloc(m,sizeof(char*));
        initBoard(board,m,n);
        Node* head=createNode();
        snake(board,head,4);
        generateFruit(board,m,n,head);
        char move='H';
        do{
            printf("\nH is head\nT is tail\n\n");
            displayBoard(board,m,n);
            printf("\n\nINSTRUCTIONS:\n    Press w for Up\n    Press a for Left\n    Press d for Right\n    Press s for Down\n    Press q for Quit\n");
            scanf(" %c",&move);
            system("clear");
            if(move!='q')
            {
                int res=moveSnake(board,m,n,move,head);
                if(res==-1)
                {
                    printf("Game Over! You Ate Yourself!\n\n");
                    break;
                }
                score+=res;
            }
        }while(move!='q');
        if(move=='q')
        printf("\nGame Over! You Quit!\n\n");
        printf("Your score is: %d ",score);
    
    return 0;
}