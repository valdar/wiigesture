#include <iostream>
#include <cstdlib>
#include <ctime>

#include "chmm.h"
#include "sample_3d.h"
#include "Gaussian_3d.h"
#include "Gaussian_3d_mixture.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


using namespace std;

/**
 *TODO: leggere "Types of HMM" (266)
 *      leggere parte riguardo scaling (272 + errata)
 *      leggere riguardo training con sequenze multiple (273)
*/

int main()
{

    /*
    Gaussian_3d gauss;
    std::cout<<gauss<<std::endl;

    Sample_3d s(0,0,0);

    double res = gauss.pdf_3d( s );

    std::cout<<res<<std::endl;


    boost::numeric::ublas::matrix<double> mat(3,3);

    int k = 0;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            mat(i,j) = k;
            k++;
        }
    }

    std::cout<<mat<<std::endl;
    */

    srand(time(0));

    // se size<2 si ha errore in backwardProc, e crash a riga 146 di hmm.cpp
    int size = 100;// + rand()%100;

    cout << "size: " << size << endl;

    int num_samples = 10;

    vector< vector<Sample_3d> > dataset;

    for(int i=0; i<num_samples; i++){
    	 vector<Sample_3d>* gesture = new vector<Sample_3d>();

    	for(int j=0; j<size; j++){
    	    double x = 2.0 + 1.0/(1 + rand()%10);
    	    double y = 2.0 + 1.0/(1 + rand()%10);
    	    double z = 2.0 + 1.0/(1 + rand()%10);
			Sample_3d* s = new Sample_3d(x, y, z);
			gesture->push_back( *s );

			// check
			/*
			std::cout<<"Sample "<<j<<std::endl;
            std::cout<<s->getData()[0]<<";"<<s->getData()[1]<<";"<<s->getData()[2]<<";"<<std::endl;
            */
    	}

    	dataset.push_back(*gesture);
	}

    int numStati = 8;
    bool isErgodic = false;

    cHMM hmm(numStati, isErgodic, 1);

    hmm.trainMS(dataset);

    //std::cout<<hmm.getA()<<std::endl;

    return 0;
}
