#include "utility.h"

int arg_debug_mode;
int arg_usage_sum;
char arg_log_file[1000];
int arg_listen_port;
char arg_root_folder[1000];
int arg_queue_time;
int arg_thread_num;
int arg_schedule_mode;

int arg_parser(int argc, char * argv[]){
   init_arg();
   int i;
   int err = 0;
   for (i = 1; i < argc; ++i){
       if (err) {break;}
       if (argv[i][0] != '-'){
           err = 1;
           break;
       } else {
           switch (argv[i][1]){
               case 'd':
                    arg_debug_mode = 1;
                    break;
               case 'h':
                    arg_usage_sum = 1;
                    break;
               case 'l':
                    if (++i < argc){
                        strcpy(arg_log_file, argv[i]);
                    } else {
                        err = 1;
                    }
                    break;
               case 'p':
                    arg_listen_port = ++i < argc ? atoi(argv[i]) : 8080;
                    err = i < argc ? 0 : 1;
                    break;
               case 'r':
                    if (++i < argc){
                        strcpy(arg_root_folder, argv[i]);
                    } else {
                        err = 1;
                    }
                    break;
               case 't':
                    arg_queue_time = ++i < argc ? atoi(argv[i]) : 60;
                    err = i < argc ? 0 : 1;
                    break;
               case 'n':
                    arg_thread_num = ++i < argc ? atoi(argv[i]) : 1;
                    err = i < argc ? 0 : 1;
                    break;
               case 's':
                    if (++i < argc){
                        arg_schedule_mode = !strcmp(argv[i], "sjf") ? 1 : 0;
                    }
                    break;
               default:
                    err = 1;
                    break;
               }
          }
   }
   if (err) {
       printf ("Argument Error! \n");
       return 0;
   }
}

void init_arg(){
    arg_debug_mode = 0;
    arg_usage_sum = 0;
    strcpy(arg_log_file, "N/A");
    arg_listen_port = 7717;
    strcpy(arg_root_folder, "/home/zsu2/www");
    arg_queue_time = 60;
    arg_thread_num = 1;
    arg_schedule_mode = 0;
}

int util_log_to_file(char remote_ip_addr[], time_t *time_queued, time_t *time_exec, char quote[], int status, long response_length){
    if (strcmp(arg_log_file, "N/A") == 0){return 0;}
    FILE * pFile;
    if (pFile = fopen(arg_log_file, "r")){ 
        fclose(pFile);
        pFile = fopen(arg_log_file, "a");
    } else {
        pFile = fopen(arg_log_file, "w");
    }
    
    struct tm *ptm_q, *ptm_e;
    ptm_q = gmtime ( time_queued );
    ptm_e = gmtime ( time_exec );
    char *monstr_q, *monstr_e;
    monstr_q = _int_mon2str_(&(ptm_q->tm_mon));
    monstr_e = _int_mon2str_(&(ptm_e->tm_mon));
    fprintf(pFile, "%s - [%02d/%s/%d:%02d:%02d:%02d - 0000] [%02d/%s/%d:%02d:%02d:%02d - 0000] \"%s\" %d %ld", remote_ip_addr, ptm_q->tm_mday, monstr_q, 1900+ptm_q->tm_year, ptm_q->tm_hour, ptm_q->tm_min, ptm_q->tm_sec, ptm_e->tm_mday, monstr_e, 1900+ptm_e->tm_year, ptm_e->tm_hour, ptm_e->tm_min, ptm_e->tm_sec, quote, status, response_length);
    fclose(pFile);
}

