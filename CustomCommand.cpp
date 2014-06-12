#include "CustomCommand.h"

const char* custom_command_list[] = 
{
	"exit",
	"cd",
	"jobs",
	"fg",
//	"bg"
};

int (*fn[])(char**) = 
{
	custom_exit,
	custom_cd,
	custom_jobs,
	custom_fg
};

map<string, int> custom_command_map;

int custom_exit(char** argv)
{
	return(0);
}

int custom_cd(char** argv)
{
	char* path = argv[1];
	if(path == NULL)
		return(0);
	int r_val = chdir(path);
	if(r_val == -1)
	{
		perror("cd: ");
		return(-1);
	}
	else
	{
		return(0);
	}
}

int custom_jobs(char** argv)
{
	int cnt = 1;
	for(list<Job*>::iterator i=bg_jobs.begin();i!=bg_jobs.end();i++,cnt++)
	{
		char* temp;
		if((*i)->status == Running)
		{
			str_copy(temp, "Running");
		}
		else if((*i)->status == Stopped)
		{
			str_copy(temp, "Stopped");
		}
		else if((*i)->status == Finished)
		{
			str_copy(temp, "Finished");
		}
		else
		{
			str_copy(temp, "Unknown");
		}
		printf("[%d] %s\t%s\n", cnt, temp, (*i)->content);
		delete[] temp;
	}
	return(0);
}

int custom_fg(char** argv)
{
	char* idx_str = argv[1];
	if(idx_str == NULL)// no efficient
	{
		printf("fg: please specify number for fg\n");
		return(-1);
	}
	int idx = atoi(idx_str);
	if(idx <= 0)
	{
		printf("fg: number must be positive!\n");
		return(-1);
	}
	if(idx > (int)bg_jobs.size())// idx bigger than bg_jobs's size
	{
		printf("fg: number too big!\n");
		return(-1);
	}
	list<Job*>::iterator iter = bg_jobs.begin();
	for(int i=1;i<idx;i++,iter++);// move to the idx entry
	if((*iter)->status == Finished)// job finished, cannot fg
	{
		printf("fg: job %d is finished!\n", idx);
		return(-1);
	}
	else if(((*iter)->status == Running) || ((*iter)->status == Stopped))// job running or stopped
	{
		if((*iter)->status == Stopped)// job stopped, need to be resume
		{
			// To do
		}
		if(fg_job != NULL)
		{
			delete fg_job;
			fg_job = NULL;
		}
		fg_job = new Job;
		swap((*iter)->pid_list, fg_job->pid_list);
		str_copy(fg_job->content, (*iter)->content);
		bg_jobs.erase(iter);
		fg_job->status = Running;
		list<int>::iterator pid_temp;
		for(list<int>::iterator iter=(fg_job->pid_list).begin();iter!=(fg_job->pid_list).end();)// while pid list not empty
		{
			int pid=*iter, status, r_val;
			r_val = waitpid(pid,&status,0);// wait pid to exit
			if((r_val > 0) || ((r_val == -1) && (errno == ECHILD)))
			{
				pid_temp = iter;
				iter++;
				(fg_job->pid_list).erase(pid_temp);
			}
		}
		delete fg_job;
		fg_job = NULL;
		return(0);
	}
	else
	{
		return(-1);
	}
}

int custom_command_init()
{
	for(int i=0;i<CUSTOM_CMD_NUM;i++)
	{
		custom_command_map.insert(pair<string, int>(string(custom_command_list[i]), i));
	}
	return(0);
}

int custom_command()
{
	Command* first = *(command_list.begin());
	map<string, int>::iterator iter;
	iter = custom_command_map.find(first->name);
	if(iter == custom_command_map.end())
	{
		return(-1);
	}

	int (*f)(char**) = fn[iter->second];
	f(first->coeff_list);

	if(strcmp(first->name, "exit") == 0)
	{
		log_debug("exit is call!");
		return(1);
	}
	return(0);
}
