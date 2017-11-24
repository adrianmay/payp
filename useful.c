#include <stdlib.h>

double rand01() { return ((double)rand())/((double)RAND_MAX); }

double randNorm (double mu, double sigma) {
	const int samples = 10;
	double v=0;
	int i; for (i=0;i<samples;i++) v+=rand01(); v/=samples; v-=0.5;
	// v is approx normally distributed with mu=0 and sigma=1/samples*sqrt(12)
	return v*samples*3.464*sigma + mu;
}

void avgAdjust(int dir, int num, double * avg, double datum) {
	if (dir==0) //datum is change to existing element
		(*avg) += datum/num;
	else //adding or removing a datum
		(*avg) = ( (*avg)*num + dir*datum ) / ( num + dir );
}

