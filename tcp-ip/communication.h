/***************************************************************************************************
 *   File
 *       communication.h
 *
 *   Abstract
 *       common structures for socket programming
 *
 *   Author & date
 *       Dayong Yang
 *       2014-07-03
 ***************************************************************************************************/
#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//variable macros
#define SERVER_IP           INADDR_ANY
#define SERVER_PORT         (9191)
#define CLIENT_IP           INADDR_ANY
#define CLIENT_PORT         (0)
#define LISTEN_QUEUE        (20)
#define BUFFER_SIZE         (1024)
//function macros
#define SYSTEM_ERR(format, ...) do{\
    printf("[%d]"format,getpid(),##__VA_ARGS__);\
    exit(1);\
}while(0)
#define SYSTEM_INFO(format, ...) do{\
    printf("[%d]"format,getpid(),##__VA_ARGS__);\
}while(0)
#define DEBUG(format, ...) do{\
    printf(format,##__VA_ARGS__);\
}while(0)


#endif
