#include "main.h"

extern Sig_action *head;
extern int status;

//in this function i implement here i store the job by using single linked list
int insert_at_first(Sig_action **head, char *input_string, pid_t c_pid)
{
	Sig_action *new = malloc(sizeof(Sig_action));

	if(new == NULL)
	{
		return FAILURE;
	}

	strcpy(new->command, input_string);
	new->child_pid = c_pid;

	new->next = *head;
	strcpy(new->status, "Stop");

	*head = new;

	return SUCCESS;
}

//here i use this because if all jobs are ccompleted then after that we have to delete
int delete_at_first(Sig_action **head)
{
	if(*head == NULL)
	{
		return FAILURE;
	}
	Sig_action *job = *head;

	*head = (*head)->next;

	free(job);

	return SUCCESS;
}

//Here this function is for check whatever jobs are running or sleep in background
void run_job_background()
{
	Sig_action *job = head;
	if(job)
	{
		printf("%s \n",job->command);
		kill(job->child_pid, SIGCONT);
		signal(SIGCHLD, sig_handler);

		delete_at_first(&head);
	}
	else
	{
		printf("bg : current: No such job\n");
	}
}

//here this function is for check the foreground
void run_job_foreground()
{
	Sig_action *job = head;

	if(job)
	{
		kill(job->child_pid, SIGCONT);

		printf("%s\n",job->command);
		waitpid(job->child_pid, NULL, WUNTRACED);
		delete_at_first(&head);
	}
	else
	{
		printf("bg : current: No such job\n");
	}
}
