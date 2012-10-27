#include "server.h"

const int MODE_GET = 1;
const int MODE_HEAD = 2;

char * HTTP_STAT_200 = "200 OK";
char * HTTP_STAT_403 = "403 FORBIDDEN";
char * HTTP_STAT_404 = "404 NOT FOUND";

char * ENTITY_BODY_404 = "Err 404: The Content Requested Not Found. (and everybody hates me... T_T)\n";
char * ENTITY_BODY_403 = "Err 403: Permission Denied on The Content Requested.\n";

const char * SERVER_IDTIFIER = "MINOR-HTTPD BUFFALO";

int serv_port;
pthread_t __serv_thread;

void serv_init(){
    serv_port = arg_listen_port;
    nw_listen_on_socket(serv_port);
    scheduler_init();
    tpool_init(serv_reply_to_remote);
    pthread_create(&__serv_thread, NULL, serv_t_server, NULL);
}

void * serv_t_server(void * arg){
    int remote_fd;
    struct serv_request * sreq;
    int valid;
    struct scheduler_job * s_job;
    printf("[DB] Main server thread started.\n");
    while (1){
        remote_fd = nw_accept_incoming();
        sreq = (struct serv_request *)malloc(sizeof(struct serv_request));
        s_job = (struct scheduler_job *)malloc(sizeof(struct scheduler_job));
        sreq->recv_time = util_get_current_time();
        sreq->remote_fd = remote_fd;
        sreq->remote_ip = (char *)malloc(64*sizeof(char));
        strcpy(sreq->remote_ip, nw_get_remote_addr());
        sreq->full_content = nw_read_from_remote(sreq->remote_fd);
        valid = util_parse_http_request(sreq);
        if (valid == 1){
            sreq->quot_line = util_get_first_line(sreq->full_content);
            printf("[DB] Valid request received.\n");
            printf("[DB] =================Request Content====================\n");
            printf("%s\n", sreq->full_content);
            printf("[DB] ==================End of Content====================\n");
            sreq->req_len = util_get_req_len(sreq->path);
            *s_job = scheduler_create_job(sreq, sreq->req_len);
            // the add job will triger the semaphore that the consumer of the scheduler had been waiting for.
            scheduler_add_job(s_job);
            printf("[DB] job (#%p) has been added to scheduler.\n", s_job);
        } else {
            printf("[DB] Discarding invalid requests (neither HTTP/1.x or HTTP/0.9). \n");
            serv_free_request(sreq);
        }
        // loop back to accept new incoming data
    }
}

void * serv_reply_to_remote(struct scheduler_job * the_job){
    // get the request from the job
    struct serv_request * sreq = (struct serv_request *)the_job->job_data;
    sreq->exec_time = util_get_current_time();
    // create a reply object and fill it
    struct serv_reply * srpy;
    srpy = util_get_response(sreq);
    // reply to remote
    nw_write_to_remote(sreq->remote_fd, srpy->full_content);
    nw_close_conn(sreq->remote_fd);
    // call log to file
    util_log_to_file(sreq->remote_ip, &(sreq->recv_time), &(sreq->exec_time), sreq->quot_line, srpy->status_code, srpy->content_len);
    // clean up
    serv_free_request(sreq);
    serv_free_reply(srpy);
    free(the_job);
}

void serv_free_request(struct serv_request * sreq){
    free(sreq->path);
    if(sreq->quot_line){free(sreq->quot_line);}
    free(sreq->remote_ip);
    if(sreq->full_content){free(sreq->full_content);}
    free(sreq);
}

void serv_free_reply(struct serv_reply * srpy){
    free(srpy->full_content);
    free(srpy);
}

void serv_destroy(){
    nw_destroy();
    tpool_destroy();
    scheduler_destroy();
    printf("[DB] server destroyed, ready to exit.\n");
}
