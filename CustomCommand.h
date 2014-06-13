#ifndef CUSTOM_COMMAND_H
#define CUSTOM_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <map>
#include <sys/wait.h>
#include <errno.h>

#include "LogDebug.h"
#include "Definition.h"

using namespace std;

#define CUSTOM_CMD_NUM 5

extern list<Command*> command_list;
extern Job* fg_job;
extern list<Job*> bg_jobs;

int custom_exit(char** argv);

int custom_cd(char** argv);

int custom_jobs(char** argv);

int custom_fg(char** argv);

int custom_bg(char** argv);

int custom_command_init();

int custom_command();

#endif
