#ifndef DEFINITION_H
#define DEFINITION_H

#include <list>

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
		bl_background = false;
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
	bool bl_background;// whether background, 0 means not, 1 means yes
};

typedef enum _JobStatus
{
	Running = 0,
	Stopped = 1,
	Finished = 2
} JobStatus;

class Job
{
public:
	Job()
	{
		status = Running;
		content = NULL;
	};
	~Job()
	{
		if(content!=NULL)
		{
			delete[] content;
			content = NULL;
		}
	};
	list<int> pid_list;
	int status;
	char* content;
};

int str_copy(char* &dst, const char* src);

#endif
