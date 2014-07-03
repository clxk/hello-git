#include "CLI.h"
#include <pthread.h>

//#define PTHREAD_SUPPORT
//#define DEBUG
#ifdef DEBUG
#define LOG(format, ...) fprintf(stdout,format, ##__VA_ARGS__)
#else
#define LOG(format, ...)
#endif

#define HEADER "$$HEADERSTART"
#define TAIL   "$$HEADEREND"
#define MAX_LOAD_LEN  (1024)
#define GET_SHORT_VALUE(addr)   *(unsigned short *)(addr)
#define STRING_BUFFER_LEN (1024)
#ifdef PTHREAD_SUPPORT
#define PTHREAD_MAX (10)
#endif

static size_t globalAddrOffset = 0;
static long long memSize = 0;
static int pointNum = 0,lineNum = 0,layerNum = 0,pointsArrayNum = 0;
static char strbuffer[STRING_BUFFER_LEN] = {0};

void print_usage(){
    printf("usage: <to be completed ...>\n");
}
size_t nextCmdAddr(size_t addr,int size,char * buff,FILE* f){
    static int count = 0;
    int ret = 0;

    count ++;
    if(addr+size > MAX_LOAD_LEN - 4){
        memcpy(buff,&buff[addr+size],MAX_LOAD_LEN - addr - size);
        memset(&buff[MAX_LOAD_LEN - addr - size],0,addr+size);
        if(ret = fread(&buff[MAX_LOAD_LEN - addr - size],addr+size,1,f) >= 0){
            globalAddrOffset += addr+size;
            return 0;
        }else{
            LOG("[%d]reload file error,addr = 0x%x,size = 0x%x!!!\n",count,addr,size);
            exit(1);
        }

    }
    return addr + size;
}
void dumpObject(object* pObj){
    polyLine*       pPolyLine = NULL;
    polyPoint*      pPolyPoint = NULL;
    int             i = 0;

    LOG("--header-->\n");
    LOG("%s\n",pObj->header);
    LOG("<--tail--\n");
    LOG("\n\n");
    while(pObj->layers){
        LOG("--layer start-->\n");
        LOG("\tz=%f\n",pObj->layers->z);
        pPolyLine = pObj->layers->polyLines;
        while(pPolyLine){
            LOG("\t--polyline start-->\n");
            LOG("\tID:%d",pPolyLine->id);
            LOG("\tDIR:%d",pPolyLine->dir);
            LOG("\tNUM:%d\n",pPolyLine->num);
            pPolyPoint = pPolyLine->points;
            //while(pPolyPoint){
                for(i = 0;i < pPolyLine->num; i++){
                    LOG("\t\t(%f,%f)\n",pPolyPoint->x[i],pPolyPoint->y[i]);
                }
                //pPolyPoint = pPolyPoint->next;
            //}
            LOG("\t--polyline end-->\n");
            pPolyLine = pPolyLine->next;
        }
        LOG("<--layer end--\n");
        pObj->layers = pObj->layers->next;
    }
}
void insertLayer(object *pObj,layer* pLayer){
    layer* p = NULL;
    if(pObj->layers == NULL){
        pObj->layers = pLayer;
    }else{
        p = pObj->layers;
        while(p->next)p = p->next;
        p->next = pLayer;
    }
}
void insertPolyLine(layer *pLayer,polyLine* pPolyLine){
    polyLine* p = NULL;
    if(pLayer->polyLines == NULL){
        pLayer->polyLines = pPolyLine;
    }else{
        p = pLayer->polyLines;
        while(p->next)p = p->next;
        p->next = pPolyLine;
    }
}
object* processFile(const char *file){
    FILE*           f = NULL;
    char            buff[MAX_LOAD_LEN] = {0};
    size_t          dataOffset = 0;
    size_t          cmdAddr = 0;
    unsigned short  nextCmd = 0;
    polyPoint*      pPolyPoint = NULL;
    polyPoint*      pPrePolyPoint = NULL;
    polyLine*       pPolyLine = NULL;
    layer*          pLayer = NULL;
    object*         pObject = NULL;

    f = fopen(file, "rb");
    if(f == NULL){
        LOG("can't open file: %s!!!\n",file);
        return NULL;
    }

