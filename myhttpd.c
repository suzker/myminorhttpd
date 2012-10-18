#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "scheduler.h"

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

    // init scheduler
    scheduler_init();

    // insert jobs into the scheduler
    int *k;
    struct scheduler_job *job;
    int i;
    for (i=0; i<19; ++i){
        k = (int *)malloc(sizeof(int));
         *k = rand() % 30 + 1;
         job = (struct scheduler_job *)malloc(sizeof(struct scheduler_job));
         *job = scheduler_create_job(k, (long)(*k));
         scheduler_add_job(job);
    }
    printf("finished adding jobs into scheduler. \n");

    // the inserted jobs should be popped out automatically
    printf("waiting for output to be finished, sleep for 3 secs\n");
    sleep(3);

    scheduler_destroy();
    return 0;
}
