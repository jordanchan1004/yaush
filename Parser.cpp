#include "Parser.h"

int parser(list<char*>* word_list, list<Command*>* command_list)
{
	list<char*>::iterator iter = word_list->begin();
	list<char*> temp_list;
	Command* command = NULL;
	int num;
	char* temp_string;

	command_list->clear();// clear the command list

	while(iter != word_list->end())// till the end of word list
	{
		if((strcmp(*iter, "<") == 0) || (strcmp(*iter, ">") == 0) || (strcmp(*iter, "|") == 0))// start with '<' or '>' or '|' is wrong
		{
			return(1);
		}
		command = new Command;
		if(command == NULL)
			return(-1);
		if(command_list->empty())// first command, input is stdin
		{
			if(str_copy(command->input, "stdin") != 0)
				return(-1);
		}
		else// not first command, input is pipe
		{
			if(str_copy(command->input, "pipe") != 0)
				return(-1);
		}
		if(str_copy(command->output, "stdout") != 0)
			return(-1);
		if(str_copy(command->name, *iter) != 0)// copy to command name
			return(-1);
		if(str_copy(temp_string, *iter) != 0)// copy command name to temp_string and push into temp_list
			return(-1);
		temp_list.push_back(temp_string);
		iter++;
		for(;iter!=word_list->end();iter++)// work until the end of word list
		{
			if(strcmp(*iter, "<") == 0)// reopen input
			{
				iter++;
				if((iter == word_list->end()) || (strcmp(*iter, "|") == 0))// must have input name(not "|") next to '<'
					return(1);
				if(str_copy(command->input, *iter) != 0)
					return(-1);
			}
			else if(strcmp(*iter, ">") == 0)// reopen output
			{
				iter++;
				if((iter == word_list->end()) || (strcmp(*iter, "|") == 0))// must have output name(not "|") next to '>'
					return(1);
				if(str_copy(command->output, *iter) != 0)
					return(-1);
			}
			else if(strcmp(*iter, "|") == 0)// pipe, the end of the command
			{
				if(str_copy(command->output, "pipe") != 0)
					return(-1);
				iter++;
				break;// end of command, so break
			}
			else// coefficients
			{
				if(str_copy(temp_string, *iter) != 0)// copy to temp_string and push_back to temp_list
					return(-1);
				temp_list.push_back(temp_string);
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
		command_list->push_back(command);// push the command to the command list
		log_debug("A command is added to command list!");

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
