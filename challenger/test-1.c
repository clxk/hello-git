#include <stdio.h>
#include <stdlib.h>

#define     PLAYER_NUM      (21)
#define     KILL            (3)
char *  createPlayerList(int num){
    int i = 0;
    char * pList = malloc(sizeof(int)*num);
    for(i=0;i<num;i++){
        pList[i] = i;
    }
    return pList;
}
int main(int argc,char** argv){
    int i = 0,counter = 0,surive = PLAYER_NUM,loops = 0,skips = 0;
    char *p = createPlayerList(PLAYER_NUM);
    while(surive > 1){
        loops ++;
        i %= PLAYER_NUM;
        if(p[i] == -1){
            i++;
            skips ++;
            continue;
        }
        counter ++;
        if(counter == KILL){
            p[i] = -1;
            counter = 0;
            surive --;
            printf("%02d is out!!!\n",i%PLAYER_NUM);
        }
        i++;
    }
    printf("loops = %d,skips = %d\n",loops,skips);
    for(i=0;i<PLAYER_NUM;i++){
        if(p[i] != -1){
            printf("last surive player's index = %d\n",i);
        }
    }
    return 0;
}
