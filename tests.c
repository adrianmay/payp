#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

typedef struct _TB { struct _TB * next; int i; } Testblob;
typedef struct _TBB { struct _TTB * next; int i; int j; } Testbigblob;

typedef struct { Testblob head; int culled; } Testworld;

static Testworld * W() { return (Testworld*)(*d()); }

static Testblob * newBlobAfter(int big, Testblob * parent, int i) {  
	Testblob * noob;
	if (big)
		noob = (Testblob*) NEW(Testbigblob);
	else
		noob = NEW(Testblob);
	noob->next = parent->next;
	parent->next = noob;
	noob->i = i;
	return noob;
}

static void removeBlobAfter(Testblob * parent) {
	Testblob * victim = parent->next;
	parent->next = victim->next;
	pree(victim);
}

static void cullEvery(int m) {
	Testblob * p = &W()->head; 
	do { 
		if (p->next->i%m==m-1) removeBlobAfter(p);
		p=p->next;
	} while (p!=&W()->head);
}

static void newTestWorld() {
	(*d()) = NEW(Testworld);
	W()->culled = 0x5678;
	W()->head.next = &W()->head;
	W()->head.i = 0x1234;
	int i; Testblob * p; 
	for (p=&W()->head,i=0;i<10;i++,p=newBlobAfter((i%2?0:1),p,i)) ;
	cullEvery(6);
}

int testPalloc() {
	openHeap();
	if (*d()==0) {
		newTestWorld();
		printf("Made testworld\n");
	} else {
		int i; Testblob * p;
		cullEvery(--(W()->culled));
		for (p=&W()->head,i=0;i<50;i++,p=p->next) {
			printf("%d ", p->i);
		}
	}
	closeHeap();
	return 0;
}



