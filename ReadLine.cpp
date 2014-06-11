#include "ReadLine.h"

static char* line_read = NULL;

char* rl_gets()
{
	char shell_prompt[2000];
	if(line_read)
	{
		free(line_read);
		line_read = NULL;
	}

	snprintf(shell_prompt, sizeof(shell_prompt), "%s@yaush:%s$ ", getenv("USER"),getcwd(NULL,1000));

	line_read = readline(shell_prompt);

	if(line_read && *line_read)
	{
		add_history(line_read);
	}

	return(line_read);
}
