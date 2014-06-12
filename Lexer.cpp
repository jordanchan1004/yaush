#include "Lexer.h"

int lexer(char* line, list<char*>* word_list)
{
	char* pos_pre = line;
	char* pos;
	char* word;
	int word_len;

	word_list->clear();

	while(*pos_pre == SEPARATOR)// skip SEPARATOR at the beginning
	{
		pos_pre++;
	}

	while(*pos_pre != '\0')// till the end of the line
	{
		pos = strchr(pos_pre, SEPARATOR);// find the position of separator
		if(pos != NULL)// found
		{
			word_len = pos - pos_pre;
		}
		else// not found, means word to the end
		{
			word_len = strlen(pos_pre);
			pos = pos_pre + word_len;// move pos to the end
		}
		word = new char[word_len + 1];// allocate new space for word
		if(word == NULL)// failed
		{
			perror("Allocate failed");
			return(-1);
		}
		strncpy(word, pos_pre, word_len);// copy
		word[word_len] = '\0';// add end to the word
		word_list->push_back(word);// add the word to the list
		pos_pre = pos;// renew the pos_pre
		while(*pos_pre == SEPARATOR)// skip the separator just behind the separator
		{
			pos_pre++;
		}
	}

	// log print
	for(list<char*>::iterator iter=word_list->begin();iter!=word_list->end();iter++)
	{
		log_debug("%s",*iter);
	}

	return(0);
}
