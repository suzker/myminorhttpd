#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ****************************
// static variables - arguments
// ****************************
int arg_debug_mode;     //  to enable single thread non-daemond mode (-d)
int arg_usage_sum;      //  to print usage summary in the end (-h)
char arg_log_file[1000];     //  path to store log file (-l)
int arg_listen_port;     //  port to bind (-p)
char arg_root_folder[1000];  //  folder path of the root (-r)
int arg_queue_time;      //  queing delay, in seconds (-t)
int arg_thread_num;      //  number of threads (-n)
int arg_schedule_mode;  //  scheduling mode, true for SJF, false for FIFO (-s)

/**
 function: arg_parser
  to parse the input arguments and assign related values to the static variables.
  input:
      int argc: number of arguments
      cahr * argv[]: pointer to the argument values
  output:
      bool: if parsing is successful or not
*/
int arg_parser(int argc, char * argv[]);

/**
 function: init_arg
  to initialize the arguments with default values
*/
void init_arg();

/**
 function: log_to_file
  to log to a file if -l opt is enabled
  input:
    char * : the remote ip address
    time_t * : the time received by the queuing thread
    time_t * : the time being assign to an exec thread
    char * : the quote of first line from the request
    int * : the status code
    int * : the response length in byte
*/
int log_to_file(char remote_ip_addr[], time_t *time_queued, time_t *time_exec, char quote[], int *status, int *response_length);

/**
 function: _ini_mon2str_
  to convert the month into string
  input:
    int *: month in integer
    cahr [] : char array to store result
*/
void _int_mon2str_(int * monint, char monstr[]);
