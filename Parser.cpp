#include "Parser.h"

int parser()
{
	if(word_list.empty())// no words
	{
		command_list.clear();// clear and return
		return(-1);
	}

	list<char*>::iterator iter = word_list.begin();
	list<char*> temp_list;
	Command* command = NULL;
	int num;
	char* temp_string;

	command_list.clear();// clear the command list

	while(iter != word_list.end())// till the end of word list
	{
		// start with '<' or '>' or '|' or '&' is wrong
		if((strcmp(*iter, "<") == 0) || (strcmp(*iter, ">") == 0) || (strcmp(*iter, "|") == 0) || (strcmp(*iter, "&") == 0))
		{
			printf("Invalid command!\n");
			return(-1);
		}
		command = new Command;
		if(command == NULL)
			return(-1);
		if(command_list.empty())// first command, input is stdin
		{
			if(str_copy(command->input, "stdin") != 0)
				return(-1);
		}
		else// not first command, input is pipe
		{
			if(str_copy(command->input, "pipe") != 0)
				return(-1);
		}
		if(str_copy(command->output, "stdout") != 0)// output is stdout
			return(-1);
		if(str_copy(command->name, *iter) != 0)// copy to command name
			return(-1);
		if(str_copy(temp_string, *iter) != 0)// copy to temp_string and push into temp_list
			return(-1);
		temp_list.push_back(temp_string);
		iter++;
		while(iter != word_list.end())// work until the end of word list
		{
			if(strcmp(*iter, "<") == 0)// reopen input
			{
				iter++;
				// must have input name(not "|" or "&") next to '<'
				if((iter == word_list.end()) || (strcmp(*iter, "|") == 0) || (strcmp(*iter, "&") == 0))
				{
					printf("Invalid input file name!\n");
					return(-1);
				}
				if(str_copy(command->input, *iter) != 0)
					return(-1);
				iter++;
			}
			else if(strcmp(*iter, ">") == 0)// reopen output
			{
				iter++;
				// must have output name(not "|" or "&") next to '<'
				if((iter == word_list.end()) || (strcmp(*iter, "|") == 0) || (strcmp(*iter, "&") == 0))
				{
					printf("Invalid output file name!\n");
					return(-1);
				}
				if(str_copy(command->output, *iter) != 0)
					return(-1);
				iter++;
			}
			else if(strcmp(*iter, "|") == 0)// pipe, the end of the command
			{
				if(str_copy(command->output, "pipe") != 0)
					return(-1);
				iter++;
				if((iter == word_list.end()) || (strcmp(*iter, "&") !=0))// the end of word list or next is not "&"
				{
					break;// end of command, so break
				}
			}
			else if(strcmp(*iter, "&") == 0)// bg, the end of all commands
			{
				for(list<Command*>::iterator i=command_list.begin();i!=command_list.end();i++)
				{
					(*i)->bl_background = true;
					log_debug("%s is changed to %s", (*i)->name, (*i)->bl_background ? "background":"foreground");
				}
				command->bl_background = true;
				iter = word_list.end();
				break;
			}
			else// coefficients
			{
				if(str_copy(temp_string, *iter) != 0)// copy to temp_string and push_back to temp_list
					return(-1);
				temp_list.push_back(temp_string);
				iter++;
			}
		}
		num = temp_list.size();// count the number of coeff
		command->coeff_num = num;// set the number of command
		command->coeff_list = new char*[num + 1];// new coeff_list
		int i = 0;
		for(list<char*>::iterator j=temp_list.begin();j!=temp_list.end();i++,j++)// copy each coeff to coeff_list
		{
			if(str_copy(command->coeff_list[i], *j) != 0)
				return(-1);
		}
		command->coeff_list[num] = NULL;// set the end of coeff_list to NULL
		command_list.push_back(command);// push the command to the command list

		// log print
		log_debug("A command is added to command list!");
		log_debug("Command name is: %s", command->name);
		log_debug("Command coeffs:");
		for(int i=0;i<command->coeff_num;i++)
			log_debug("%s",command->coeff_list[i]);
		log_debug("Input is: %s", command->input);
		log_debug("Output is: %s", command->output);
		log_debug("Background: %s", command->bl_background ? "yes":"no");

		command = NULL;// set command to NULL
		temp_list.clear();// clear the temp_list
	}

	return(0);
}

int str_copy(char* &dst, const char* src)
{
	int len = strlen(src);
	dst = new char[len + 1];
	if(dst == NULL)
	{
		perror("Allocate char* failed");
		return(-1);
	}
	strncpy(dst, src, len);
	dst[len] = '\0';
	return(0);
}
