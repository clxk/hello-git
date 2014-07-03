/***************************************************************************************************
 *   File
 *       communication-server.c
 *
 *   Abstract
 *       server
 *          1th-level thread:   waiting for connection request! If comming fork a child to deal with it.
 *          2th-level thread:   fork a child then exit,other wise 1th maybe suspended.
 *          3th-level thread:   an independent thread,try to receive data from client side
 *          4th-level thread:   deal with requests form client side then exit.
 *
 *   Author & date
 *       Dayong Yang
 *       2014-07-03
 ***************************************************************************************************/
#include "communication.h"
static pid_t pid_1th,pid_2th,pid_3th;

int main(int argc, char **argv){
    struct sockaddr_in  server_addr;
    int                 server_socket;
    int                 opt = 1;

    //initialize sockaddr on server
    bzero(&server_addr,sizeof(server_addr));                        //zero infill
    server_addr.sin_family      = AF_INET;                          //IPv4
    server_addr.sin_addr.s_addr = htons(SERVER_IP);                 //host endian to network endian - IP addr
    server_addr.sin_port        = htons(SERVER_PORT);               //host endian to network endian - port number
    //create an endpoint for communication
    server_socket               = socket(PF_INET,SOCK_STREAM,0);
    if(server_socket < 0){
        SYSTEM_ERR("%-20s create socket failed!","Server:");
    }
    //set options on sockets: SOL_SOCKET(setting for socket leveel) SO_REUSEADDR(reuse addr if *opt != 0)
    setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //bind a name to a socket
    if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))){
        SYSTEM_ERR("%-20s bind port(%d) failed!","Server:",SERVER_PORT);
    }
    //listen for connections on a socket
    if (listen(server_socket,LISTEN_QUEUE)){
        SYSTEM_ERR("%-20s listen failed!","Server:");
    }

    while(1){
        struct sockaddr_in  client_addr;
        socklen_t           length = sizeof(client_addr);
        int                 sockfd = -1;
        pid_t               pid;

        SYSTEM_INFO("%-20s watting for request ...\n","Server:");
        sockfd = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if(sockfd == -1){
            SYSTEM_ERR("%-20s accept error!","Server:");
        }else{
            SYSTEM_INFO("%-20s connectting ...\n","Server:");
            pid = fork();
            if(pid < 0){
                SYSTEM_ERR("%-20s fork child error!","Server:");
            }else if(pid > 0){
                //parent-process£¬the 1th level process: close sockfd then wait until 2th level process exit.
                pid_1th = getpid();
                close(sockfd);
                if(waitpid(pid,NULL,0) != pid){
                    SYSTEM_ERR("%-20s wait pid error!","Server:");
                }
            }else{
                pid = fork();
                if(pid < 0){
                    SYSTEM_ERR("\t%-20s fork child error!","Child-Server:");
                }else if(pid > 0){
                    //child-process,the 2th level process,close sockfd then exit.(create 3th level process before exit)
                    pid_2th = getpid();
                    SYSTEM_INFO("\t%-20s running ...\n","Child-Server:");
                    close(sockfd);
                    break;
                }else{
                    char    buffer[BUFFER_SIZE];
                    time_t  timep;

                    //The 3th level process,maintain connection between 'server' and 'client'
                    pid_3th = getpid();
                    SYSTEM_INFO("\t\t%-20s running ...\n","Grandson-Server:");
                    bzero(buffer,BUFFER_SIZE);
                    timep = time(NULL);
                    inet_ntop(AF_INET,&client_addr.sin_addr,buffer,INET_ADDRSTRLEN);
                    SYSTEM_INFO("\t\t%-20s %-15s  connected!  @-%s\n","Grandson-Server:",buffer,ctime(&timep));
                    send(sockfd,"hello,well come to server!\n",strlen("hello,well come to server!\n"),0);

                    while(1){
                        SYSTEM_INFO("\t\t%-20s receiving ...\n","Grandson-Server:");
                        bzero(buffer,BUFFER_SIZE);
                        length = recv(sockfd,buffer,BUFFER_SIZE,0);
                        if(length < 0){
                            SYSTEM_INFO("\t\t%-20s receive data failed!\n","Grandson-Server:");
                        }else if(length == 0){
                            break;
                        }else{
                            pid = fork();
                            if(pid < 0){
                                SYSTEM_ERR("\t\t%-20s fork child error!","Grandson-Server:");
                            }else if(pid == 0){
                                //The 4th level process,do what the client requested,close sock fd and exit.
                                SYSTEM_INFO("\t\t\t%-20s running ...\n","Real-Work-Horse:");
                                if(dup2(sockfd,STDOUT_FILENO) != STDOUT_FILENO ||
                                        dup2(sockfd,STDERR_FILENO) != STDERR_FILENO){
                                    SYSTEM_ERR("\t\t\t%-20s dup2 error!","Real-Work-Horse:");
                                }
                                close(sockfd);
                                execl("/bin/cat","cat",buffer,(char*)0);
                            }else{
                                if(waitpid(pid,NULL,0) != pid){
                                    SYSTEM_ERR("\t\t%-20s wait pid error!","Grandson-Server:");
                                }else{
                                    DEBUG("[%d]""\t\t\t%-20s exit ...\n",pid,"Real-Work-Horse:");
                                }
                                break;
                            }
                        }
                    }
                    close(sockfd);
                    break;
                }
            }
        }
    }

    if(getpid() == pid_1th){
        SYSTEM_INFO("%-20s exit ...\n","Server:");
    }else if(getpid() == pid_2th){
        SYSTEM_INFO("\t%-20s exit ...\n","Child-Server:");
    }else if(getpid() == pid_3th){
        SYSTEM_INFO("\t\t%-20s exit ...\n","Grandson-Server:");
    }else{
        SYSTEM_INFO("%-20s\n","Process Exit:");
    }
    close(server_socket);
}
