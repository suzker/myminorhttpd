#include "network.h"
// __SOMAXCONN a desired size of backlog on listening, but might shrink to actual limit of a specific OS.
const int __SOMAXCONN = 128;
const int __buff_size = 1024;
int __socket_fd, __portnum;
socklen_t __len_remote;
struct sockaddr_in __serv_addr, __remote_addr;

void nw_listen_on_socket(int portno){
    __socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (__socket_fd < 0){
       perror("Error establishing socket on port, exiting... \n"); 
    }
    __portnum = portno;
    bzero((char *)&__serv_addr, sizeof(__serv_addr)); // clear and fill zeros in socket addr structure
    //setup __serv_addr
    __serv_addr.sin_family = AF_INET;
    __serv_addr.sin_addr.s_addr = INADDR_ANY;
    __serv_addr.sin_port = htons(__portnum);
    //bind
    if (bind(__socket_fd, (struct sockaddr *) &__serv_addr, sizeof(__serv_addr)) < 0){
        perror("Error binding the socket to the port, exiting...\n");
    }
    //listen
    listen(__socket_fd, __SOMAXCONN);
}

int * nw_accept_incoming(){
    __len_remote = sizeof(__serv_addr);
    int *__remote_fd;
    __remote_fd = (int *)malloc(sizeof(int));
    *__remote_fd = accept(__socket_fd, (struct sockaddr *) &__remote_addr, &__len_remote);
    if (*__remote_fd < 0){
        perror("Error accepting remote connections, exiting...\n");
    }
    return __remote_fd;
}

char * nw_read_from_remote(int *remote_fd){
    char *__buff;
    __buff = (char*)malloc(__buff_size*sizeof(char));
    // clear buffer
    bzero(__buff, __buff_size);
    int count = 1;
    int n = 0;
    n = read(*remote_fd, __buff, (__buff_size - 1));
    if (n<1) { perror("Error reading stream, exiting...\n");}
    return __buff;
}

void nw_write_to_remote(int *remote_fd, char *content){
    int n = 0;
    n = write(*remote_fd, content, strlen(content));
    if (n < 0) {perror("Error writing responds to the remote connection, exiting...\n");}
}

void nw_close_conn(int *remote_fd){
    close(*remote_fd);
}

void nw_close_serv(){
    close(__socket_fd);
}

void nw_destroy(){
    // TODO to be added...    
}
