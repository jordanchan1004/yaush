#include "yaush.h"

int main()
{
	char* line_read;
	list<char*> word_list;
	list<Command*> command_list;
	Job fg_job;
	list<Job*> bg_jobs;

	int flag;

	for(;;)
	{
		line_read = rl_gets();
		if(!line_read)
		{
			printf("\n");
			break;
		}
		else
		{
			flag = lexer(line_read, &word_list);
			if(flag == 0)
			{
				flag = parser(&word_list, &command_list);
				if(flag == 0)
				{
					flag = cmd_execute(&command_list, line_read, &fg_job, &bg_jobs);
				}
			}
		}
	}
	return(0);
}
