/**
 Server component
  contains a server thread to take incoming socket requests. 
  also methods related to respond a appropriate responds to the remote clients.

  Author: Zhiliang Su
  Revised: Oct 20, 2012
*/
#ifndef __HEADER_SERVER__
#define __HEADER_SERVER__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "network.h"
#include "utility.h"
#include "scheduler.h"
#include "tpool.h"

struct serv_request{
    int * remote_fd;
    int mode;
    char * path;
    float protocol;
    time_t * recv_time;
    char * quot_req;
    char * remote_ip;
};

/**
 function: serv_init
  to intialize the server
*/
void serv_init();

/**
 Thread: server_t_serv
  take incoming sockets
*/
void * serv_t_server(void *);

/**
 function: serv_is_http_request(char *)
  to check if a request from the remote connection is a valid http(1.0) request.
   input:
        char * req_str: the whole request in cstring
        struct serv_request *: the structure pointer to store the request details if the request is valid
   output:
        1 for valid and 0 for invalid
*/
int serv_parse_http_request(char *, struct serv_request *);

/**
 function: serv_reply_to_remote
  to reply to a certain remote given a scheduler_job. will call utility to retrieve the content of the file responsible for the request and write to output.
  ** this func will be sent to the tpool to work inside one of the worker threads.
   input:
        struct scheduler_job * : the job
   output:
        1 for success otherwise failed
*/
void* serv_reply_to_remote(struct scheduler_job *);
#endif
