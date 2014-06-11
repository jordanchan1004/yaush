#include "yaush.h"

int main()
{
	char* line_read;
	list<char*> word_list;
	list<Command*> command_list;
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
			lexer(line_read, &word_list);
			for(list<char*>::iterator iter=word_list.begin();iter!=word_list.end();iter++)
			{
				printf("%s\n",*iter);
			}
			flag = parser(&word_list, &command_list);
			if(flag == 0)
			{
				for(list<Command*>::iterator iter=command_list.begin();iter!=command_list.end();iter++)
				{
					printf("name: %s\n", (*iter)->name);
					printf("coeff num: %d\n", (*iter)->coeff_num);
					printf("coeff list:\t");
					for(int i=0;i<(*iter)->coeff_num;i++)
					{
						printf("%s\t",(*iter)->coeff_list[i]);
					}
					printf("\nin: %s\nout: %s\n\n",(*iter)->input,(*iter)->output);
				}
			}
			else
			{
				printf("Wrong input!\n");
			}
		}
	}
	return(0);
}
