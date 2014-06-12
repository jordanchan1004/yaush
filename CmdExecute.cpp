#include "CmdExecute.h"

int cmd_execute()
{
	if(command_list.empty())// no command
		return(0);

	list<Command*>::reverse_iterator iter = command_list.rbegin();// command iterator from tail
	bool bg = (*(command_list.begin()))->bl_background;// whether background
	Job* job = new Job;// new Job
	if(job == NULL)// allocate failed
	{
		perror("Allocate for job failed");
		return(-1);
	}
	if(str_copy(job->content, line_read) != 0)
		return(-1);

	int pipe_num = command_list.size();// pipe number same as command number
	int** fd = new int*[pipe_num];// file descripters for pipe
	for(int i=0;i<pipe_num;i++)
	{
		fd[i] = new int[2];
	}
	for(int i=0;i<pipe_num;i++)// pipe
	{
		pipe(fd[i]);
	}

	int pid;

	for(int i=pipe_num-1;iter!=command_list.rend();iter++,i--)// run for all commands from tail to head
	{
		pid = fork();// fork
		if(pid > 0)// parent
		{
			close(fd[i][0]);
			close(fd[i][1]);
			(job->pid_list).push_back(pid);// add process ID to job's pid list
		}
		else if(pid == 0)// child
		{
			// handle input-output redirection and pipe
			if(strcmp((*iter)->input, "pipe") == 0)// input is pipe
			{
				dup2(fd[i-1][0], STDIN_FILENO);
				close(fd[i-1][0]);
				close(fd[i-1][1]);
			}
			else if(strcmp((*iter)->input, "stdin") != 0)// input is file
			{
				int fid = open((*iter)->input, O_RDONLY);
				if(fid == -1)
				{
					fprintf(stderr,"Open file %s failed: %s\n", (*iter)->input, strerror(errno));
					exit(-1);
				}
				dup2(fid, STDIN_FILENO);
				close(fid);
			}

			if(strcmp((*iter)->output, "pipe") == 0)// output is pipe
			{
				dup2(fd[i][1], STDOUT_FILENO);
				close(fd[i][0]);
				close(fd[i][1]);
			}
			else if(strcmp((*iter)->output, "stdout") != 0)// output is file
			{
				int fid = open((*iter)->output, O_WRONLY|O_CREAT|O_TRUNC, 0664);
				if(fid == -1)
				{
					fprintf(stderr,"Open file %s failed: %s\n", (*iter)->input, strerror(errno));
					exit(-1);
				}
				dup2(fid, STDOUT_FILENO);
				close(fid);
			}

			int flag = execvp((*iter)->name, (*iter)->coeff_list);// execute
			if(flag == -1)// failed
			{
				fprintf(stderr, "%s: %s\n", (*iter)->name, strerror(errno));
			}

			log_debug("execvp return: %d", flag);
			exit(-1);// child must stop
		}
		else// fork failed
		{
			perror("Fork failed");
			return(-1);
		}
	}

	if(bg)// background
	{
		printf("[%lu] %d\n", (bg_jobs.size() + 1), *((job->pid_list).begin()));// print infomation of the background job to be added.
		bg_jobs.push_back(job);// push job to the background jobs list
	}
	else// foreground
	{
		fg_job = job;
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
	}

	return(0);
}

int check_bg_list()
{
	bool bg = (*(command_list.begin()))->bl_background;// whether background
	list<Job*>::iterator job_temp;
	int cnt=1;

	for(list<Job*>::iterator i=bg_jobs.begin();i!=bg_jobs.end();cnt++)// check each job in the bg_jobs list
	{
		if((*i)->status != Running)// not running, no need to check
		{
			if(((*i)->status == Finished) && (!bg))// this job is finished and this time is foreground command
			{
				// remove this job
				job_temp = i;
				i++;
				bg_jobs.erase(job_temp);
			}
			else
			{
				i++;
			}
		}
		else// job is running, must check whether finished
		{
			list<int>* pid_list = &((*i)->pid_list);// get the pid list of this job
			list<int>::iterator pid_temp;
			for(list<int>::iterator j=pid_list->begin();j!=pid_list->end();)// for each pid
			{
				int pid=*j,status,r_val;
				r_val = waitpid(pid, &status, WNOHANG);
				if(r_val == 0)// pid not finished
				{
					j++;
				}
				else // get return or error
				{
					if(r_val > 0)// return successfully
					{
						log_debug("Process %d is waited successfully, return value: %d", pid, r_val);
					}
					else if(errno == ECHILD)// pid not found
					{
						log_debug("Process %d cannot found!", pid);
					}
					else// other error
					{
						perror("Unexpected error with waitpid");
					}
					pid_temp = j;// store j to temp
					j++;// move to next
					pid_list->erase(pid_temp);// erase this pid
				}
			}
			if(pid_list->empty())// this job is done
			{
				(*i)->status = Finished;// set the status of this job to be finished
				if(bg)// this time is background command, not remove this job
				{
					printf("[%d] finished\t%s\n", cnt, (*i)->content);// print information
					i++;
				}
				else// this time is foreground command, remove this job
				{
					printf("[%d]+ finished\t%s\n", cnt, (*i)->content);// print information
					job_temp = i;
					i++;
					bg_jobs.erase(job_temp);
				}
			}
			else// this job is not done yet
			{
				i++;
			}
		}// end of checking running job
	}// end of checking bg_jobs list

	return(0);
}
