#ifndef PASSGEN_H
#define PASSGEN_H
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "print.h"
#define help_flags 0x01
#define string_flags 0x02
#define serial_flags 0x04
#define number_flags 0x08
#define follow_flags 0x10

void PSetFlags(uint16_t toflags);
void PsetOutput(FILE* output);
int seqgen(char* code_string, unsigned int start_position, unsigned int number_of_pass);
int rangen(char* code_string, unsigned int start_position, unsigned int number_of_pass,unsigned int seed);

#endif
