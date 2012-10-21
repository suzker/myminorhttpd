#include "server.h"

int serv_port;

void serv_init(){
    serv_port = arg_listen_port;
    nw_listen_on_socket(serv_port);
    scheduler_init();
    tpool_init(serv_reply_to_remote);
}

void * serv_t_server(void * arg){
    int * remote_fd;
    char * read_output;
    struct serv_request * req;
    int valid;
    long req_len;
    struct scheduler_job * s_job;
    while (1){
        req = (struct serv_request *)malloc(sizeof(struct serv_request));
        s_job = (struct scheduler_job *)malloc(sizeof(struct scheduler_job));
        remote_fd = nw_accept_incoming();
        /* the server thread will be hanged here til' there is a socket conn. */
        read_output = nw_read_from_remote(remote_fd);
        valid = serv_is_http_request(read_output, req);
        if (valid){
            // TODO call utility to calculate the length of the request
            req_len = util_get_req_len();
            // TODO call scheduler to add job
            *s_job = scheduler_create_job(req, req_len);
            // the add job will triger the semaphore that the consumer of the scheduler had been waiting for.
            scheduler_add_job(s_job);
        }
        // loop back to accept new incoming data
    }
}

int serv_is_http_request(char * req, struct serv_request * s_req){
    // TODO parse the req
    // TODO if-else determine the req is valid or not
    // TODO if valid save into s_req and return
}

void * serv_reply_to_remote(struct scheduler_job * the_job){
    // TODO get the "struct serv_request" out of "the_job"
    // TODO call utility to get the file content
    // TODO call nw func "nw_write_to_remote"
}
