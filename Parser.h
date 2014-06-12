#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <list>
#include <string.h>
#include <stdio.h>

#include "LogDebug.h"
#include "Definition.h"

using namespace std;

int parser(list<char*>* word_list, list<Command*>* command_list);

#endif
