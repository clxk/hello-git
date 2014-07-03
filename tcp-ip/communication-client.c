/***************************************************************************************************
 *   File
 *       communication-client.c
 *
 *   Abstract
 *       client
 *
 *   Author & date
 *       Dayong Yang
 *       2014-07-03
 ***************************************************************************************************/
#include "communication.h"
int main(int argc, char **argv){
    char                buffer[BUFFER_SIZE];
    socklen_t           length;
    struct sockaddr_in  server_addr;
    struct sockaddr_in  client_addr;
    int                 client_socket;
    //int                 opt = 1;
    unsigned int        count = 0;

    if (argc != 4){
        SYSTEM_ERR("Usage: ./%s clientIPAddress SrcFileName DstFileName\n",argv[0]);
    }
    //initialize sockaddr on client
    bzero(&client_addr,sizeof(client_addr));                        //zero infill
    client_addr.sin_family      = AF_INET;                          //IPv4
    client_addr.sin_addr.s_addr = htons(CLIENT_IP);                 //host endian to network endian - IP addr
    client_addr.sin_port        = htons(CLIENT_PORT);               //host endian to network endian - port number
    //create an endpoint for communication
    client_socket               = socket(PF_INET,SOCK_STREAM,0);
    if(client_socket < 0){
        SYSTEM_ERR("%-20s create socket failed!","client:");
    }
    //set options on sockets: SOL_SOCKET(setting for socket leveel) SO_REUSEADDR(reuse addr if *opt != 0)
    //setsockopt(client_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //bind a name to a socket
    if(bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr))){
        SYSTEM_ERR("%-20s bind port(%d) failed!","client:",CLIENT_PORT);
    }

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(SERVER_PORT);
    if(inet_aton(argv[1],&server_addr.sin_addr) == 0){
        SYSTEM_ERR("%-20s server IP address error!","client:");
    }
    length                      = sizeof(server_addr);
    if(connect(client_socket,(struct sockaddr*)&server_addr,length) < 0){
        SYSTEM_ERR("%-20s can't connect to %s ","client:",argv[1]);
    }

    bzero(&buffer,BUFFER_SIZE);
    length = recv(client_socket,buffer,BUFFER_SIZE,0);
    DEBUG("%s\n",buffer);
    bzero(&buffer,BUFFER_SIZE);
    strncpy(buffer,argv[2], strlen(argv[2])>BUFFER_SIZE?BUFFER_SIZE:strlen(argv[2]));
    send(client_socket,buffer,BUFFER_SIZE,0);

    FILE * fp = fopen(argv[3],"w");
    if(NULL == fp ){
        SYSTEM_ERR("%-20s%s can't open to write","client:",argv[3]);
    }
    bzero(buffer,BUFFER_SIZE);
    while(length = recv(client_socket,buffer,BUFFER_SIZE,0)){
        if(length < 0){
            break;
        }
        count += fwrite(buffer,sizeof(char),length,fp);
        bzero(buffer,BUFFER_SIZE);
        DEBUG("%-20s %d bytes transfered ...\n","client:",count);
    }

    DEBUG("%-20s file transfer completed!\n","client:");
    fclose(fp);
    close(client_socket);
    return 0;
}