char* _int_mon2str_(int * monint){
    char * mon_set[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    return mon_set[*monint];
}    

char * _int_weekdays_(int * dayint){
    char * day_set[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    return day_set[*dayint];
}

long util_get_req_len(char * path){
    long len;
    char * abs_path = util_get_abs_path(path);
    char * idx_path;
    char * flist;
    int f_status = util_is_file_exist(abs_path);
    int i_status;
    if (f_status == 2){ // look for default index.html
        idx_path = util_get_idx_path(abs_path);
        i_status = util_is_file_exist(idx_path);
        if (i_status == 0){
            flist = util_get_flist(abs_path);
            len = strlen(flist);
            free(flist);
        }
        if (i_status == 3){len = strlen(ENTITY_BODY_403);}
        if (i_status == 1){len = util_get_file_len(idx_path);}
        free(idx_path);
    } else {
        if (f_status == 0){len = strlen(ENTITY_BODY_404);}
        if (f_status == 3){len = strlen(ENTITY_BODY_403);}
        if (f_status == 1){len = util_get_file_len(abs_path);}
    }
    free(abs_path);
    return len;
}

void print_help(){
    // TODO tba
}

char * util_get_flist(char * dir_path){
    FILE *fp;
    char path[1035];
    char * buff;
    buff = (char *)malloc(65535*sizeof(char));
    char command[1035];
    sprintf(command, "/bin/ls %s -lt", dir_path);
    fp = popen(command, "r");
    if (fp == NULL){
        perror("Error reading dirctory file list. I will suggest a retry.");
    }

    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        strcat(buff, path);
    }
    pclose(fp);
    return buff;
}

long util_get_file_len(char * file_path){
    struct stat file_stat;
    if (stat(file_path, &file_stat) < 0){
        return -1;
    }
    return file_stat.st_size;
}

char * util_get_abs_path(char * rel_path){
    char * abs_path;
    abs_path= (char *)malloc((strlen(arg_root_folder)+strlen(rel_path)+5)*sizeof(char));
    strcpy(abs_path, arg_root_folder);
    strcat(abs_path, rel_path);
    return abs_path;
}

char * util_get_idx_path(char * abs_path){
    char * idx_path;
    idx_path = (char * )malloc((strlen(abs_path)+strlen("index.html")+5)*sizeof(char));
    strcpy(idx_path, abs_path);
    strcat(idx_path, "index.html");
    return idx_path;
}

char * util_get_file_content(char * abs_path){
    char * buffer = 0;
    long length;
    FILE * f = fopen (abs_path, "rb");

    if (f) {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer) {
            fread (buffer, 1, length, f);
        }
        fclose (f);
    }

    return buffer;
}

time_t util_get_current_time(){
    time_t raw_t;
    time(&raw_t);
    return raw_t;
}

