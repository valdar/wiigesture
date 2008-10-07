
#include "chmm.h"

cHMM::cHMM(int stati, bool isErgodic, int span){

    this->numStati = stati;
    this->pi = new double[numStati];

    this->A = new double*[numStati];
    for(int i=0; i<numStati; i++)
        A[i] = new double[numStati];

    if(isErgodic)
        init_ergodic();
    else
        init_left_to_right(span);

}


void cHMM::init_left_to_right(int span){

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

}

/**
 * Inizializza l'cHMM secondo il modello ergodico
 */
void cHMM::init_ergodic(){

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

double** cHMM::forwardProc(std::vector<int> O){

    int ossSize = O.size();
    double** alpha = new double*[numStati];
    for(int i=0; i<numStati; i++)
        alpha[i] = new double[ossSize];

    /* passo inizializzazione */
    for(int s=0; s<numStati; s++)
        alpha[s][0] = pi[s] * B[s][O.at(0)];

    /* passo di induzione */
    for(int j=1; j<ossSize; j++){

        for(int i=0; i<numStati; i++){

            double temp = 0;

            for(int stato=0; stato<numStati; stato++)
                temp += alpha[stato][j-1] * A[stato][i];

            alpha[i][j] = temp*B[i][O.at(j)];
        }
    }

    return alpha;
}

void cHMM::train(std::vector< std::vector<int> > trainingset){

    for(int data=0; data<trainingset.size(); data++){

        std::vector<int> current = trainingset.at(data);
        double** alpha = forwardProc(current);
        double** beta = backwardProc(current);

        /* aggiornamento pi */
        if(isErgodic){

            double P = getProbability(alpha);

            for(int i=0; i<numStati; i++)
                pi[i] = alpha[i][1] * beta[i][1] / P;
        }

        /* aggiornamento A */
        for(int i=0; i<numStati; i++){

            for(int j=0; j<numStati; j++){

                double up = 0;
                double down = 0;

                for(int t=0; t<current.size()-2; t++){

                    up += alpha[i][t] * A[i][j] * B[j][current.at(t+1)] * beta[j][t+1];
                    down += alpha[i][t] * beta[j][t];

                }//t

                A[i][j] = up / down;

            }//j
        }//i

        /* aggiornamento B */
        for(int j=0; j<numStati; j++){

            for(int k=0; k<numOss; k++){

                double up = 0;
                double down = 0;

                for(int t=0; t<current.size()-1; t++){

                    if(current.at(t) == k)
                        up += alpha[j][t] * beta[j][t];

                    down += alpha[j][t] * beta[j][t];
                }//t

                B[j][k] = up / down;
            }//k
        }//j

    }//data
}


void cHMM::trainMS(std::vector< std::vector<int> > trainingset){

    double** A_up = new double*[numStati];
    double** A_down = new double*[numStati];
    double** B_up = new double*[numStati];
    double** B_down = new double*[numStati];

    for(int i=0; i<numStati; i++){
        A_up[i] = new double[numStati];
        A_down[i] = new double[numStati];
        B_up[i] = new double[numOss];
        B_down[i] = new double[numOss];
    }

    /* inizializza A_up, A_down, B_up, B_down a zero */
    for(int i=0; i<numStati; i++){
        for(int j=0; j<numStati; j++){
            A_up[i][j] = 0;
            A_down[i][j] = 0;
        }

        for(int k=0; k<numOss; k++){
            B_up[i][k] = 0;
            B_down[i][k] = 0;
        }
    }

    for(int data=0; data<trainingset.size(); data++){

        std::vector<int> current = trainingset.at(data);
        double** alpha = forwardProc(current);
        double** beta = backwardProc(current);

        double P = getProbability(alpha);

        /* aggiornamento pi */
        if(isErgodic){
            for(int i=0; i<numStati; i++)
                pi[i] = alpha[i][1] * beta[i][1] / P;
        }

        /* aggiornamento A */
        for(int i=0; i<numStati; i++){

            for(int j=0; j<numStati; j++){

                double up = 0;
                double down = 0;

                for(int t=0; t<current.size()-2; t++){

                    up += alpha[i][t] * A[i][j] * B[j][current.at(t+1)] * beta[j][t+1];
                    down += alpha[i][t] * beta[j][t];

                }//t

                A_up[i][j] += up / P;
                A_down[i][j] += down / P;

            }//j
        }//i


        /* aggiornamento B */
        for(int j=0; j<numStati; j++){

            for(int k=0; k<numOss; k++){

                double up = 0;
                double down = 0;

                for(int t=0; t<current.size()-1; t++){

                    if(current.at(t) == k)
                        up += alpha[j][t] * beta[j][t];

                    down += alpha[j][t] * beta[j][t];
                }//t

                B_up[j][k] += up / P;
                B_down[j][k] += down / P;

            }//k
        }//j

    }//data

    /* aggiorna le matrici A e B */
    for(int i=0; i<numStati; i++){

        for(int j=0; j<numStati; j++)
            A[i][j] = A_up[i][j] / A_down[i][j];

        for(int k=0; k<numOss; k++)
            B[i][k] = B_up[i][k] / B_down[i][k];

    }

    /* libera la memoria usata per calcoli temporanei */
     for(int i=0; i<numStati; i++){
         delete A_up[i];
         delete A_down[i];
         delete B_up[i];
         delete B_down[i];
     }

     delete A_up;
     delete A_down;
     delete B_up;
     delete B_down;

}


double cHMM::getProbability(double** alpha){

    double prob = 0;
    for(int i=0; i<numStati; i++)
        prob += alpha[i][numOss -1];

    return prob;
}


/**
 * @note Deallocare la memoria all'indirizzo beta una volta usata la funzione!!!
 */

double** cHMM::backwardProc(std::vector<int> O){
    int ossSize = O.size();
    double** beta = new double*[numStati];
    for(int i=0; i<numStati; i++)
        beta[i] = new double[ossSize];

    /* passo inizializzazione */
    for(int i=0; i<numStati; i++)
        beta[i][ossSize-1]=1;

    /* passo induzione */
    for(int j=ossSize-2; j>=0; j--){
        for(int i=0; i<numStati; i++){
            beta[i][j]=0; //inizializzazione valori matrice
            for(int stato=0; stato<numStati; stato++)
                beta[i][j] += A[i][stato]*B[stato][O.at(j+1)]*beta[stato][j+1];
        }
    }
    return beta;
}


void cHMM::print(){

    std::cout.precision(15);
    std::cout.setf(std::ios_base::scientific, std::ios_base::floatfield);

    std::cout<<"Matrice A"<<std::endl;

    for(int i=0; i<numStati; i++){
        for(int j=0; j<numStati; j++)
            std::cout <<" " << A[i][j];
        std::cout<<std::endl;
    }

    std::cout<<std::endl<<"Matrice B"<<std::endl;
    for(int i=0; i<numStati; i++){
        for(int j=0; j<numOss; j++)
            std::cout <<" " << B[i][j];
        std::cout<<std::endl;
    }

}


void cHMM::print_to_file(){

    std::ofstream outfile;
    outfile.open("out.txt");

    outfile.precision(15);
    outfile.setf(std::ios_base::scientific, std::ios_base::floatfield);

    outfile<<"Matrice A"<<std::endl;

    for(int i=0; i<numStati; i++){
        for(int j=0; j<numStati; j++)
            outfile <<" " << A[i][j];
        outfile<<std::endl;
    }

    outfile<<std::endl<<"Matrice B"<<std::endl;
    for(int i=0; i<numStati; i++){
        for(int j=0; j<numOss; j++)
            outfile <<" " << B[i][j];
        outfile<<std::endl;
    }

    outfile.close();

}



double** cHMM::getA(){
    return A;
}

double** cHMM::getB(){
    return B;
}

double* cHMM::getPi(){
    return pi;
}

int cHMM::getNumStati(){
    return numStati;
}

int cHMM::getNumOss(){
    return numOss;
}
