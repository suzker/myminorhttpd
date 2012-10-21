#include "utility.h"

const int RESP_TYPE_404 = 0;
const int RESP_TYPE_FLIST = 1;
const int RESP_TYPE_EXACT = 2;

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
    char monstr_q[5], monstr_e[5];
    _int_mon2str_(&(ptm_q->tm_mon), monstr_q);
    _int_mon2str_(&(ptm_e->tm_mon), monstr_e);
    fprintf(pFile, "%s - [%02d/%s/%d:%02d:%02d:%02d - 0000] [%02d/%s/%d:%02d:%02d:%02d - 0000] \"%s\" %d %d", remote_ip_addr, ptm_q->tm_mday, monstr_q, 1900+ptm_q->tm_year, ptm_q->tm_hour, ptm_q->tm_min, ptm_q->tm_sec, ptm_e->tm_mday, monstr_e, 1900+ptm_e->tm_year, ptm_e->tm_hour, ptm_e->tm_min, ptm_e->tm_sec, quote, *status, *response_length);
    fclose(pFile);
}

void _int_mon2str_(int * monint, char monstr[]){
   switch (*monint){
       case 0:
            strcpy(monstr, "Jan");
            break;
       case 1:
            strcpy(monstr, "Feb");
            break;
       case 2:
            strcpy(monstr, "Mar");
            break;
       case 3:
            strcpy(monstr, "Apr");
            break;
       case 4:
            strcpy(monstr, "May");
            break;
       case 5:
            strcpy(monstr, "Jun");
            break;
       case 6:
            strcpy(monstr, "Jul");
            break;
       case 7:
            strcpy(monstr, "Aug");
            break;
       case 8:
            strcpy(monstr, "Sep");
            break;
       case 9:
            strcpy(monstr, "Oct");
            break;
       case 10:
            strcpy(monstr, "Nov");
            break;
       case 11:
            strcpy(monstr, "Dec");
            break;
       default:
            strcpy(monstr, "n/a");
            break;
   } 
   return;
}

long util_get_req_len(char * req_path){
    // TODO TBA
}

char * util_get_response(char * req_path){
    // TODO TBA
}

char * get_response_type(char * req_path){
    // TODO TBA
}
