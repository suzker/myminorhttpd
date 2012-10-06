#include "utility.h"

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
    strcpy(arg_log_file, "./myhttpd.log");
    arg_listen_port = 8080;
    strcpy(arg_root_folder, "./www/");
    arg_queue_time = 60;
    arg_thread_num = 1;
    arg_schedule_mode = 0;
}

int log_to_file(char *remote_ip_addr[], ){

}
