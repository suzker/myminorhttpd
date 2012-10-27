#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "utility.h"
#include "scheduler.h"
#include "server.h"

int main(int argc, char *argv[]){
    if (!arg_parser(argc, argv)){
        util_print_help();
        return 0;
    }
    if (arg_usage_sum == 1){
        util_print_help();
        return 0;
    }
    if (arg_debug_mode == 1){ // debug mode...
        char cmd_input[1024];
        char cmd_bye[] = "bye";
        printf("[DB] Entering Debug mode.\n");
        serv_init();
        printf("[DB] Server initialized, type 'bye' to terminate server.\n");
        do {
            printf ("[DB] Wrong command, type 'bye' to terminate the server.\n");
            gets (cmd_input);
        } while (strcmp (cmd_input, cmd_bye) != 0);
        return 0;
    }
    else{ // daemond mode
        pid_t pid = 0;
        pid_t sid = 0;
        pid = fork();
        if (pid < 0){
            perror("Forked failed, failed to enter the daemond mode, exiting...\n");
            exit(1);
        }
        if (pid > 0){ // the parent will see this
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
