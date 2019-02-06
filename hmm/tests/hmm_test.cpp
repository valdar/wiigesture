#include "hmm.h"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/test/floating_point_comparison.hpp>

//#define BOOST_TEST_MODULE HMM_test

BOOST_AUTO_TEST_SUITE( HMM_test )

BOOST_AUTO_TEST_CASE( constructorLfetToRight_test )
{
    //creazione HMM
    HMM test_object(3, 3, false, 1);

    // matrice A iniziale HMM
    boost::numeric::ublas::matrix<double> A;
    A = test_object.getA();

    // matrice A iniziale come dovrebbe essere
    boost::numeric::ublas::matrix<double> Atest(3,3);

    Atest(0,0)=0.5;
    Atest(1,0)=0.0;
    Atest(2,0)=0.0;
    Atest(0,1)=0.5;
    Atest(1,1)=0.5;
    Atest(2,1)=0.0;
    Atest(0,2)=0.0;
    Atest(1,2)=0.5;
    Atest(2,2)=1.0;


    //matrice B iniziale HMM
    boost::numeric::ublas::matrix<double> B;
    B = test_object.getB();

    //matrice B iniziale come dovrebbe essere
    boost::numeric::ublas::matrix<double> Btest(3,3);

    Btest(0,0)=(double)1/3;
    Btest(1,0)=(double)1/3;
    Btest(2,0)=(double)1/3;
    Btest(0,1)=(double)1/3;
    Btest(1,1)=(double)1/3;
    Btest(2,1)=(double)1/3;
    Btest(0,2)=(double)1/3;
    Btest(1,2)=(double)1/3;
    Btest(2,2)=(double)1/3;

    // controllo che HMM non sia ergodico
    BOOST_REQUIRE( !test_object.getIsErgodic() );

    //controllo matrice A
    for(int i=0; i<A.size1(); i++){
       for(int j=0; j<A.size2(); j++){
            BOOST_CHECK_CLOSE(A(i,j),Atest(i,j),0.0000001);
        }
    }

    //controllo matrice B
    for(int i=0; i<B.size1(); i++){
       for(int j=0; j<B.size2(); j++){
            BOOST_CHECK_CLOSE(B(i,j),Btest(i,j),0.0000001);
        }
    }
}

BOOST_AUTO_TEST_CASE( forwardProc_test ){

    //dichiarazione HMM
    HMM test_object(3, 3, false, 1);

    //dichiarazione vettore d'ingresso
    std::vector<int> O;
    O.push_back(0);
    O.push_back(0);
    O.push_back(0);

    //matrice alpha prodotta dall'HMM
    boost::numeric::ublas::matrix<double> ALPHA(3,3);
    test_object.forwardProc(O, ALPHA);

    //matrice alpha calcolata con mathlab
    boost::numeric::ublas::matrix<double> ALPHAtest(3,3);

    ALPHAtest(0,0)=(double)1/3;
    ALPHAtest(1,0)=0.0;
    ALPHAtest(2,0)=0.0;
    ALPHAtest(0,1)=0.055555555555556;
    ALPHAtest(1,1)=0.055555555555556;
    ALPHAtest(2,1)=0.0;
    ALPHAtest(0,2)=0.009259259259259;
    ALPHAtest(1,2)=0.018518518518519;
    ALPHAtest(2,2)=0.009259259259259;

    //confronto
    for(int i=0; i<ALPHA.size1(); i++){
        for(int j=0; j<ALPHA.size2(); j++){
            BOOST_CHECK_CLOSE(ALPHA(i,j),ALPHAtest(i,j),0.0000001);
        }
    }

}

BOOST_AUTO_TEST_CASE( backwardProc_test ){

    //dichiarazione HMM
    HMM test_object(3, 3, false, 1);

    //dichiarazione vettore d'ingresso
    std::vector<int> O;
    O.push_back(0);
    O.push_back(0);
    O.push_back(0);

    //matrice beta prodotta dall'HMM
    boost::numeric::ublas::matrix<double> BETA(3,3);
    test_object.backwardProc(O, BETA);

    //matrice beta calcolata con mathlab
    boost::numeric::ublas::matrix<double> BETAtest(3,3);

    BETAtest(0,0)=0.111111111111111;
    BETAtest(1,0)=0.111111111111111;
    BETAtest(2,0)=0.111111111111111;
    BETAtest(0,1)=0.333333333333333;
    BETAtest(1,1)=0.333333333333333;
    BETAtest(2,1)=0.333333333333333;
    BETAtest(0,2)=1.0;
    BETAtest(1,2)=1.0;
    BETAtest(2,2)=1.0;

    //confronto
    for(int i=0; i<BETA.size1(); i++){
        for(int j=0; j<BETA.size2(); j++){
            BOOST_CHECK_CLOSE(BETA(i,j),BETAtest(i,j),0.0000001);
        }
    }

}

BOOST_AUTO_TEST_CASE( train_single_sequence_test ){

    //dichiarazione HMM
    HMM test_object(3, 3, false, 1);

    //dichiarazione vettore d'ingresso
    std::vector<int> O;
    O.push_back(0);
    O.push_back(0);
    O.push_back(0);
    O.push_back(0);
    // necessita di almeno 4 componenti perchè l'HMM ha 3 stati!!!

    //aggiornamento amtrici A e B tramite train
    std::vector<std::vector<int> > gestureSet;
    gestureSet.push_back(O);
    test_object.trainMS(gestureSet);

    //matrice A aggiornata (calcolata con mathlab)
    boost::numeric::ublas::matrix<double> Anew(3,3);

    Anew(0,0)=0.500000000000000;
    Anew(1,0)=0.0;
    Anew(2,0)=0.0;
    Anew(0,1)=0.500000000000000;
    Anew(1,1)=0.500000000000000;
    Anew(2,1)=0.0;
    Anew(0,2)=0.0;
    Anew(1,2)=0.500000000000000;
    Anew(2,2)=1.0;

    //matrice B aggiornata (calcolata con mathlab)
    boost::numeric::ublas::matrix<double> Bnew(3,3);

    Bnew(0,0)=1.0;
    Bnew(1,0)=1.0;
    Bnew(2,0)=1.0;
    Bnew(0,1)=0.0;
    Bnew(1,1)=0.0;
    Bnew(2,1)=0.0;
    Bnew(0,2)=0.0;
    Bnew(1,2)=0.0;
    Bnew(2,2)=0.0;

    //confronto
    boost::numeric::ublas::matrix<double> Ahmm(3,3);
    Ahmm = test_object.getA();

    boost::numeric::ublas::matrix<double> Bhmm(3,3);
    Bhmm = test_object.getB();

    for(int i=0; i<Ahmm.size1(); i++){
        for(int j=0; j<Ahmm.size2(); j++){
            BOOST_CHECK_CLOSE(Ahmm(i,j),Anew(i,j),0.0000001);
        }
    }

    for(int i=0; i<Bhmm.size1(); i++){
        for(int j=0; j<Bhmm.size2(); j++){
            BOOST_CHECK_CLOSE(Bhmm(i,j),Bnew(i,j),0.0000001);
        }
    }


}

BOOST_AUTO_TEST_SUITE_END()
