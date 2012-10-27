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

struct serv_request{
    int remote_fd;
    int mode;
    char * path;
    time_t recv_time;
    time_t exec_time;
    char * quot_line;
    char * remote_ip;
    char * full_content;
    long req_len;
};

struct serv_reply{
    int status_code;
    char * full_content; // all we have to response.
    long content_len; // it's the entity-body's length.
};

// include the utility lib later after the structs are defined to void warnings
#include "utility.h"
#include "network.h"
#include "scheduler.h"
#include "tpool.h"

extern const int MODE_GET;
extern const int MODE_HEAD;

extern char * HTTP_STAT_200;
extern char * HTTP_STAT_403;
extern char * HTTP_STAT_404;

extern char * ENTITY_BODY_403;
extern char * ENTITY_BODY_404;

extern const char * SERVER_IDTIFIER;

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
 function: serv_reply_to_remote
  to reply to a certain remote given a scheduler_job. will call utility to retrieve the content of the file responsible for the request and write to output.
  ** this func will be sent to the tpool to work inside one of the worker threads.
   input:
        struct scheduler_job * : the job
   output:
        1 for success otherwise failed
*/
void* serv_reply_to_remote(struct scheduler_job *);

/**
 function: serv_free_request()
  to free a serv_request struct
*/
void serv_free_request(struct serv_request * sreq);

/**
 function: serv_free_reply()
  to free a serv_reply struct
*/
void serv_free_reply(struct serv_reply * srpy);

/**
 function: serv_destroy()
  to destroy the whole server, ready to exit
*/
void serv_destroy();
#endif
