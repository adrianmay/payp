extern int shouldquit;

typedef struct { float probRepro; } Code;

typedef struct { float bal; Code code; } Animal;

DECLARE_LISTED(Animal)

typedef struct { 
	ListedAnimal * animals; 
	int numAnimals;
	float avgProbRepro;
	float avgBal;
} World;

// TESTIMG ONLY


