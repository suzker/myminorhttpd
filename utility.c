#include "utility.h"
const char * ENTITY_BODY_404 = "Err 404: The Content Requested Not Found. (and everybody hates me... T_T)\n";
const char * ENTITY_BODY_403 = "Err 403: Permission Denied on The Content Requested.\n";

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
    arg_listen_port = 8080;
    strcpy(arg_root_folder, "./www/");
    arg_queue_time = 60;
    arg_thread_num = 1;
    arg_schedule_mode = 0;
}

int log_to_file(char remote_ip_addr[], time_t *time_queued, time_t *time_exec, char quote[], int *status, int *response_length){
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
    fprintf(pFile, "%s - [%02d/%s/%d:%02d:%02d:%02d - 0000] [%02d/%s/%d:%02d:%02d:%02d - 0000] \"%s\" %d %d", remote_ip_addr, ptm_q->tm_mday, monstr_q, 1900+ptm_q->tm_year, ptm_q->tm_hour, ptm_q->tm_min, ptm_q->tm_sec, ptm_e->tm_mday, monstr_e, 1900+ptm_e->tm_year, ptm_e->tm_hour, ptm_e->tm_min, ptm_e->tm_sec, quote, *status, *response_length);
    fclose(pFile);
}

char* _int_mon2str_(int * monint){
    char * mon_set[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    return mon_set[*monint];
}    

long util_get_req_len(char * path){
    char * abs_path;
    switch(get_response_type(path)){
        case INVLD:
            return -1;
            break;
        case FRBID:
            return 1; 
            break;
    }

    return 0;
}

char * util_get_response(char * path){
    // TODO TBA
}

enum RESP_TYPE get_response_type(char * path){
    char * abs_path;
    char * idx_path;
    abs_path = (char *)malloc((strlen(arg_root_folder)+strlen(path))*sizeof(char));
    idx_path = (char *)malloc((strlen(path)+15)*sizeof(char));
    strcpy(abs_path, arg_root_folder);
    strcat(abs_path, path);
    strcpy(idx_path, path);
    strcat(idx_path, "index.html");

    printf("input path: %s;\nabs_path: %s;\nidx_path: %s;\n", path, abs_path, idx_path);

    struct stat file_stat;
    if (stat(abs_path, &file_stat) < 0){
        return INVLD;
    }

    if (!(file_stat.st_mode & S_IROTH)){
        return FRBID;
    }   

    if (S_ISDIR(file_stat.st_mode)){
        if (get_response_type(idx_path) == EXACT){
            return DFIDX;
        } else {
            return FLIST;
        }
    }

    if (S_ISREG(file_stat.st_mode)){
        return EXACT;
    }

    return FRBID;
}
