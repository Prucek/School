// Matej Jurik xjurik12 , Peter Rucek xrucek00 

#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <iostream>
#include <string.h>


std::vector<std::mutex *> locks; /* pole zamku promenne velikosti */

char *line;

char *read_line(int *res)
{
	std::string line;
	char *str;
	if (std::getline(std::cin, line))
	{
		str = (char *) malloc(sizeof(char)*(line.length()+1));
		strcpy(str,line.c_str());
		*res = 1;
		return str;
	} else
	{
		*res = 0;
		return NULL;
	}

}


void f(int ID)
{
	printf("Thread %i started\n",ID);
}

int main(int argc, char *argv[])
{
	if (argc < 4 || argc % 2 != 0)
	{
		fprintf(stderr,"USAGE: aut MIN_SCORE STR1 SC1 [ STR2 SC2 ] [ STR3 SC3 ] ...\n");
		return 1;
	}
	int margin_scores[100] = {0};
	char *strings[100] = {0};
	int it = 0;
	int min_score = (int) strtol(argv[2], (char **)NULL, 10);

	for (int i = 3; i < argc; i= i+2)
	{
		if (strlen(argv[i-1]) != 5)
		{
			fprintf(stderr,"Len of STR is not 5\n");
			return 1;
		}
		strings[it] = argv[i-1];
		margin_scores[it++] = (int) strtol(argv[i], NULL, 10);
	}

	(void) min_score;
	(void) margin_scores;
	(void) strings;

	/*******************************
	 * Inicializace threadu a zamku
	 * *****************************/
	int num = (argc/2) - 1;
	int num_locks = 15;
	std::vector <std::thread *> threads; /* pole threadu promenne velikosti */

	/* vytvorime zamky */
	locks.resize(num_locks); /* nastavime si velikost pole zamky */
	for(int i = 0; i < num_locks; i++)
	{	
		std::mutex *new_lock = new std::mutex();
		locks[i] = new_lock;
		/* Pokud je treba, tak vytvoreny zamek muzeme rovnou zamknout */
		(*(locks[i])).lock();
	}

	/* vytvorime thready */
	threads.resize(num); /* nastavime si velikost pole threads */
	for(int i = 0; i < num; i++)
	{	
		std::thread *new_thread = new std::thread (f,i);
		threads[i] = new_thread;
	}
	/**********************************
	 * Vlastni vypocet aut
	 * ********************************/
	int res;
	line = read_line(&res);
	while(res)
	{
		printf("%s\n",line);
		free(line); /* uvolnim pamet */
		line=read_line(&res);
	}


	/**********************************
	 * Uvolneni pameti
	 * ********************************/

	/* provedeme join a uvolnime pamet threads */
	for(int i = 0; i < num; i++)
	{
		(*(threads[i])).join();
		delete threads[i];
	}
	/* uvolnime pamet zamku */
	for(int i = 0; i < num_locks; i++)
	{
		delete locks[i];
	}

	printf("everything finished\n");
	return 0;
}
