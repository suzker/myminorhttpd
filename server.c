#include "server.h"

const int MODE_GET = 1;
const int MODE_HEAD = 2;
const int PROT_HTTP = 1;
const int PROT_HTTPS = 2;

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
        valid = serv_parse_http_request(read_output, req);
        if (valid != 0){
            req->remote_fd = remote_fd;
            req->quot_req = _get_first_line_(read_output);
            req->recv_time = _get_current_time_();
            req->remote_ip = nw_get_remote_addr();
            req_len = util_get_req_len(req->path);
            *s_job = scheduler_create_job(req, req_len);
            // the add job will triger the semaphore that the consumer of the scheduler had been waiting for.
            scheduler_add_job(s_job);
        }
        // loop back to accept new incoming data
    }
}

int serv_parse_http_request(char * req, struct serv_request * s_req){
    // split
    char * req_args[3];
    char * pch;
    pch  = strtok(req, " ");
    int i = 0;
    while (pch != NULL && i<3){
        req_args[i++] = pch;
        pch = strtok(NULL, " ");
    }
    
    // GET or HEAD
    if (strcmp(req_args[0], "GET")==0){
        s_req->mode = MODE_GET;
    } else if(strcmp(req_args[1], "HEAD") == 0){
        s_req->mode = MODE_HEAD;
    } else {
        return 0;
    }

    // URL
    if (req_args[1][0]!='/'){
        return 0;
    } else {
        s_req->path = req_args[1];
    }

    // HTTP/1.x
    if (strlen(req_args[2])>=8){
        char subbuff[7];
        memcpy(subbuff, req_args[2], 7);
        if (strcmp(subbuff, "HTTP/1.")==0){
            s_req->protocol = 1.0;
        } else if(strcmp(subbuff, "HTTP/0.")){
            s_req->protocol = 0.9;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

void * serv_reply_to_remote(struct scheduler_job * the_job){
    int status_code;
    struct serv_request * my_resq = (struct serv_request *)the_job->job_data;
    long req_len = the_job->len;
    char * resp_content;
    if (my_resq->mode == MODE_GET){
        resp_content = (char *)malloc((req_len+1566)*sizeof(char));
    } else {
        resp_content = (char *)malloc((1566)*sizeof(char));
    }
    status_code = util_get_response(my_resq->path, my_resq->mode, resp_content);
    nw_write_to_remote(my_resq->remote_fd, resp_content);
    time_t * exec_time = _get_current_time_();
    // call log to file and clean up
    log_to_file(my_resq->remote_ip, my_resq->recv_time, exec_time, my_resq->quot_req, status_code, req_len);
    free(resp_content);
    free(my_resq);
}
