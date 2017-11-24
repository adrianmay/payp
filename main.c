#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

// #define TESTING 1

int maxIterations=MAX_ITERATIONS, iteration=0;

static void show() { 
	printf("%d %d %f %f %f %f\n", iteration, WORLD->numAnimals, WORLD->avgBal, 
			WORLD->avgProbReproK0, WORLD->avgProbReproK1, WORLD->avgProbReproK2  ); 
}

static int showProbRepro(ListedAnimal * la, void * fp) {
	fprintf((FILE*)fp, "%f\n", la->pItem->code.probReproK0);
	return 0;
}

static void shown() { 
	FILE * fp = fopen( "probrepro.hist", "w" );
	forAllAnimals(showProbRepro, fp);
	fclose(fp);
}


int main(int argc, char ** argv) {
	if (argc==2)
		maxIterations = atoi(argv[1]);
	openHeap();
	//if (!initWeb()) { printf("Can't open web server\n"); die(); }	
	if (WORLD==0) newWorld();
	while (!shouldquit && (maxIterations==0 || iteration <= maxIterations)) { 
		iteration++; 
		step(); 
		show(); 
	}
	shown();
	//unitWeb();
	closeHeap();
	return 0;
}


