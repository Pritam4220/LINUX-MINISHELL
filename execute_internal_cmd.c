#include <stdio.h>
#include "main.h"

extern int status;
extern Sig_action *head;

void execute_internal_commands(char *input_string)
{
        if(strcmp(input_string, "exit") == 0)
        {
                exit(0);
        }
        else if(strcmp(input_string,"pwd") == 0)
        {
                char buff[100];
                getcwd(buff,sizeof(buff));
                printf("%s\n",buff);
        }
        else if(strncmp(input_string,"cd ",3) == 0)
        {
                int ret = chdir(input_string+3);
                if(ret == -1)
                {
                        printf("It is an Invalid path\n");
                }
        }
        else if(strncmp(input_string,"echo ",3) == 0)
        {
                echo(input_string,status);
        }
        else if(!strcmp(input_string,"clear"))
        {
                system("clear");
        }
        else if(!strcmp(input_string, "bg"))            // background job control
        {
                //printf("yoooo\n");
                run_job_background();
        }
        else if(!strcmp(input_string, "fg"))            // foreground job control
        {
                run_job_foreground();
        }
        else if(!strcmp(input_string, "jobs"))          // list of all jobs
        {
                Sig_action *job = head;
                int si_no = 0;

                while(job != NULL)
                {
                        // printing the job info
                        printf("[%d]  %-15s  %-15s  %-15d\n", ++si_no, job->status, job->command, job->child_pid);
                        // update the pointer to the next job
                        job = (Sig_action *)job->next;  // Cast added here
                }
        }
}

void echo(char *input_string,int status)
{
        if(strcmp(input_string,"echo $$") == 0)
        {
                printf("PID is %d\n",getpid());
        }
        else if(strcmp(input_string, "echo $?") == 0)
        {
                printf("Exit code is %d\n",status);
        }
        else if(strcmp(input_string, "echo $SHELL") == 0)
        {
                char *ptr = getenv("SHELL");
                printf("%s\n",ptr);
        }
        else
        {
                printf("Invalid command\n");
        }
}

