#ifndef _HEADER_UTILITY_
#define _HEADER_UTILITY_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "server.h"

extern int arg_debug_mode;     //  to enable single thread non-daemond mode (-d)
extern int arg_usage_sum;      //  to print usage summary in the end (-h)
extern char arg_log_file[1000];     //  path to store log file (-l)
extern int arg_listen_port;     //  port to bind (-p)
extern char arg_root_folder[1000];  //  folder path of the root (-r)
extern int arg_queue_time;      //  queing delay, in seconds (-t)
extern int arg_thread_num;      //  number of threads (-n)
extern int arg_schedule_mode;  //  scheduling mode, true for SJF, false for FIFO (-s)

enum RESP_TYPE{
    FRBID,
    FLIST,
    EXACT,
    DFIDX,
    INVLD
};

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
 function: util_log_to_file
  to log to a file if -l opt is enabled
  input:
    char * : the remote ip address
    time_t * : the time received by the queuing thread
    time_t * : the time being assign to an exec thread
    char * : the quote of first line from the request
    int * : the status code
    long * : the response length in byte
*/
int util_log_to_file(char remote_ip_addr[], time_t *time_queued, time_t *time_exec, char quote[], int status, long response_length);

/**
 function util_get_current_time
*/
time_t util_get_current_time();

/**
 function util_get_time_str
  to generate standard GMT time str.
  * note this function calls malloc inside, thus the return MUST be freed after used.
*/
char * util_get_time_str(time_t *);

/**
 function: _ini_mon2str_ (private)
  to convert the month into string
  input:
    int *: month in integer
*/
char * _int_mon2str_(int * monint);

/**
 function: _int_weekdays_
  to convert the day into weekdays string
*/
char * _int_weekdays_(int * dayint);

/**
 function: util_get_req_len
  to get the request length according to the file the request asking for.
   input:
        char *: the requesting path
   output:
        long : the length
*/
long util_get_req_len(char *);

/**
 function: util_print_help()
  RT
*/
void util_print_help();

/**
 function : util_get_flist
  get file list given the url to a dirctory
  * note this function WILL call malloc inside. free must be called to the return outside of the function.
*/
char * util_get_flist(char *);

/**
 function: util_get_file_len
  given a file path return the length of the file
*/
long util_get_file_len(char *);

/**
 function: util_get_abs_path(char * related_path)
 * note this function WILL call malloc inside. free must be called to the return outside of the function.
*/
char * util_get_abs_path(char * rel_path);

/**
 function: _get_idx_path_ (char *)
 * note this function WILL call malloc inside. free must be called to the return outside of the function.
*/
char * util_get_idx_path (char* abs_path);

/**
 function: _get_file_content_ (char *)
 * note this function WILL call malloc inside. free must be called to the return outside of the function.
*/
char * util_get_file_content(char * abs_path);

/**
 function: _get_file_timestamp_(char *)
  * will malloc new time_t in heap space and need to be freed somewhere else;
*/
time_t * util_get_file_timestamp(char * abs_path);

/**
 function: util_get_first_line(char *)
  * will malloc new char[] in heap space and need to be freed somewhere else;
*/
char * util_get_first_line(char *);

/**
 function: util_is_file_exist(char *)
  given a file abs_path, to see if the file exists
  return 0 for not exist, 1 for regular file(not link), 2 for directory, 3 for no permission;
*/
int util_is_file_exist(char * abs_path);

/**
 function: util_get_response
  to generate a response regarding to the input request
  ** note, this func DOES call malloc. a subsequent free to the return should be done outside of this function.
*/
struct serv_reply * util_get_response(struct serv_request * sreq);

/**
 function: util_parse_http_request(struct serv_request *)
  to parse a http request and return the validation as an int. also if the valid is true, the input serv_request object will be filled w/ req details
  ** note, this func DOES call malloc. a subsequent free to the return should be done outside of this function.
   input:
        struct serv_request *: the structure pointer to store the request details if the request is valid
   output:
        1 for valid and 0 for invalid
*/
int serv_parse_http_request(struct serv_request *);
#endif
