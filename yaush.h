#ifndef YAUSH_H
#define YAUSH_H

#include <stdio.h>
#include <setjmp.h>
#include <iostream>

#include "LogDebug.h"
#include "Definition.h"
#include "ReadLine.h"
#include "Lexer.h"
#include "Parser.h"
#include "CmdExecute.h"
#include "CustomCommand.h"
#include "Shortcut.h"

using namespace std;

char* line_read=NULL;
list<char*> word_list;
list<Command*> command_list;
Job* fg_job=NULL;
list<Job*> bg_jobs;
sigjmp_buf jmpbuf_ctrlz;
sigjmp_buf jmpbuf_ctrlc;

#endif
