#ifndef YAUSH_H
#define YAUSH_H

#include <stdio.h>
#include <iostream>

#include "LogDebug.h"
#include "Definition.h"
#include "ReadLine.h"
#include "Lexer.h"
#include "Parser.h"
#include "CmdExecute.h"
#include "CustomCommand.h"

using namespace std;

char* line_read=NULL;
list<char*> word_list;
list<Command*> command_list;
Job* fg_job=NULL;
list<Job*> bg_jobs;

#endif
