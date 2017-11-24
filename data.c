#include <stdlib.h>
#include <stdio.h>
#include "header.h"

int shouldquit=0;

static float mutateProbRepro(float prob) {
	return prob + randNorm(0, prob*MUTERATE_PROBREPRO);
}

static Animal* newAnimal(Animal * parent) {
	Animal * nu = NEW(Animal);
	// Split money equally between parent and child after charging repro costs
	nu->bal = parent->bal = (parent->bal - FEE_REPRO)/2.0;
	nu->code.probReproK0 = mutateProbRepro(parent->code.probReproK0);
	nu->code.probReproK1 = mutateProbRepro(parent->code.probReproK1);
	nu->code.probReproK2 = mutateProbRepro(parent->code.probReproK2);
	return nu;
}

static ListedAnimal * newListedAnimal(Animal * a, ListedAnimal * n) {
	ListedAnimal * nu = NEW(ListedAnimal);
	nu->pItem=a;
	nu->pNext=n;
	return nu;
}

static void reproduce(ListedAnimal * predecessor) {
	TRACE( ("Reproduce\n") )
	ListedAnimal * listedparent = predecessor->pNext;
	Animal * parent = listedparent->pItem;
	float oldparentbal = parent->bal;
	predecessor->pNext = newListedAnimal(newAnimal(parent), predecessor->pNext);
	avgAdjust(0, WORLD->numAnimals, &WORLD->avgBal, parent->bal-oldparentbal);
	avgAdjust(1, WORLD->numAnimals, &WORLD->avgBal, predecessor->pNext->pItem->bal); 
	avgAdjust(1, WORLD->numAnimals, &WORLD->avgProbReproK0, predecessor->pNext->pItem->code.probReproK0); 
	avgAdjust(1, WORLD->numAnimals, &WORLD->avgProbReproK1, predecessor->pNext->pItem->code.probReproK1); 
	avgAdjust(1, WORLD->numAnimals, &WORLD->avgProbReproK2, predecessor->pNext->pItem->code.probReproK2); 
	WORLD->numAnimals++;
	if ( WORLD->animals == listedparent ) // So new one gets its first turn on
		WORLD->animals = predecessor->pNext; // the next (not this) iteration
}

static void croak(ListedAnimal * predecessor) {
	TRACE( ("Croak\n") )
	if (predecessor == predecessor->pNext) {
		printf("Last animal died\n");
		die();
	}
	ListedAnimal * listedcorpse = predecessor->pNext;
	Animal * corpse = listedcorpse->pItem;
	predecessor->pNext = listedcorpse->pNext;
	avgAdjust(-1, WORLD->numAnimals, &WORLD->avgProbReproK0, corpse->code.probReproK0); 
	avgAdjust(-1, WORLD->numAnimals, &WORLD->avgProbReproK1, corpse->code.probReproK1); 
	avgAdjust(-1, WORLD->numAnimals, &WORLD->avgProbReproK2, corpse->code.probReproK2); 
	avgAdjust(-1, WORLD->numAnimals, &WORLD->avgBal, corpse->bal); 
	WORLD->numAnimals--;
	if ( WORLD->animals == listedcorpse ) // Killed the root animal so use
		WORLD->animals=predecessor->pNext; // corpse's successor
	pree(corpse);
	pree(listedcorpse);
}

static float rent() { return WORLD->numAnimals < RENTLESS_POP 
	? 0 : FEE_TURN*(WORLD->numAnimals-RENTLESS_POP);
}

static float earn() { return randNorm(EARN_MU, EARN_SIGMA); }

static int stepAnimal(ListedAnimal * pred, void * extra) { //returns pop difference
	Animal * subject = pred->pNext->pItem;
	float changebal = earn() - rent(); //Later depends on animal
	subject->bal += changebal;
	avgAdjust(0, WORLD->numAnimals, &WORLD->avgBal, changebal); 
	if (  subject->bal < 0 || 
			rand01() < PROB_ACCIDENT || //Serial killer required to encourage reproduction
			WORLD->numAnimals > PASSOVER_POP && iteration%PASSOVER_FREQ==0 && rand01()<PASSOVER_RATE ) 
	{ croak(pred); return -1; }
	else {
		if (rand01() < subject->code.probReproK0 + (subject->code.probReproK1 + subject->code.probReproK2*subject->bal)*subject->bal ) 
		{ reproduce(pred); return 1; }
		return 0;
	}
}

void forAllAnimals(int (*func)(ListedAnimal *, void * extra), void * extra) {
	ListedAnimal * la = WORLD->animals;
	while (!shouldquit) {
		int i, popdiff = func(la, extra);
		for (i=0;i<popdiff+1;i++) {
			la=la->pNext;
			if ( la == WORLD->animals )
				return;
		}
	}
}

void step() { forAllAnimals(stepAnimal, 0); }

void newWorld() {
	*d() = NEW(World);
	Animal * adam = NEW(Animal);
	adam->bal=BAL_INIT;
	adam->code.probReproK0 = PROBREPRO_INIT;
	adam->code.probReproK1 = PROBREPRO_INIT;
	adam->code.probReproK2 = PROBREPRO_INIT;
	ListedAnimal * listedAdam = newListedAnimal(adam, 0);
	listedAdam->pNext=listedAdam; //Ring buffer
	WORLD->animals=listedAdam;
	WORLD->numAnimals=1;
	WORLD->avgProbReproK0=PROBREPRO_INIT;
	WORLD->avgProbReproK1=PROBREPRO_INIT;
	WORLD->avgProbReproK2=PROBREPRO_INIT;
	WORLD->avgBal=BAL_INIT;
}

