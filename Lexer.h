#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <list>

#include "LogDebug.h"
#include "Definition.h"

using namespace std;

#define SEPARATOR ' '

int lexer(char* line, list<char*>* word_list);

#endif
