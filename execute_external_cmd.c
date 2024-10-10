#include <stdio.h>
#include "main.h"
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


// Name: Pritam kumar
char *cmd_tok[20];
void execute_external_commands(char *input_string)
{
	char *string = malloc(strlen(input_string)+1);
	strcpy(string,input_string);

	char *token = strtok(string, " ");
	int i=0;

	while(token != NULL && i<20)
	{
		cmd_tok[i] = malloc(20);
		strcpy(cmd_tok[i],token);
		token = strtok(NULL, " ");
		i++;
	}
	cmd_tok[i] = NULL;

	int j=0;
	int pipe=0,Nopipe=0;
	while(cmd_tok[j] != NULL)
	{
		if(strcmp(cmd_tok[j], "|") == 0)
		{
			pipe=1;
			break;
		}
		j++;
	}
	if(pipe)
	{
		n_pipe(cmd_tok);
		fflush(stdout);
	}
	else
	{
		execvp(cmd_tok[0], cmd_tok+0);
		//fflush(stdout);

	}
	free(string);
}

int n_pipe(char **cmd_tok)
{
	//here i store the command index into an array and that '|' = NULL
	int cmd=0,cmd_count=1,index=1;
	int cmd_arr[25] = {0};
	while(cmd_tok[cmd] != NULL)
	{
		if(strcmp(cmd_tok[cmd], "|") == 0)
		{
			cmd_arr[index++] = cmd + 1;
			cmd_tok[cmd] = NULL;
			cmd_count++;
		}
		cmd++;
	}

	//creating pipe
	int fd[2];

	for(int i=0; i < cmd_count; i++)
	{
		//this condition is for because at last iteration there is no need to create pipe
		if(i < cmd_count - 1)
		{
			if(pipe(fd) == -1)
			{
				perror("pipe");
				return -1;
			}
		}

		pid_t pid = fork();

		if(pid > 0)      //parent
		{

			close(fd[1]);
			dup2(fd[0],0);
			close(fd[0]);
		}
		//child
		else if(pid == 0)
		{
			if(i < cmd_count - 1)
			{
				close(fd[0]);
				dup2(fd[1],1);
			}
			execvp(cmd_tok[cmd_arr[i]],cmd_tok+cmd_arr[i]);

		}
		else
		{
			perror("fork");
		}
	}
}


