#include <iostream>
#include <cstdlib>
#include <ctime>

#include "hmm.h"

using namespace std;


int main()
{
    srand(time(0));

    int size = 1 + rand()%1000;



    vector<int> gesture;
    for(int i=0; i<size; i++)
        gesture.push_back(1 + rand()%14);

    int numStati = 8;
    int numSimboli = 14;
    bool isErgodic = true;

    HMM hmm(numStati, numSimboli, isErgodic);

    vector< vector<int> > dataset;
    dataset.push_back(gesture);

    hmm.train(dataset);

    cout << "size: " << size << endl;

    return 0;
}
