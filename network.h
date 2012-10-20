/** 
 networking utilities wrappers under linux
 author: Zhiliang SU
 revised: 19 Oct, 2012
*/
#ifndef __HEADER_NETWORK__
#define __HEADER_NETWORK__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 function: nw_listen_on_socket
  to set up a socket by binding and listening to the specific port number
   ipnut: 
        portno, the port number
*/
void nw_listen_on_socket(int portno);

/**
 function: nw_accept_incoming
  to open put the socket into the accepting status to serve incoming requests
   output:
        int * remote_fd: the remote file descriptor of the remote connection
*/
int * nw_accept_incoming();

/**
 function: nw_read_from_remote
  to read a message from the remote connection
   input: 
        remote_fd: the file descriptor of the remote connection
   output:
        char *: the output string
*/
char * nw_read_from_remote(int *remote_fd);

/**
 function: nw_write_to_remote
  to write a message to the remote connection
   input:
        remote_fd: the file descriptor of the remote connection
        content: the content to be written to the remote connection
*/
void nw_write_to_remote(int *remote_fd, char *content);

/**
 function: nw_destroy
  to stop all socket and free all used variables
*/
void nw_destroy();
#endif
