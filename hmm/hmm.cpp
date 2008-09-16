#include "hmm.h"

HMM::HMM(int stati, int osservazioni){

    this->numStati = stati;
    this->numOss = osservazioni;
    this->pi = new int[numStati];
    this->A = new double[numStati*numStati];
    this->B = new double[numStati*numOss];

}


void HMM::init_left-to-right(int span){

    int forwardLimit = span;

    /* probabilità stato iniziale */
    pi[0] = 1.0;
    for(int i=1; i<numStati; i++)
        pi[i] = 0;

    /* matrice di transizione */
    for(int i=0; i<numStati; i++) {
        for(int j=0; j<numStati; j++) {

            if(i==numStati-1 && j==numStati-1) { // ultimo elemento (basso a dx)
                A[i][j] = 1.0;
            }
            else if(i==numStati-2 && j==numStati-2) {
                A[i][j] = 0.5;
            }
            else if(i==numStati-2 && j==numStati-1) {
                A[i][j] = 0.5;
            }
            else if(i<=j && i>j-forwardLimit-1) {
                A[i][j] = 1.0/( (double) (forwardLimit+1) );
            }
            else {
                A[i][j] = 0.0;
            }
        }
    }

     /* matrice di emissione */
    for(int i=0; i<numStati; i++)
        for(int j=0; j<numOss; j++)
            B[i][j] = 1.0 / (double) numOss;

}

/**
 * Inizializza l'HMM secondo il modello ergodico
 */
void HMM::init_ergodic(){

    /* probabilità stato iniziale */
    for(int i=0; i<numStati; i++)
        pi[i] = 1.0 / (double) numStati;

    /* matrice di transizione */
    for(int i=0; i<numStati; i++){
        for(int j=0; j<numStati; j++){
            A[i][j] = 1.0 / (double) numStati;
        }
    }

    /* matrice di emissione */
    for(int i=0; i<numStati; i++)
        for(int j=0; j<numOss; j++)
            B[i][j] = 1.0 / (double) numOss;

}

/**
 * @note Deallocare la memoria all'indirizzo alpha una volta usata la funzione!!!
 */
double* HMM::forwardProc(int* O){

    double* alpha = new double[numStati*numOss];

    /* passo inizializzazione */
    for(int s=0; s<numStati; s++)
        alpha[s][0] = pi[s]*B[O[0]];

    /* passo di induzione */
    for(int j=1; j<numOss; j++){
        for(int i=0; i<numStati; i++){
            double temp = 0;

            for(int stato=0; stato<numStati; stato++)
                temp += alpha[stato][j-1] * A[stato][i];

            alpha[i][j] = temp*B[i][O[j]];
        }
    }

    return alpha;
}

void HMM::train(Vector<int*> trainsequence);

double HMM::getProbability(int* O){

    double* alpha = forwardProc(O);

    /* passo terminazione */
    double prob = 0;
    for(int i=0; i<numStati; i++)
        prob += alpha[i][numOss -1];

    delete[] alpha;

    return prob;
}

double* HMM::backwardProc(int* O);

void HMM::print();

void HMM::save(char* filename);
void HMM::load(char* filename);


double* HMM::getA(){
    return A;
}

double* HMM::getB(){
    return B;
}

double* HMM::getPi(){
    return pi;
}

int HMM::getNumStati(){
    return numStati;
}

int HMM::getNumOss(){
    return numOss;
}
