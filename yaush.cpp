#include "yaush.h"

int main()
{
	int flag;
	custom_command_init();
	bind_shortcut();

	if(sigsetjmp(jmpbuf_ctrlz,1) != 0)
	{
	}
	if(sigsetjmp(jmpbuf_ctrlc,1) != 0)
	{
	}

	for(;;)
	{
		line_read = rl_gets();
		if(!line_read)
		{
			printf("\n");
			break;
		}
		else if(*line_read)
		{
			flag = lexer();
			if(flag == 0)
			{
				flag = parser();
				if(flag == 0)
				{
					flag = custom_command();
					if(flag == -1)// not find custom command
					{
						flag = cmd_execute();
					}
					else if(flag == 1)// exit
					{
						check_bg_list();
						break;
					}
					check_bg_list();
				}
			}
		}
	}
	return(0);
}
