#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING      (10)
#define NUM             (10)
static char array[3][NUM];
static char index[3];
static int  step = 0;

void hanuota(int n,char* from,char* to,char* bk){
    if(n == 1){
        step ++;
        printf("[%3d]%s --%02d--> %s\n",step,from,n-1,to);
    }else{
        hanuota(n-1,from,bk,to);
        step ++;
        printf("[%3d]%s --%02d--> %s\n",step,from,n-1,to);
        hanuota(n-1,bk,to,from);
    }
}
void show_progress(void){
    int i = 0;
    printf("\tA[] = ");
    for(i=0;i<index[0];i++){
        printf("%02d ",array[0][i]-1);
    }
    printf("\n");


    printf("\tB[] = ");
    for(i=0;i<index[1];i++){
        printf("%02d ",array[1][i]-1);
    }
    printf("\n");


    printf("\tC[] = ");
    for(i=0;i<index[2];i++){
        printf("%02d ",array[2][i]-1);
    }
    printf("\n");
}
void hanuotaEx(int n,int from,int to,int bk){
    char f[MAX_STRING],t[MAX_STRING],b[MAX_STRING];
    switch(from){
        case 0:
            f[0] = 'A';
            f[1] = '\0';
            break;
        case 1:
            f[0] = 'B';
            f[1] = '\0';
            break;
        case 2:
            f[0] = 'C';
            f[1] = '\0';
            break;
        default:
            break;
    }
    switch(to){
        case 0:
            t[0] = 'A';
            t[1] = '\0';
            break;
        case 1:
            t[0] = 'B';
            t[1] = '\0';
            break;
        case 2:
            t[0] = 'C';
            t[1] = '\0';
            break;
        default:
            break;
    }
    if(n == 1){
        step ++;
        printf("[%3d]%s --%02d--> %s\n",step,f,n-1,t);
        index[from] = index[from] - 1;
        array[from][index[from]]    = 0;
        array[to][index[to]]        = n;
        index[to]   = index[to] + 1;
        show_progress();
    }else{
        hanuotaEx(n-1,from,bk,to);
        step ++;
        printf("[%3d]%s --%02d--> %s\n",step,f,n-1,t);
        index[from] = index[from] - 1;
        array[from][index[from]]    = 0;
        array[to][index[to]]        = n;
        index[to]   = index[to] + 1;
        show_progress();
        hanuotaEx(n-1,bk,to,from);
    }
}
int main(int argc,char** argv){
    int i = 0;
    for(i=0;i<NUM;i++){
        array[0][i] = NUM-i;
    }
    index[0] = NUM;
    index[1] = 0;
    index[2] = 0;
    show_progress();
    //hanuota(NUM,"A","C","B");
    hanuotaEx(NUM,0,2,1);
}