char * util_get_time_str(time_t * raw_t){
    char * time_str;
    time_str = (char *)malloc(128*sizeof(char));
    struct tm * ptm;
    ptm = gmtime(raw_t);
    sprintf(time_str, "%s, %02d %s %04d %02d:%02d:%02d GMT", _int_weekdays_(&(ptm->tm_wday)), ptm->tm_mday, _int_mon2str_(&(ptm->tm_mon)), 1900+ptm->tm_year, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    return time_str;
}

time_t * util_get_file_timestamp(char * abs_path){
    time_t * timestamp;
    timestamp = (time_t*)malloc(sizeof(time_t));
    struct stat file_stat;
    if (stat(abs_path, &file_stat) < 0){
        return NULL;
    }
    *timestamp = file_stat.st_mtime;
    return timestamp;
}

char * util_get_first_line(char * quote){
    int i = 0;
    while(i < strlen(quote)){
       if (quote[i] == '\n')
           break;
       ++i;
    }
    char * sub;
    sub = (char * )malloc((i+1)*sizeof(char));
    memcpy(sub, quote, i);
    return sub;
}

int util_is_file_exist(char * abs_path){
    struct stat f_stat;
    if (stat(abs_path, &f_stat) <0){
        return 0;
    }
    if (!(f_stat.st_mode & S_IROTH)){
        return 3;
    }
    if (S_ISREG(f_stat.st_mode)){
        return 1;
    }
    if (S_ISDIR(f_stat.st_mode)){
        return 2;
    }
    return 0;
}

int util_parse_http_request(struct serv_request * sreq){
    // split
    char * req_args[3];
    char * pch;
    pch  = strtok(sreq->full_content, " ");
    int i = 0;
    while (pch != NULL && i<3){
        req_args[i++] = pch;
        pch = strtok(NULL, " ");
    }
    
    // GET or HEAD
    if (strcmp(req_args[0], "GET")==0){
        sreq->mode = MODE_GET;
    } else if(strcmp(req_args[1], "HEAD") == 0){
        sreq->mode = MODE_HEAD;
    } else {
        return 0;
    }

    // URL
    if (req_args[1][0]!='/'){
        return 0;
    } else {
        sreq->path = req_args[1];
    }

    // HTTP/1.x or 0.9
    if (strlen(req_args[2])>=8){
        char subbuff[7];
        memcpy(subbuff, req_args[2], 7);
        if (strcmp(subbuff, "HTTP/1.")!=0 && strcmp(subbuff, "HTTP/0.")!=0){
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

struct serv_reply * util_get_response(struct serv_request * sreq){
    struct serv_reply * srpy;
    srpy = (struct serv_reply *)malloc(sizeof(struct serv_reply));
    char * _tmp_body;
    char * _tmp_head_stat;
    char * _tmp_head_date;
    char * _tmp_head_lm;
    char * _tmp_head_ct = "text/html";

    // determine the reponse type (INVLD, FLIST, EXACT, DFIDX, FRBID)
    long len;
    char * abs_path = util_get_abs_path(sreq->path);
    char * idx_path;
    char * flist;
    int f_status = util_is_file_exist(abs_path);
    int i_status;

    if (f_status == 2){ 
        idx_path = util_get_idx_path(abs_path);
        i_status = util_is_file_exist(idx_path);
        if (i_status == 0){
            if (sreq->mode == MODE_GET){_tmp_body = util_get_flist(abs_path);}
            srpy->status_code = 200;
            _tmp_head_stat = (char *)malloc(strlen(HTTP_STAT_200));
            strcpy(_tmp_head_stat, HTTP_STAT_200);
        }
        if (i_status == 3){
            if (sreq->mode == MODE_GET){
                _tmp_body = (char *)malloc(strlen(ENTITY_BODY_403)*sizeof(char));
                strcpy(_tmp_body, ENTITY_BODY_403);
            }
            srpy->status_code = 403;
            _tmp_head_stat = (char *)malloc(strlen(ENTITY_BODY_403));
            strcpy(_tmp_head_stat, HTTP_STAT_403);
        }
        if (i_status == 1){
            if (sreq->mode == MODE_GET){
                _tmp_body = util_get_file_content(idx_path);
            }
            _tmp_head_stat = (char *)malloc(strlen(HTTP_STAT_200));
            strcpy(_tmp_head_stat, HTTP_STAT_200);
        }
        free(idx_path);
    } else {
        if (f_status == 0){
            if (sreq->mode == MODE_GET){
                _tmp_body = (char *)malloc(strlen(ENTITY_BODY_404)*sizeof(char));
                strcpy(_tmp_body, ENTITY_BODY_404);
            }
            _tmp_head_stat = (char *)malloc(strlen(ENTITY_BODY_404));
            strcpy(_tmp_head_stat, HTTP_STAT_404);
        }
        if (f_status == 3){
            if (sreq->mode == MODE_GET){
                _tmp_body = (char *)malloc(strlen(ENTITY_BODY_403)*sizeof(char));
                strcpy(_tmp_body, ENTITY_BODY_403);
            }
            _tmp_head_stat = (char *)malloc(strlen(ENTITY_BODY_403));
            strcpy(_tmp_head_stat, HTTP_STAT_403);
        }
        if (f_status == 1){
            if (sreq->mode == MODE_GET){_tmp_body = util_get_file_content(abs_path);}
            _tmp_head_stat = (char *)malloc(strlen(HTTP_STAT_200));
            strcpy(_tmp_head_stat, HTTP_STAT_200);
        }
    }
    srpy->content_len = 0;
    if (sreq->mode == MODE_GET){srpy->content_len = strlen(_tmp_body);}
   
    time_t _tmp_time_current;
    time_t * _tmp_time_lm;
    _tmp_time_current = util_get_current_time(); 
    _tmp_head_date = util_get_time_str(&_tmp_time_current);
    _tmp_time_lm = util_get_file_timestamp(abs_path);
    _tmp_head_lm = util_get_time_str(_tmp_time_lm);
    free(_tmp_time_lm);

    // write to full_content;
    int _size_of_content = (1024+strlen(_tmp_head_stat)+strlen(_tmp_head_date)+strlen(SERVER_IDTIFIER)+strlen(_tmp_head_lm)+strlen(_tmp_head_ct)+srpy->content_len);
    srpy->full_content = (char *)malloc(_size_of_content*sizeof(char));
    sprintf(srpy->full_content, "HTTP/1.0 %s\r\nDate:%s\nServer:%s\nLast-Modified:%s\nContent-Type:%s\nContent-Length%ld\n\r\n",_tmp_head_stat,_tmp_head_date,SERVER_IDTIFIER,_tmp_head_lm,_tmp_head_ct,srpy->content_len);
    if (sreq->mode == MODE_GET){
        strcat(srpy->full_content, _tmp_body);
    }

    // clean up
    if (sreq->mode == MODE_GET){free(_tmp_body);}
    free(abs_path);
    free(_tmp_head_stat);
    free(_tmp_head_date);
    free(_tmp_head_lm);
    return srpy;
}
