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

extern char* line_read;

extern list<Command*> command_list;

extern Job* fg_job;

extern list<Job*> bg_jobs;

int cmd_execute();

int check_bg_list();

#endif
