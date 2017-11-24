extern int shouldquit;

typedef struct { double probReproK0; double probReproK1; double probReproK2; } Code;

typedef struct { double bal; Code code; } Animal;

DECLARE_LISTED(Animal)

typedef struct { 
	ListedAnimal * animals; 
	int numAnimals;
	double avgProbReproK0;
	double avgProbReproK1;
	double avgProbReproK2;
	double avgBal;
} World;

extern int iteration;


