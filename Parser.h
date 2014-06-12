#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <list>
#include <string.h>
#include <stdio.h>

#include "LogDebug.h"
#include "Definition.h"

using namespace std;

extern list<char*> word_list;

extern list<Command*> command_list;

int parser();

#endif
