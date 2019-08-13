#ifndef PRINT_H
#define PRINT_H
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void vrprintf(char* astring,...);
void setVerbose(uint8_t to_set);
void foprintf(char* astring,...);
void oprintf(char* astring,...);
void set_oprintf(FILE* to_print);
void help_print(const char option, const char* help);

#endif