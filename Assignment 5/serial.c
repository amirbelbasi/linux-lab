#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void printHistogram(int *hist)
{
	for(int i = 0; i < 25; i++){
		for(int j = 0; j < hist[i]; j++){
			printf("*");
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[])
{
	int samples;
	if(argc == 2) samples = atoi(argv[1]);
	int hist[25] = {0};
	int rnd;
	int counter = 0;
	srand(time(NULL));
	for(int i = 0; i < samples; i++){
		counter = 0;
		for(int j = 0; j < 12; j++){
			rnd = rand() % 101;
			if (rnd >= 49) counter++;
			else counter--;
		}
		hist[counter+12]++;
	}
	//printHistogram(hist);
	return 0;
}