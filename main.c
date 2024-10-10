#include <stdio.h>
#include "main.h"
/*

Name: Pritam kumar
Date:
 */

char input_string[100];
char prompt[100] = "Minishell";
char *external_commands[200];
int status;
int prompt_flag = 1;
static int flag = 1;
char path[100];
char *home_path;
pid_t child_p;
int main()
{
	system("clear");
	//here i call scan function to print prompt
	scan_input(prompt,input_string);

	return 0;
}

void scan_input(char *prompt, char *input_string)
{
	//here i register the signal and assign this to sig_handler
	signal(SIGINT, sig_handler);
	signal(SIGTSTP, sig_handler);
	signal(SIGCHLD, sig_handler);

	if(flag)
	{
		//after that i extract external commands
		extract_external_commands(external_commands);
		flag = 0;
	}

	while(1)
	{
		//In this condition i print the prompt and i check some condition and print the prompt with current directory for that i use HOME in getenv
		if(prompt_flag)
		{
			//displaying prompt
			getcwd(path,sizeof(path));
			home_path = getenv("HOME");
			if(home_path != NULL && strstr(path,home_path) == path)
			{
				printf(ANSI_COLOR_GREEN" %s: "ANSI_COLOR_RESET, prompt);
				printf(ANSI_COLOR_BLUE"~%s$ "ANSI_COLOR_RESET,path + strlen(home_path));
			}
			else
			{
				printf(ANSI_COLOR_GREEN" %s: "ANSI_COLOR_RESET,prompt);
				printf(ANSI_COLOR_BLUE" %s$ "ANSI_COLOR_RESET,path);
			}
		}
		else
		{
			printf(ANSI_COLOR_RED" %s "ANSI_COLOR_RESET,prompt);
		}
		__fpurge(stdin);
		scanf("%[^\n]",input_string);
		getchar();

		if(!strlen(input_string))
		{
			continue;
		}

		//here i validating the PS1 to change the name of our minishell
		if(strncmp(input_string, "PS1=", 4) == 0 && input_string[4] != ' ')
		{
			strcpy(prompt, input_string+4);
			prompt_flag = 0;
		}
		else
		{
			//after that i called the getcommand and check command type function to which command is this 
			char *cmd = get_command(input_string);
			int ret = check_command_type(cmd);
			if(ret == EXTERNAL)
			{
				pid_t child = fork();
				child_p = child;

				if(child == 0)
				{
					//here i register the signal to default handler
					signal(SIGINT, SIG_DFL);
					signal(SIGTSTP, SIG_DFL);
					signal(SIGCHLD, SIG_DFL);

					execute_external_commands(input_string);
				}
				//in parent process it wait for child until its terminated
				else if(child > 0)
				{
					waitpid(child, &status, WUNTRACED);
				}
			}
			else if(ret == BUILTIN)
			{
				execute_internal_commands(input_string);
			}
			else
			{
				printf("Command not Found\n");
			}
		}
		//clearing input buffer
		memset(input_string,0,sizeof(input_string));
	}
}

//here function for getting command
char *get_command(char *input_string)
{
	char *ptr = malloc(strlen(input_string)+1);
	if(ptr == NULL)
	{
		perror("Memory allcoation failed");
		exit(1);
	}
	int i=0;
	while(input_string[i] != '\0')
	{
		if(input_string[i] == ' ')
		{
			break;
		}

		ptr[i] = input_string[i];
		i++;
	}
	ptr[i] = '\0';
	return ptr;
}
//here function for check command type whether it is external,bulitin
int check_command_type(char *cmd)
{
	//      printf("%s\n",cmd);
	char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
		"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source","clear",
		"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","fg","bg","jobs", NULL};
	int i=0;
	while(builtins[i] != NULL)
	{
		if(strcmp(builtins[i],cmd)==0)
		{
			return BUILTIN;
		}
		i++;
	}
	int j=0;
	while(external_commands[j] != NULL)
	{
		//printf("%s\n",external_commands[j]);
		if(strcmp(external_commands[j],cmd) == 0)
		{
			return EXTERNAL;
		}
		j++;
	}
	return NO_COMMAND;
}

