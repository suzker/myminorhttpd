#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

int main(int argc, char *argv[]){
    if (!arg_parser(argc, argv)){
        printf("GOOD BYE CRUEL WORLD! :'( \n");
    }
    else {
        printf("debug mode on: %d\n", arg_debug_mode);
        printf("display sum when terminate: %d\n", arg_usage_sum);
        printf("path to save log file: %s\n", arg_log_file);
        printf("port listening: %d\n", arg_listen_port);
        printf("root folder: %s\n", arg_root_folder);
        printf("queueing time: %d\n", arg_queue_time);
        printf("number of threads: %d\n", arg_thread_num);
        printf("schedule mode: %d\n", arg_schedule_mode);
    }
    return 0;
}
