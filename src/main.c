/*
	Bulbizard V 1.0
	Semi-Random PassWord generator

*/
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "print.h"
#include "passgen.h"

#define help_flags 0x01
#define string_flags 0x02
#define serial_flags 0x04
#define number_flags 0x08
#define follow_flags 0x10




int main(int args, char* argv[])
{
	FILE* out_file = NULL;

	//Setting up flags an general variables
	uint16_t flags = 0x00;


	//Used to get the value of options
	int opt_val = 0;

	//The start position of the random
	//or sequential algorithm
	int start_position = 0;

	//Number of password generated (Can calculate the end position of algorithm)
	int number_of_pass = 1;

	//Number of charactere to use
	int char_to = 8;
	
	//Time seed by default;
	unsigned int seed = time(NULL);

	//Prevent automatic error message
	opterr=0;	

	
	//Get the input option
	while(opt_val != -1){
		opt_val = getopt(args,argv,"f:hvsd:n:e:w:o:");
	
		switch(opt_val){
			case 'f':
				stdin = fopen(optarg,"r");
				break;
			case 'h':
				flags |= help_flags;
				break;
			case '?':
				printf("Option \"-%c\" does not exist (-h for help)\n",optopt);
				break;
			case 'v':
				setVerbose(1);
				break;
			case 's':
				flags |= serial_flags;
				vrprintf("Using sequential mode\n");
				break;
			case 'd':
				start_position = atoi(optarg);
				vrprintf("Using %i as start position\n",start_position);
				break;
			case 'n':
				flags |= number_flags;
				number_of_pass = atoi(optarg);
				vrprintf("Printing %i passwords\n");
				break;
			case 'e':
				seed = atoi(optarg);
				break;
			case 'w':
				char_to = atoi(optarg);
				vrprintf("Generating %i characteres passwords.\n",char_to);
				break;
			case 'o':
				vrprintf("Using file %s as output\n",optarg);
				char* file_name = optarg;
				out_file=fopen(file_name,"w");
				if(out_file==NULL){
					printf("Erreur lors de l'ouverture de %s\n", file_name);
					return 1;
				}
				set_oprintf(out_file);
				break;
			default:
				break;		
		}

	}
	

	
	//Print help if asked
	if(flags & help_flags){
		printf("::: Bulbizard V 1.0 :::\n");
		printf("Usage: %s [string]\n\n",argv[0]);
		help_print('h',"Print help");
		help_print('v',"Set verbose");
		help_print('s',"Use serial mode (normal is random)");
		help_print('d',"Set a position (Use whit random seed or serial mode)");
		help_print('n',"Print a certain number of PassWords");
		help_print('e',"Set the seed number (random mode)");
		help_print('o',"Set the output file");
		help_print('w',"Set the number of characteres (overided if a string is used)");
		help_print('f',"Read from file");
		printf("\nThe escape characteres for code génération are preceded whit an \\.\n");
		help_print('A',"A capital letter");
		help_print('a',"A miniscule letter");
		help_print('l',"Any letter (Cap/min)");
		help_print('L',"Any letter of Number");
		help_print('r',"Random (even non-printable)");
		help_print('p',"Random printable only");
		help_print('P',"Random printable non-number and non-letter");
		help_print('n',"A number");
		return 0;
	}
	

	PsetOutput(out_file);
	PSetFlags(flags);

	char* code_string;

	//Get the string to work whit
	if(optind<args){
		//The input is an arg
		flags |= string_flags;
		sscanf(argv[optind],"%ms",&code_string);

		vrprintf("Using string: %s\n",code_string);
		//The sequential algorithm
		if (flags & serial_flags)
		{	
			seqgen(code_string,start_position,number_of_pass);
		}else{
			rangen(code_string,start_position,number_of_pass,seed);
		}

		//Nettoyage
		free(code_string);
	
	}else{
		//The input is not an args
		//Essayer de lire de STDIN
		if(!isatty(fileno(stdin))){
			char* stdin_buffer = (char*) malloc(255*sizeof(char));

			//The input come from a pipe or a file
			//Read lines, if it fails stop
			while(fgets(stdin_buffer,255,stdin) != NULL){
				int size=strlen(stdin_buffer);

				//Copy to a buffer the right size pointed by code_string
				//Drop the ending \n
				code_string=(char*)malloc((size-1)*sizeof(char));
				strncpy(code_string,stdin_buffer,size-1);
				code_string[size-1]='\0';

				//The sequential algorithm
				if (flags & serial_flags)
				{	
					seqgen(code_string,start_position,number_of_pass);
				}else{
					rangen(code_string,start_position,number_of_pass,seed);
				}
				free(code_string);
				memset(stdin_buffer,'\0',255*sizeof(char));
			}
			free(stdin_buffer);


		}else{

			//There are no string to work whit, so put all char random
			int string_size = 2*char_to;
			code_string = (char*) malloc(2*char_to*sizeof(char));
			int i = 0;
			for (i = 0; i < string_size; ++i)
			{
				if(i%2){
					code_string[i]='p';
				}else{
					code_string[i]='\\';
				}
			}
			//The sequential algorithm
			if (flags & serial_flags)
			{	
				seqgen(code_string,start_position,number_of_pass);
			}else{
				rangen(code_string,start_position,number_of_pass,seed);
			}
			free(code_string);
		}
	}




	if(out_file!=NULL){
		fclose(out_file);
	}
	return 0;
}
