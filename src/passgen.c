#include "passgen.h"

uint16_t flags = 0x00;

void PSetFlags(uint16_t toflags){
	flags=toflags;
}

void PsetOutput(FILE* output){
	set_oprintf(output);
}

int seqgen( char* code_string, unsigned int start_position, unsigned int number_of_pass){

	//Get the length of the sequential string
	vrprintf("Starting sequential algorithm for %s\n",code_string);
	int string_size = strlen(code_string);
	vrprintf("Size of string to analyse:%i\n",string_size);

	//Taille non-optimal... mais combien plus simple	
	char* final_code = (char*)malloc(string_size*sizeof(char));
	memset(final_code,'\0',string_size*sizeof(char));

	//Création du tableau d'index et de valeur maximal
	//Calcule du nombre de mots de passe
	int t_index[string_size];
	int max_index[string_size];
	int i = 0;
	int total = 1;
	int max = 1;

	for (i = 0; i < string_size; ++i)
	{
	 	t_index[i]=0;
	 	if (code_string[i]=='\\')
		{
	 		switch(code_string[i+1]){
				case 'r':
					max_index[i]=0xFF;
					break;
				case 'p':
					max_index[i]=94;
					break;

				case 'P':
					max_index[i]=29;
					break;

				case 'A':
					max_index[i]=25;
					break;

				case 'a':
					max_index[i]=25;
					break;

				case 'l':
					max_index[i]=50;
					break;
				case 'n':
					max_index[i]=9;
					break;
				case '\\':
					max_index[i]=0;
					break;
			}

		}else{
			max_index[i]=0;
		}
		if(max_index[i]>0){
			max=max_index[i]*max;
		}
	}
	vrprintf("Nombre total de mots de passe possible = %i\n",max);

	int passage = 0;
	//Demarage des génération
	while(t_index[0]<= max_index[0]){
			

		//Génér le mot de passe
		int final_index = 0;
		for (i = 0; i < string_size; ++i)
		{
			if (code_string[i]=='\\')
			{
				//Its a variable char
				char to_add = '\0';
				long int ran_num = random();

				switch(code_string[i+1]){
					case 'r':
							to_add=t_index[i];
							break;
					case 'p':
							to_add=32+t_index[i];
						break;
					case 'P':
							if (t_index[i] < 16){
								to_add=32+(ran_num%15);
							}else if(t_index[i] < 22){
								to_add=58+(t_index[i] -16);
							}else if(t_index[i] < 27){
								to_add=91+(t_index[i] - 22);
							}else{	
								to_add=123+(ran_num-27);
							}	
					break;
					case 'A':
							to_add=65+t_index[i];
							break;
					case 'a':
							to_add=97+t_index[i];
								break;
					case 'l':
							if(t_index[i] < 26){
								to_add= 65+t_index[i];
							}else{
								to_add=97-25+t_index[i];
							}
							break;
					case 'n':
							to_add=48+t_index[i];
							break;
					case '\\':
								to_add='\\';
							break;
						break;
				}
				final_code[final_index]=to_add;
				++final_index;
				++i;
			}else{
				//A non variable char
				final_code[final_index]=code_string[i];
					++final_index;
			}
		}

		if(passage>=start_position){
			oprintf("%s\n",final_code);
		}

		//Augmenter le dernier index de un
		t_index[string_size-1]++;

		//Évaluation et distribution des retenues
		//Le faire à l'envers pour que se soit plus naturel
		for (i = string_size -1; i > 0; --i)
		{
			if (t_index[i]>max_index[i])
			{
					t_index[i] = 0;
					t_index[i-1] = t_index[i-1] + 1;
			}

		}

		//Testez si le nombre de code demander a été généré
		if(flags & number_flags){
			if(passage>=number_of_pass+start_position){
				vrprintf("Ended at position #%i\n",passage);
				return 0;
			}
		}

		++passage;
	}



	return 0;
}

int rangen(char* code_string, unsigned int start_position, unsigned int number_of_pass,unsigned int seed){
	//The random algorithm
	vrprintf("Starting random algorithm for %s\n",code_string);
	srandom(seed);
	vrprintf("Using 0x%X as seed\n",seed);

	int string_size = strlen(code_string);

	//Taille non-optimal... mais combien plus simple	
	char* final_code = (char*)malloc(string_size*sizeof(char));
	memset(final_code,'\0',string_size*sizeof(char));

	int pass = 0;
	for(pass=0; pass< (number_of_pass+start_position) ;++pass){
		int i = 0;
		int final_index = 0;
		for (i = 0; i < string_size; ++i)
		{
			if (code_string[i]=='\\')
			{
				//Its a variable char
				char to_add = '\0';
				long int ran_num = random();

				switch(code_string[i+1]){
					case 'r':
							memcpy(&to_add,&ran_num,sizeof(char));
						break;
					case 'p':
							to_add=32+(ran_num%94);
						break;
					case 'P':
							switch(ran_num%4){
								case 0:
									to_add=32+(ran_num%15);
									break;
								case 1:
									to_add=58+(ran_num%6);
									break;
								case 2:
									to_add=91+(ran_num%5);
									break;
								case 3:	
									to_add=123+(ran_num%3);
									break;
							}	
						break;
					case 'A':
							to_add=65+(ran_num%25);
						break;
					case 'a':
							to_add=97+(ran_num%25);
							break;
					case 'l':
							to_add= ((ran_num%2)?65:97)+ran_num%25;
							break;
					case 'n':
							to_add=48+(ran_num%9);
							break;
					case '\\':
							to_add='\\';
							break;
						break;
				}

				final_code[final_index]=to_add;
				++final_index;
				++i;
			}else{
				//A non variable char
				final_code[final_index]=code_string[i];
				++final_index;
			}
		}	

		if(pass>=start_position){
			oprintf("%s\n",final_code);
		}
	}

	return 0;
}