#include "main.h"

extern char input_string[100];
extern char prompt[100];
extern char path[100];
extern char *home_path;
extern pid_t child_p;
Sig_action *head = NULL;
extern int status;

//here i create my own signal handler to handle my all signal
void sig_handler(int signum)
{
	if(!strlen(input_string) && (signum == SIGTSTP || signum == SIGINT))
	{
		printf("\n");
		printf(ANSI_COLOR_GREEN" %s: "ANSI_COLOR_RESET,prompt);
		printf(ANSI_COLOR_BLUE"~%s$ "ANSI_COLOR_GREEN,path + strlen(home_path));
		fflush(stdout);
	}
	else if(signum == SIGTSTP)
	{
		insert_at_first(&head, input_string, child_p);
		kill(child_p, SIGTSTP);
	}
	else if(signum == SIGCHLD)
	{
		waitpid(-1,&status,WNOHANG);
		signal(SIGCHLD, sig_handler);
	}
}

