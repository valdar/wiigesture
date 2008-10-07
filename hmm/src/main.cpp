#include <iostream>
#include <cstdlib>
#include <ctime>

#include "hmm.h"
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
    /*
    srand(time(0));

    // se size<2 si ha errore in backwardProc, e crash a riga 146 di hmm.cpp
    int size = 16;// 1 + rand()%1000;
    int num_samples = 1;

    vector<int> gesture;
    for(int i=0; i<size; i++)
        gesture.push_back(1 + rand()%14);

    int numStati = 8;
    int numSimboli = 14;
    bool isErgodic = false;

    HMM hmm(numStati, numSimboli, isErgodic);

    vector< vector<int> > dataset;
    for(int i=0; i<num_samples; i++)
        dataset.push_back(gesture);

    hmm.trainMS(dataset);

    cout << "size: " << size << endl;

    hmm.print();
    hmm.print_to_file();
    */

    Gaussian_3d* g = new Gaussian_3d();

    std::cout<<g->cov<<std::endl;

    return 0;
}
