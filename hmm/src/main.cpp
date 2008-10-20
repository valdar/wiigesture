#include <iostream>
#include <cstdlib>
#include <ctime>

#include "chmm.h"
#include "hmm.h"
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
    int size = 101;// + rand()%100;
    int numStati = 8;
    bool isErgodic = false;

    cout << "size: " << size << endl;

    int num_samples = 10;

    vector< vector<Sample_3d> > dataset;

    /*
    for(int i=0; i<num_samples; i++){
    	 vector<Sample_3d>* gesture = new vector<Sample_3d>();

    	for(int j=0; j<size; j++){
    	    double x = 2.0 + 1.0/(1 + rand()%10);
    	    double y = 2.0 + 1.0/(1 + rand()%10);
    	    double z = 2.0 + 1.0/(1 + rand()%10);
			Sample_3d* s = new Sample_3d(x, y, z);
			gesture->push_back( *s );

			// check

			std::cout<<"Sample "<<j<<std::endl;
            std::cout<<s->getData()[0]<<";"<<s->getData()[1]<<";"<<s->getData()[2]<<";"<<std::endl;

    	}

    	dataset.push_back(*gesture);
	}
	*/


    vector<Sample_3d>* gesture = new vector<Sample_3d>();

    for(int j=0; j<size; j++){
        /*
        double x = 2.0 + 1.0/(1 + rand()%10);
        double y = 2.0 + 1.0/(1 + rand()%10);
        double z = 2.0 + 1.0/(1 + rand()%10);
        */

        int x = 2.0, y = -1.8, z = 1.5;
        Sample_3d* s = new Sample_3d(x, y, z);
        gesture->push_back( *s );
    }

    for(int i=0; i<num_samples; i++){
        dataset.push_back(*gesture);
	}


    cHMM hmm(numStati, isErgodic, 1);

    //hmm.trainMS(dataset);

    //hmm.print();

    //std::cout<<hmm.getA()<<std::endl;


    /*** test caso discreto ***/

    int n_samples_disc = 15;
    int numSimboli = 14;
    int span = 1;

    vector< vector<int> > set;
    vector<int> g;

    for(int i=0; i<100; i++){
        //int x = rand() % numSimboli;

        int x;

        if(i<25)
            x = 7;
        else if(25<=x<50)
            x = 9;
        else if(50<x<=75)
            x = 7;
        else if(75<x<=90)
            x = 10;
        else
            x = 11;

        g.push_back(x);
        //std::cout<<x<<std::endl;
    }

    for(int j=0; j<n_samples_disc; j++)
        set.push_back(g);

    //std::cout<<set.size()<<std::endl;
    /*
    g.push_back(0);
    g.push_back(1);
    g.push_back(1);
    g.push_back(2);

    set.push_back(g);
    */

    HMM dhmm(numStati, numSimboli, isErgodic, span);

    dhmm.print();

    dhmm.trainMS2(set);

    dhmm.print();

    std::cout<<"Prob: "<<std::endl;
    std::cout<<dhmm.getP(g)<<std::endl;

    return 0;
}



