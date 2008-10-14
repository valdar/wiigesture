#include <iostream>
#include <cstdlib>
#include <ctime>

#include "chmm.h"
#include "sample_3d.h"
#include "Gaussian_3d.h"
#include "Gaussian_3d_mixture.h"

using namespace std;

/**
 *TODO: leggere "Types of HMM" (266)
 *      leggere parte riguardo scaling (272 + errata)
 *      leggere riguardo training con sequenze multiple (273)
*/

int main()
{

    srand(time(0));

    // se size<2 si ha errore in backwardProc, e crash a riga 146 di hmm.cpp
    int size = 10;// + rand()%100;
    int num_samples = 1;

    vector< vector<Sample_3d> > dataset;

    for(int i=0; i<num_samples; i++){
    	 vector<Sample_3d>* gesture = new vector<Sample_3d>();
    	for(int j=0; j<size; j++){
			Sample_3d* s = new Sample_3d( (double) 1.0 + 1.0/(rand()%15), (double) 1.0 + 1.0/(rand()%15), (double) 1.0 + 1.0/(rand()%15) );
			gesture->push_back( *s );
    	}

    	dataset.push_back(*gesture);
	}


    int numStati = 8;
    bool isErgodic = false;

    cHMM hmm(numStati, isErgodic, 3);

    hmm.train(dataset);

    cout << "size: " << size << endl;

    hmm.print();

    return 0;
}