    //size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    //char *strstr(char *haystack, char *needle);
    if(fread(buff,MAX_LOAD_LEN,1,f) > 0 && !strncmp(buff,HEADER,strlen(HEADER))){
        char *p = NULL,*q = NULL;
        int count = 0;

        LOG("CLI format recognized!!!\n");
        pObject = (object*)malloc(sizeof(object));                          //malloc memory for <object>
        memset(pObject,0,sizeof(object));
        memSize += sizeof(object);
        p = strstr(buff,TAIL);
        if(p){
            dataOffset = p+strlen(TAIL)-buff;
            memcpy(pObject->header,buff,dataOffset-1);                      //make sure dataOffset < 257
            cmdAddr = dataOffset;
LOOP:
            nextCmd = *(unsigned short *)(&buff[cmdAddr]);
            switch(nextCmd){
                case 127:
                    break;
                case 128:
                    cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                    pLayer = (layer*)malloc(sizeof(layer));                 //malloc memory for <layer>
                    memset(pLayer,0,sizeof(layer));
                    layerNum++;
                    memSize += sizeof(layer);
                    pLayer->z = (float)GET_SHORT_VALUE(&buff[cmdAddr]);
                    insertLayer(pObject,pLayer);
                    cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                    goto LOOP;
                    break;
                case 129:
                    pPolyLine = (polyLine*)malloc(sizeof(polyLine));        //malloc memory for <polyLine>
                    memset(pPolyLine,0,sizeof(polyLine));
                    memSize += sizeof(polyLine);
                    lineNum++;
                    cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                    pPolyLine->id  = GET_SHORT_VALUE(&buff[cmdAddr]);
                    cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                    pPolyLine->dir = GET_SHORT_VALUE(&buff[cmdAddr]);
                    cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                    pPolyLine->num = GET_SHORT_VALUE(&buff[cmdAddr]);
                    pointNum += pPolyLine->num;
                    for(int i=0;i<pPolyLine->num;i++){
                        unsigned short x,y;
                        if(i == 0){
                            pPolyPoint = (polyPoint*)malloc(sizeof(polyPoint)); //malloc memory for <polyPoint>
                            memset(pPolyPoint,0,sizeof(polyPoint));
                            memSize += sizeof(polyPoint);
                            pPolyPoint->x = (float*)malloc(pPolyLine->num*sizeof(float));
                            pPolyPoint->y = (float*)malloc(pPolyLine->num*sizeof(float));
                            memSize += pPolyLine->num*sizeof(float)*2;
                            pointsArrayNum++;
                        }
                        cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                        x = *(unsigned short *)(&buff[cmdAddr]);
                        cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                        y = *(unsigned short *)(&buff[cmdAddr]);
                        pPolyPoint->x[i] = (float)x;
                        pPolyPoint->y[i] = (float)y;
                        if(pPolyLine->points == NULL){
                            pPolyLine->points = pPolyPoint;
                            pPrePolyPoint = pPolyPoint;
                            //pPrePolyPoint->next = NULL;
                        }
                    }
                    insertPolyLine(pLayer,pPolyLine);
                    cmdAddr = nextCmdAddr(cmdAddr,2,buff,f);
                    goto LOOP;
                    break;
                case 130:
                    break;
                case 131:
                    break;
                case 132:
                    break;
                default:
                    if(globalAddrOffset+cmdAddr >= ftell(f)){
                        LOG("Analysis completed! File length is: 0x%x\n",ftell(f));
#ifdef DEBUG
                        dumpObject(pObject);
#endif
                        break;
                    }
                    LOG("Unknown cmd: 0x%x at addr 0x%x\n",nextCmd,globalAddrOffset+cmdAddr);
                    break;
            }
        }else{
            LOG("can't find %s,please check the file format!!!\n",TAIL);
        }
    }else{
        LOG("Unknown file format!!!\n");
    }

    fclose(f);
    return pObject;
}
#ifdef PTHREAD_SUPPORT
void * thr_fn(void *arg){
    object* pObject = NULL;

    printf("child thread run ...\n");
    pObject = processFile((const char *)arg);
    if(pObject == NULL){
        printf("[CHILD] process file: %s error!!!\n",(char*)arg);
    }else{
        printf("[CHILD] process file: %s success!!!\n",(char*)arg);
    }
    printf("child thread stop ...\n");
}
#endif
int main(int argc, char **argv){
    //time statistic
    clock_t start, finish;
    double total_time;
    //output debug info
    char *p = &strbuffer[0];
#ifdef PTHREAD_SUPPORT
    //pthreads related
    pthread_t tid[PTHREAD_MAX];
    int thread_count = 0,i = 0,err = 0;
    void* tret;
#endif

    p+=sprintf(p,"\n---------------------------------------------------------------------------------------------------\n");
    start = clock();
    for(int argn = 1; argn < argc; argn++){
        char* str = argv[argn];

        if (str[0] == '-'){
            for(str++; *str; str++){
                switch(*str){
                    case 'h':
                        print_usage();
                        exit(1);
                    default:
                        printf("Unknown option: %c\n", *str);
                        break;
                }
            }
        }else{
            try {
                p+=sprintf(p,"argv[%d] = %s\n",argn,argv[argn]);
#ifdef PTHREAD_SUPPORT
                err = pthread_create(&tid[thread_count++],NULL,thr_fn,argv[argn]);
                if(err !=0){
                    printf("can't create thread: %s\n",strerror(err));
                }
#else
                processFile(argv[argn]);
#endif
            }catch(...){
                printf("Unknown exception\n");
                exit(1);
            }
        }
    }
#ifdef PTHREAD_SUPPORT
    for(i=0;i<thread_count;i++){
        err = pthread_join(tid[i],&tret);
        if(err != 0){
            printf("can't join with thread%d: %s\n",i,strerror(err));
        }else{
            printf("thread join: %d!!!\n",i);
        }
    }
#endif
    finish = clock();
    total_time = (double)(finish-start) / CLOCKS_PER_SEC;
    p+=sprintf(p,"TIME:%f MEM:%lld Point:%d Line:%d Layer:%d PointsArrayNum:%d\n",
            total_time,memSize,pointNum,lineNum,layerNum,pointsArrayNum);
    printf("%s\n",strbuffer);

    return 0;
}

