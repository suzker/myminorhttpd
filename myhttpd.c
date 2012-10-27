#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "utility.h"
#include "scheduler.h"
#include "server.h"

int main(int argc, char *argv[]){
    system("clear");
    if (!arg_parser(argc, argv)){
        util_print_help();
        return 0;
    }
    if (arg_usage_sum == 1){
        util_print_help();
        return 0;
    }
    if (arg_debug_mode == 1){ // debug mode...
        arg_thread_num = 1; // overwirtes the number of threads
        char cmd_input[1024];
        char cmd_bye[] = "bye";
        char cmd_clear[] = "clear";
        printf("[DB] Entering Debug mode.\n");
        serv_init();
        printf("[DB] Server initialized, type 'bye' to terminate server and 'clear' to purge screen outputs.\n");
        do {
            gets (cmd_input);
            printf ("\n");
            if (strcmp (cmd_input, cmd_bye) == 0)
                break;
            if (strcmp (cmd_input, cmd_clear) == 0)
                system(cmd_clear);
        } while (1);
        serv_destroy();
        return 0;
    }
    else{ // daemond mode
        printf("[myhttpd] server running on daemond mode ... ");
        pid_t pid = 0;
        pid_t sid = 0;
        pid = fork();
        if (pid < 0){
            perror("Forked failed, failed to enter the daemond mode, exiting...\n");
            exit(1);
        }
        if (pid > 0){ // the parent will see this
            printf("OK!\n[myhttpd] use 'ps -A | grep myhttpd' command to see the pid of the daemond. \n");
            exit(0);
        }
        umask(0);
        sid = setsid();
        if (sid < 0){
            perror("Failed to setup new session, exiting...\n");
            exit(1);
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        serv_init();
        while(1){
            sleep(3600);
        }
    }
}
