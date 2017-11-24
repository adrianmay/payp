#pragma pack(4)

// #define DEBUG 1
#ifdef DEBUG
#include <stdio.h>
#define TRACE(params) printf params;
#else
#define TRACE(params)
#endif 

#define FEE_REPRO 10.0
#define FEE_TURN 0.003
#define RENTLESS_POP 100
#define EARN_MU 5.0
#define EARN_SIGMA 1.0
#define MUTERATE_PROBREPRO 0.1
#define PROBREPRO_INIT 0.0025
#define POP_INIT 100
#define BAL_INIT 100
#define PROB_ACCIDENT 0.05

#define DECLARE_LISTED(CLS) \
typedef struct Listed ## CLS { \
	CLS * pItem; \
	struct Listed ## CLS * pNext; \
} Listed ## CLS; 

#define NEW(CLS) ((CLS*) palloc(sizeof(CLS)))

#define WORLD ((World*)*d())

#include "data.h"
#include "forwards.hh"
