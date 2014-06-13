#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <errno.h>

#include "LogDebug.h"
#include "Definition.h"

using namespace std;

extern sigjmp_buf jmpbuf_ctrlz;
extern sigjmp_buf jmpbuf_ctrlc;
extern Job* fg_job;
extern list<Job*> bg_jobs;

void ctrlz_catcher(int signo, siginfo_t* info, void* ctx);

void ctrlc_catcher(int signo, siginfo_t* info, void* ctx);

void bind_shortcut();

#endif
