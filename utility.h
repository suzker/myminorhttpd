#ifndef _HEADER_UTILITY_
#define _HEADER_UTILITY_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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
 function: log_to_file
  to log to a file if -l opt is enabled
  input:
    char * : the remote ip address
    time_t * : the time received by the queuing thread
    time_t * : the time being assign to an exec thread
    char * : the quote of first line from the request
    int * : the status code
    long * : the response length in byte
*/
int log_to_file(char remote_ip_addr[], time_t *time_queued, time_t *time_exec, char quote[], int status, long response_length);

/**
 function _get_current_time_
*/
time_t * _get_current_time_();

/**
 function _get_time_str_
*/
char * _get_time_str_(time_t *);

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
 function: util_get_response(char *)
  to get an appropriate response for the input path (dirctory or file or root)
  *this function is also responsible for constructing appropriate HTTP response headers.
   input:
        char * : the requesting path
        char * : the pointer to the memory to store the response contents
   output:
        int : the status code;
*/
int util_get_response(char *, int, char *);

/**
 function: get_response_type(char *)
  given the input path (dirctory, file or root), to see if it's exist or not and and return the appropriate type (404, index.html, .flist, the_page).
    input:
         char * : the requesting path
    output:
         enum RESP_TYPE : the type
*/
enum RESP_TYPE get_response_type(char *);

/**
 function: print_help()
  RT
*/
void print_help();

/**
 function : _get_flist_
  get file list given the url to a dirctory
*/
char * _get_flist_(char *);

/**
 function: _get_file_len
  given a file path return the length of the file
*/
long _get_file_len(char *);

/**
 function: _get_abs_path_(char *)
*/
char * _get_abs_path_(char * path);

/**
 function: _get_idx_path_ (char *)
*/
char * _get_idx_path_ (char* abs_path);

/**
 function: _get_file_content_ (char *)
*/
char * _get_file_content_(char * file_path);

/**
 function: _get_resp_header_(enum RESP_TYPE, char *)
*/
int _get_resp_header_(enum RESP_TYPE, char * header_str, char * abs_path, long content_len);

/**
 function: _get_file_timestamp_(char *)
*/
time_t * _get_file_timestamp_(char * file_path);

/**
 function: _get_first_line_(char *)
*/
char * _get_first_line_(char *);
#endif
