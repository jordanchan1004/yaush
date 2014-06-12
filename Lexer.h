#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <list>

#include "LogDebug.h"
#include "Definition.h"

using namespace std;

#define SEPARATOR ' '

extern char* line_read;
extern list<char*> word_list;

int lexer();

#endif
