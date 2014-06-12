#ifndef CMD_EXECUTE_H
#define CMD_EXECUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <list>
#include <string.h>
#include <sys/wait.h>

#include "LogDebug.h"
#include "Definition.h"

using namespace std;

int cmd_execute(list<Command*>* command_list, const char* line_read, Job* fg_job, list<Job*>* bg_jobs);

#endif
