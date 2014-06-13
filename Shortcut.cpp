#include "Shortcut.h"

void ctrlz_catcher(int signo, siginfo_t* info, void* ctx)
{
	printf("\n");
	log_debug("Ctrl-Z is caught by yaush");
	if(fg_job)
	{
		Job* temp = new Job;
		swap(temp->pid_list, fg_job->pid_list);
		str_copy(temp->content, fg_job->content);
		temp->status = Stopped;
		bg_jobs.push_back(temp);
		delete fg_job;
		fg_job = NULL;
	}
	for(list<Job*>::iterator i=bg_jobs.begin();i!=bg_jobs.end();i++)
	{
		if((*i)->status == Running)
		{
			(*i)->status = Stopped;
		}
	}	

	siglongjmp(jmpbuf_ctrlz, 1);
}

void ctrlc_catcher(int signo, siginfo_t* info, void* ctx)
{
	printf("\n");
	log_debug("Ctrl-C is caught by yaush");
	if(fg_job)// if Ctrl-C is called when foreground job running
	{
		list<int>::iterator pid_temp;
		for(list<int>::iterator i=(fg_job->pid_list).begin();i!=(fg_job->pid_list).end();)
		{
			//kill(*i, SIGINT);
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
		delete fg_job;// delete fg_job
		fg_job = NULL;

	}

	list<Job*>::iterator job_temp;
	for(list<Job*>::iterator i=bg_jobs.begin();i!=bg_jobs.end();)
	{
		if((*i)->status == Running)
		{
			list<int>* pid_list = &((*i)->pid_list);
			list<int>::iterator pid_temp;
			for(list<int>::iterator j=pid_list->begin();j!=pid_list->end();)
			{
				int pid=*j, status, r_val;
				r_val = waitpid(pid,&status,0);// wait pid to exit
				if((r_val > 0) || ((r_val == -1) && (errno == ECHILD)))// 
				{
					pid_temp = j;
					j++;
					pid_list->erase(pid_temp);
				}
				else
				{
					perror("yaush: fg: Unexpected error with waitpid");
				}
			}
			job_temp = i;
			i++;
			bg_jobs.erase(job_temp);
		}
		else
		{
			i++;
		}
	}

	siglongjmp(jmpbuf_ctrlc, 1);
}

void bind_shortcut()
{
	struct sigaction act;
	sigfillset(&(act.sa_mask));
	act.sa_flags = SA_SIGINFO;
	
	act.sa_sigaction = ctrlz_catcher;// bind ctrl-z catcher
	sigaction(SIGTSTP, &act, NULL);

	act.sa_sigaction = ctrlc_catcher;// bind ctrl-c catcher
	sigaction(SIGINT, &act, NULL);
}
