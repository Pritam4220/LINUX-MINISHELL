#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


typedef struct signal_data
{
    char command[100];
    pid_t child_pid;
    char status[20];
    struct signal_data *next;
}Sig_action;


#define BUILTIN         1
#define EXTERNAL        2
#define NO_COMMAND      3

#define SUCCESS 1
#define FAILURE 0

#define SHELL_TERMINAL   0

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);
void sig_handler(int signum);
void extract_external_commands(char **external_commands);
int n_pipe(char **cmd_tok);

// job control function prototypes
int insert_at_first(Sig_action **head, char *input_string, pid_t child_p);
int delete_at_first(Sig_action **head);
void run_job_foreground();
void run_job_background();
void open_file_read_status(pid_t pid, Sig_action **job);

#endif
