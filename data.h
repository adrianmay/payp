extern int shouldquit;

typedef struct { float probReproK0; float probReproK1; float probReproK2; } Code;

typedef struct { float bal; Code code; } Animal;

DECLARE_LISTED(Animal)

typedef struct { 
	ListedAnimal * animals; 
	int numAnimals;
	float avgProbReproK0;
	float avgProbReproK1;
	float avgProbReproK2;
	float avgBal;
} World;

extern int iteration;


