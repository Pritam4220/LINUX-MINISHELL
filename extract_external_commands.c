#include <stdio.h>
#include <string.h>
#include "main.h"

void extract_external_commands(char **external_commands)
{
	int fd = open("external.txt", O_RDONLY);
	if(fd == -1)
	{
		perror("Failed");
		exit(1);
	}
	char ch;
	char buf[500];
	int i=0,index=0;
	while((read(fd, &ch,1)) > 0)
	{
		if(ch != '\n')
		{
			buf[i] = ch;
			i++;
		}
		else
		{
			buf[i] = '\0';
			external_commands[index] = malloc(20);
			strcpy(external_commands[index],buf);
			index++;
			i=0;
			memset(buf,0,sizeof(buf));
		}

		external_commands[index] = NULL;
	}
	close(fd);
}


