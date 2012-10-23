#include "utility.h"
char * ENTITY_BODY_404 = "Err 404: The Content Requested Not Found. (and everybody hates me... T_T)\n";
char * ENTITY_BODY_403 = "Err 403: Permission Denied on The Content Requested.\n";
const char * SERVER_IDTIFIER = "MINOR-HTTPD BUFFALO";

int arg_debug_mode;
int arg_usage_sum;
char arg_log_file[1000];
int arg_listen_port;
char arg_root_folder[1000];
int arg_queue_time;
int arg_thread_num;
int arg_schedule_mode;

const int _MODE_GET = 1;
const int _MODE_HEAD = 2;

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
    arg_listen_port = 8080;
    strcpy(arg_root_folder, "./www/");
    arg_queue_time = 60;
    arg_thread_num = 1;
    arg_schedule_mode = 0;
}

int log_to_file(char remote_ip_addr[], time_t *time_queued, time_t *time_exec, char quote[], int status, long response_length){
    if (!strcmp(arg_log_file, "N/A")){return 0;}
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
    char * abs_path;
    abs_path = _get_abs_path_(path);
    char * idx_path;
    idx_path = _get_idx_path_(abs_path);
    switch(get_response_type(path)){
        case INVLD:
            len = strlen(ENTITY_BODY_404);
            break;
        case FRBID:
            len = strlen(ENTITY_BODY_403);
            break;
        case FLIST:
            len = strlen(_get_flist_(abs_path));
            break;
        case EXACT:
            len = _get_file_len(abs_path);
            break;
        case DFIDX:
            len = _get_file_len(idx_path);
            break;
        default:
            len = 0;
            break;
    }
    free(abs_path);
    free(idx_path);
    return len;
}

int util_get_response(char * path, int REQ_MODE, char * resp_str){
    char * abs_path;
    abs_path = _get_abs_path_(path);
    char * idx_path;
    idx_path = _get_idx_path_(abs_path);
    enum RESP_TYPE t = get_response_type(path);
    char * header_str;
    header_str = (char *)malloc(1566*sizeof(char));
    long content_len = util_get_req_len(path);
    int status_code = _get_resp_header_(t, header_str, abs_path, content_len);
    strcpy(resp_str, header_str);
    if (REQ_MODE == _MODE_GET){
        switch(t){
            case INVLD:
                strcat(resp_str, ENTITY_BODY_404);
                break;
            case FRBID:
                strcat(resp_str, ENTITY_BODY_403);
                break;
            case FLIST:
                strcat(resp_str, _get_flist_(abs_path));
                break;
            case EXACT:
                strcat(resp_str, _get_file_content_(abs_path));
                break;
            case DFIDX:
                strcat(resp_str, _get_file_content_(idx_path));
                break;
            default:
                strcat(resp_str, ENTITY_BODY_404);
                break;
            }
     }
     
    free(abs_path);
    free(idx_path);
    free(header_str);
    return status_code;
}

enum RESP_TYPE get_response_type(char * path){
    char * abs_path;
    abs_path = _get_abs_path_(path);
    char * idx_path;
    idx_path = (char *)malloc((strlen(path)+15)*sizeof(char));
    strcpy(idx_path, path);
    strcat(idx_path, "index.html");

    struct stat file_stat;
    if (stat(abs_path, &file_stat) < 0){
        free(abs_path);
        free(idx_path);
        return INVLD;
    }
    free(abs_path);

    if (!(file_stat.st_mode & S_IROTH)){
        free(idx_path);
        return FRBID;
    }

    if (S_ISDIR(file_stat.st_mode)){
        if (get_response_type(idx_path) == EXACT){
            free(idx_path);
            return DFIDX;
        } else {
            free(idx_path);
            return FLIST;
        }
    }
    free(idx_path); 
    if (S_ISREG(file_stat.st_mode)){
        return EXACT;
    }

    return FRBID;
}

void print_help(){
    // TODO tba
}

char * _get_flist_(char * dir_path){
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
    return buff;
}

long _get_file_len(char * file_path){
    struct stat file_stat;
    if (stat(file_path, &file_stat) < 0){
        return -1;
    }
    return file_stat.st_size;
}

char * _get_abs_path_(char * rel_path){
    char * abs_path;
    abs_path = (char *)malloc((strlen(arg_root_folder)+strlen(rel_path))*sizeof(char));
    strcpy(abs_path, arg_root_folder);
    strcat(abs_path, rel_path);
    return abs_path;
}

char * _get_idx_path_ (char * abs_path){
    char * idx_path;
    idx_path = (char * )malloc((strlen(abs_path)+strlen("index.html"))*sizeof(char));
    strcpy(idx_path, abs_path);
    strcat(idx_path, "index.html");
    return idx_path;
}

char * _get_file_content_(char * file_path){
    char * buffer = 0;
    long length;
    FILE * f = fopen (file_path, "rb");

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

int _get_resp_header_(enum RESP_TYPE t, char * header_str, char * abs_path, long content_len){
    int status_code;
    char * current_time;
    time_t * _tmp_t;
    _tmp_t = _get_current_time_();
    current_time = (char *)_get_time_str_(_tmp_t);
    char * last_mod_time;
    switch (t){
        case FRBID:
            status_code = 403;
            break;
        case INVLD:
            status_code = 404;
            break;
        case FLIST:
            status_code = 200;
            break;
        case EXACT:
            status_code = 200;
            _tmp_t = _get_file_timestamp_(abs_path);
            break;
        case DFIDX:
            status_code = 200;
            _tmp_t = _get_file_timestamp_(abs_path);
            break;
        default:
            status_code = 404;
            break;
    }

    last_mod_time = _get_time_str_(_tmp_t);
    sprintf(header_str, "HTTP/1.0 %d\r\n\tDate:%s\tServer:%s\tLast-Modified:%s\tContent-Type:text/html\tContent-Length:%ld\r\n", status_code,current_time,SERVER_IDTIFIER,last_mod_time,content_len);

    free(_tmp_t);
    free(current_time);
    free(last_mod_time);
    return status_code;
}

time_t * _get_current_time_(){
    time_t * raw_t;
    raw_t = (time_t *)malloc(sizeof(time_t));
    time(raw_t);
    return raw_t;
}

char * _get_time_str_(time_t * raw_t){
    char * time_str;
    time_str = (char *)malloc(128*sizeof(char));
    struct tm * ptm;
    ptm = gmtime(raw_t);
    sprintf(time_str, "%s, %02d %s %04d %02d:%02d:%02d GMT", _int_weekdays_(&(ptm->tm_wday)), ptm->tm_mday, _int_mon2str_(&(ptm->tm_mon)), 1900+ptm->tm_year, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    free(ptm);
    free(raw_t);
    return time_str;
}

time_t * _get_file_timestamp_(char * file_path){
    time_t * timestamp;
    timestamp = (time_t*)malloc(sizeof(time_t));
    struct stat file_stat;
    if (stat(file_path, &file_stat) < 0){
        return NULL;
    }
    *timestamp = file_stat.st_mtime;
    return timestamp;
}

char * _get_first_line_(char * quote){
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
