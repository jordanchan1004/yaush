#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <list>
#include <string.h>
#include <stdio.h>
#include "LogDebug.h"

using namespace std;

class Command
{
public:
	Command()
	{
		name = NULL;
		coeff_num = 0;
		coeff_list = NULL;
		input = NULL;
		output = NULL;
	};
	~Command()
	{
		if(output != NULL)
		{
			delete[] output;
			output = NULL;
		}
		if(input != NULL)
		{
			delete[] input;
			input = NULL;
		}
		if(coeff_list != NULL)
		{
			for(int i=0;i<coeff_num;i++)
			{
				if(coeff_list[i]!=NULL)
				{
					delete[] (coeff_list[i]);
					coeff_list[i] = NULL;
				}
			}
			delete[] coeff_list;
		}
		if(name != NULL)
		{
			delete[] name;
			name = NULL;
		}
	};
	char* name;// command name
	int coeff_num;// coefficient number
	char** coeff_list;// coefficient list
	char* input;// input name, can be "stdin" or "pipe" or "*" (* is name of a file)
	char* output;// output name, can be "stdout" or "pipe" or "*" (* is name of a file)
};

int parser(list<char*>* word_list, list<Command*>* command_list);

int str_copy(char* &dst, const char* src);

#endif
