#include "print.h"
/* "Librairies" pour l'affichage du texte */

FILE* out_file = NULL;
uint8_t verbose = 0;
unsigned int last_ffprint = 0;

void vrprintf(char* astring,...)
{
	if(verbose){
		printf(astring);
	}
}

void setVerbose(uint8_t to_set){
	verbose = to_set;
}

void foprintf(char* astring,...)
{
	int i = 0;
	for (i = 0; i < last_ffprint; ++i)
	{
		printf("\b");
	}
	va_list arguments;
	va_start(arguments, astring);
	char* to_print;
	vsprintf(to_print,astring,arguments);
	last_ffprint = strlen(to_print);
	printf(to_print);
}

void oprintf(char* astring,...)
{
	va_list arguments;
	va_start(arguments,astring);
	if(out_file != NULL){
		vfprintf(out_file,astring,arguments);
	}else{
		vprintf(astring,arguments);
	}
}

void set_oprintf(FILE* to_print){
	out_file = to_print;
}

void help_print(const char option, const char* help)
{
	printf("\t-%c\t\t%s\n",option,help);
}