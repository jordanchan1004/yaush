#include "CustomCommand.h"

const char* custom_command_list[] = 
{
	"exit",
	"cd",
	"jobs",
	"fg",
	"bg"
};

int (*fn[])(char**) = 
{
	custom_exit,
	custom_cd,
	custom_jobs,
	custom_fg,
	custom_bg
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
		perror("yaush: cd");
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
		printf("yaush: fg: Please specify a number for fg\n");
		return(-1);
	}
	int idx = atoi(idx_str);
	if((idx <= 0) || (idx > (int)bg_jobs.size()))// index invalid
	{
		printf("yaush: fg: Index for fg is invalid!\n");
		return(-1);
	}
	list<Job*>::iterator iter = bg_jobs.begin();
	for(int i=1;i<idx;i++,iter++);// move to the idx entry
	if((*iter)->status == Finished)// job finished, cannot fg
	{
		printf("yaush: fg: Job %d is finished!\n", idx);
		return(-1);
	}
	else if(((*iter)->status == Running) || ((*iter)->status == Stopped))// job running or stopped
	{
		if(fg_job != NULL)
		{
			delete fg_job;
			fg_job = NULL;
		}
		fg_job = new Job;
		swap((*iter)->pid_list, fg_job->pid_list);// copy pid_list
		str_copy(fg_job->content, (*iter)->content);// copy content
		bg_jobs.erase(iter);// remove this job from bg_jobs
		if((*iter)->status == Stopped)// job stopped, need to be resume
		{
			// tell all the process in pid_list to continue 
			list<int>* pid_list = &(fg_job->pid_list);
			log_debug("The size of pid_list of the job is: %lu", pid_list->size());
			for(list<int>::iterator i=pid_list->begin();i!=pid_list->end();i++)
			{
				int r_val = kill(*i, SIGCONT);
				if((r_val == -1) && (errno != ESRCH))
				{
					perror("Tell one pid of stopped job failed");
				}
			}
		}
		fg_job->status = Running;// set fg_job running
		// wait for all pid in fg_job to finish
		list<int>::iterator pid_temp;
		for(list<int>::iterator i=(fg_job->pid_list).begin();i!=(fg_job->pid_list).end();)// while pid list not empty
		{
			int pid=*i, status, r_val;
			r_val = waitpid(pid,&status,0);// wait pid to exit
			if((r_val > 0) || ((r_val == -1) && (errno == ECHILD)))// 
			{
				pid_temp = i;
				i++;
				(fg_job->pid_list).erase(pid_temp);
			}
			else
			{
				perror("yaush: fg: Unexpected error with waitpid");
			}
		}
		delete fg_job;
		fg_job = NULL;
		return(0);
	}
	else// for other future status of job
	{
		return(-1);
	}
}

int custom_bg(char** argv)
{
	if(argv[1] == NULL)// no coeffs, found the last stopped job
	{
		list<Job*>::reverse_iterator iter;
		int cnt = bg_jobs.size();
		for(iter=bg_jobs.rbegin();iter!=bg_jobs.rend();iter++,cnt--)
		{
			if((*iter)->status == Stopped)// found one
			{
				// tell all the process in pid_list to continue 
				list<int>* pid_list = &((*iter)->pid_list);
				log_debug("The size of pid_list of the job is: %lu", pid_list->size());
				for(list<int>::iterator i=pid_list->begin();i!=pid_list->end();i++)
				{
					int r_val = kill(*i, SIGCONT);
					if((r_val == -1) && (errno != ESRCH))
					{
						perror("Tell one pid of stopped job failed");
					}
				}
				(*iter)->status = Running;// set the status back to running
				printf("[%d] %s\n", cnt, (*iter)->content);
				return(0);
			}
		}
		// no job is stopped
		printf("yaush: bg: No jobs stopped found!\n");
		return(-1);
	}
	else// with coeff
	{
		int idx = atoi(argv[1]);
		if((idx <= 0) || (idx > (int)bg_jobs.size()))// index invalid
		{
			printf("yaush: bg: Index for bg is invalid!\n");
			return(-1);
		}
		log_debug("The size of bg_jobs is: %lu", bg_jobs.size());
		list<Job*>::iterator iter = bg_jobs.begin();
		for(int i=1;i<idx;i++,iter++);// move to the idx entry
		Job* target = (*iter);
		if(target->status == Finished)// job finished
		{
			printf("yaush: bg: Job %d is finished!\n", idx);
			return(-1);
		}
		else if(target->status == Running)// job running
		{
			printf("yaush: bg: Job %d is running!\n", idx);
			return(-1);
		}
		else// job stopped
		{
			// tell all the process in pid_list to continue 
			list<int>* pid_list = &(target->pid_list);
			log_debug("The size of pid_list of the job is: %lu", pid_list->size());
			for(list<int>::iterator i=pid_list->begin();i!=pid_list->end();i++)
			{
				int r_val = kill(*i, SIGCONT);
				if((r_val == -1) && (errno != ESRCH))
				{
					perror("Tell one pid of stopped job failed");
				}
			}
			target->status = Running;
			printf("[%d] %s\n", idx, target->content);
			return(0);
		}
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
