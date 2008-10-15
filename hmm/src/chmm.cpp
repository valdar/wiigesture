
#include "chmm.h"

cHMM::cHMM(int stati, bool isErgodic, int gaussPerMixture, int span){

    this->numStati = stati;
    this->nGauss = gaussPerMixture;
    this->pi = new double[numStati];
    this->A.resize(numStati, numStati);

    for(int i=0; i<numStati; i++){
        mixture_vect.push_back( *(new Gaussian_3d_mixture(nGauss)) );
    }

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
                A(i,j) = 1.0;
            }
            else if(i==numStati-2 && j==numStati-2) {
                A(i,j) = 0.5;
            }
            else if(i==numStati-2 && j==numStati-1) {
                A(i,j) = 0.5;
            }
            else if(i<=j && i>j-forwardLimit-1) {
                A(i,j) = 1.0/( (double) (forwardLimit+1) );
            }
            else {
                A(i,j) = 0.0;
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
            A(i,j) = 1.0 / (double) numStati;
        }
    }

}


double cHMM::B(int stato, Sample_3d sample){

    return mixture_vect.at(stato).mix_probability(sample);

}

void cHMM::forwardProc(std::vector< Sample_3d > O, boost::numeric::ublas::matrix<double> &alpha){

    double scale[O.size()];
    forwardProc_scale(O, alpha, scale);

}

/**
 * @note scale deve avere dimensione pari a O.size()
 */

void cHMM::forwardProc_scale(std::vector< Sample_3d > O, boost::numeric::ublas::matrix<double> &alpha, double* scale){

    int i,j,t;

    // numero componenti della gesture
    int ossSize = O.size();

    // alpha: numStati x ossSize
    /* passo inizializzazione: formula 19 */
    for(i=0; i<numStati; i++){
        alpha(i,0) = pi[i] * B(i, O.at(0));
        // debug
        //std::cout<<B(i, O.at(0))<<std::endl;
    }

    // debug
    /*
    std::cout<<"Alpha in forward_proc_scale dopo ciclo iniziale:"<<std::endl;
    std::cout<<alpha<<std::endl;
    */

    // scaling iniziale: formula 92a
    scale[0] = 0;
    for(i=0; i<numStati; i++)
        scale[0] += alpha(i,0);
    for(i=0; i<numStati; i++)
        alpha(i,0) = alpha(i,0) / scale[0];


    /* passo di induzione: formula 20 */
    for(t=1; t<ossSize; t++){

        scale[t] = 0;

        for(j=0; j<numStati; j++){

            double temp = 0;

            for(i=0; i<numStati; i++)
                temp += alpha(i, t-1) * A(i,j);

            alpha(j,t) = temp*B(j, O.at(t));

            // calcola il coefficiente di scaling corrente
            scale[t] += alpha(j,t);
        }

        // scaling
        for(j=0; j<numStati; j++){
            alpha(j,t) = alpha(j,t) / scale[t];
        }
    }

    // debug
    /*
    std::cout<<"alpha dopo lo scaling: "<<alpha<<std::endl;
    */

}


double cHMM::getProbabilityFromAlpha(boost::numeric::ublas::matrix<double> alpha){

    double prob = 0;
    for(int i=0; i<numStati; i++)
        prob += alpha(i, alpha.size2()-1);

    return prob;
}

double cHMM::getProbabilityFromScale(double* scale, int size){
	double prob = 1;
	for(int i=0; i<size; i++)
		prob *= scale[i];

	return 1/prob;
}


void cHMM::backwardProc(std::vector< Sample_3d > O, boost::numeric::ublas::matrix<double> &beta, double* scale){
    int ossSize = O.size();

    /* passo inizializzazione con scaling */
    for(int i=0; i<numStati; i++)
        beta(i,ossSize-1) = 1 / scale[ossSize-1];

    /* passo induzione */
    for(int t=ossSize-2; t>=0; t--){

        for(int i=0; i<numStati; i++){

            //inizializzazione valori matrice
            beta(i,t)=0;

            for(int stato=0; stato<numStati; stato++){
                // calcolo beta
                beta(i,t) += A(i,stato) * B(stato,O.at(t+1)) * beta(stato,t+1);
                // scaling
                beta(i,t) = beta(i,t) / scale[t];
            }

        }
    }
}

void cHMM::train(std::vector< std::vector<Sample_3d> > trainingset){

    for(int data=0; data<trainingset.size(); data++){

        std::vector<Sample_3d> current = trainingset.at(data);
        boost::numeric::ublas::matrix<double> alpha(numStati, current.size());
        boost::numeric::ublas::matrix<double> beta(numStati, current.size());

        double scale[current.size()];

        forwardProc_scale(current, alpha, scale);
        backwardProc(current, beta, scale);

        // debug
        /*
        std::cout<< "scale:" << std::endl;
        for(int d=0; d<current.size(); d++)
            std::cout<<scale[d]<<std::endl;
        */

        // debug
        /*
        std::cout<< "alpha:" << std::endl;
        std::cout<< alpha << std::endl;
        std::cout<< "beta:" << std::endl;
        std::cout<< beta << std::endl;
        */

        // aggiornamento pi
        if(isErgodic){

            double P = getProbabilityFromScale(scale, (int)current.size());

            for(int i=0; i<numStati; i++)
                pi[i] = alpha(i,1) * beta(i,1) / P;
        }

        // aggiornamento A
        for(int i=0; i<numStati; i++){

            for(int j=0; j<numStati; j++){

                double up = 0;
                double down = 0;

                for(int t=0; t<current.size()-1; t++){

                    up += alpha(i,t) * A(i,j) * B(j,current.at(t+1)) * beta(j,t+1);
                    down += alpha(i,t) * beta(j,t);

                }//t

                A(i,j) = up / down;

            }//j
        }//i

        // aggiornamento parametri gaussiane
        // gamma[t][j][k]
        double ***gamma;
        gamma = (double ***)malloc(current.size() * sizeof(double **));
        for (int t = 0; t < current.size(); t++)
        {
            gamma[t] = (double **)malloc(numStati * sizeof(double *));
            for (int j = 0; j < numStati; j++)
            {
			gamma[t][j] = (double *)malloc(mixture_vect.at(j).howmany * sizeof(double));
            }
        }

        // calcolo gamma
        for(int t=0; t<current.size(); t++){ // ciclo sui sample della gesture

            double sum = 0;
            for(int j=0; j<numStati; j++){
                sum += alpha(j,t)*beta(j,t);
            }

            for(int j=0; j<numStati; j++){ // ciclo sugli stati

                for(int k=0; k<mixture_vect.at(j).howmany; k++){ // ciclo sulle componenti della mixture dello stato corrente

                    gamma[t][j][k] = alpha(j,t) * beta(j,t) * mixture_vect.at(j).weight[k] *
                                        mixture_vect.at(j).components.at(k).pdf_3d(current.at(t)) /
                                        ( sum * B(j, current.at(t)) );

                }
            }

        }

        // aggiornamento pesi misture: mixture_vect.at(STATO).weight[MISTURA]
        for(int j=0; j<numStati; j++){

            int n_mix = mixture_vect.at(j).howmany;

            for(int k=0; k<n_mix; k++){

                double up = 0, down = 0;

                for(int t=0; t<current.size(); t++){

                    up += gamma[t][j][k];

                    for(int m=0; m<n_mix; m++){

                        down += gamma[t][j][m];
                    }

                }

                mixture_vect.at(j).weight[k] = up / down;

            }
        }

        // aggiornamento medie
        for(int j=0; j<numStati; j++){

            int n_mix = mixture_vect.at(j).howmany;

            for(int k=0; k<n_mix; k++){

                double down = 0;

                for(int t=0; t<current.size(); t++){

                    down += gamma[t][j][k];

                }

                for(int n=0; n<3; n++){

                    double up = 0;

                    for(int t=0; t<current.size(); t++){

                        up += gamma[t][j][k] * current.at(t)[n];

                    }

                    mixture_vect.at(j).components.at(k).mean[n] = up / down;

                }
            }
        }

        // aggiornamento covarianze
        for(int j=0; j<numStati; j++){

            int n_mix = mixture_vect.at(j).howmany;

            for(int k=0; k<n_mix; k++){

                double down = 0;

                for(int t=0; t<current.size(); t++){

                    down += gamma[t][j][k];

                }

                for(int n=0; n<3; n++){

                    double up = 0;

                    for(int t=0; t<current.size(); t++){

                        up += gamma[t][j][k] *
                                (current.at(t)[n] - mixture_vect.at(j).components.at(k).mean[n]) *
                                (current.at(t)[n] - mixture_vect.at(j).components.at(k).mean[n]);

                    }

                    // nota: si aggiornano solo le covarianze sulla diagonale, in quanto lavoriamo
                    // con matrici di covarianza diagonali
                    mixture_vect.at(j).components.at(k).cov(n,n) = up / down;

                }
            }
        }

        // può servire check su covarianze, cioè se cov(i,i) < K, allora cov(i,i) = K

        // libera la memoria
        for (int t = 0; t < current.size(); t++){
            for (int j = 0; j < numStati; j++){
                free(gamma[t][j]);
            }
            free(gamma[t]);
        }
        free(gamma);

    }//data
}


void cHMM::trainMS(std::vector< std::vector<Sample_3d> > trainingset){

    boost::numeric::ublas::matrix<double> A_up(numStati, numStati);
    boost::numeric::ublas::matrix<double> A_down(numStati, numStati);

    std::vector<Gaussian_3d_mixture> gaussians_up;
    std::vector<Gaussian_3d_mixture> gaussians_down;

    // inizializza A_up, A_down
    for(int i=0; i<numStati; i++){
        for(int j=0; j<numStati; j++){
            A_up(i,j) = 0;
            A_down(i,j) = 0;
        }
    }

    // inizializa gaussians_up, gaussians_down con tutti zero
    for(int k=0; k<numStati; k++){

        Gaussian_3d_mixture* g1 = new Gaussian_3d_mixture(nGauss, true);
        Gaussian_3d_mixture* g2 = new Gaussian_3d_mixture(nGauss, true);

        gaussians_up.push_back( *g1 );
        gaussians_down.push_back( *g2 );

    }

    for(int data=0; data<trainingset.size(); data++){

        std::vector<Sample_3d> current = trainingset.at(data);
        double scale[current.size()];
        boost::numeric::ublas::matrix<double> alpha(numStati, current.size());
        boost::numeric::ublas::matrix<double> beta(numStati, current.size());
        forwardProc_scale(current, alpha, scale);
        backwardProc(current, beta, scale);

        double P = getProbabilityFromScale(scale, current.size());

        // aggiornamento pi
        if(isErgodic){
            for(int i=0; i<numStati; i++)
                pi[i] = alpha(i,1) * beta(i,1) / P;
        }

        // aggiornamento A
        for(int i=0; i<numStati; i++){

            for(int j=0; j<numStati; j++){

                double up = 0;
                double down = 0;

                for(int t=0; t<current.size()-2; t++){

                    up += alpha(i,t) * A(i,j) * B(j,current.at(t+1)) * beta(j,t+1);
                    down += alpha(i,t) * beta(j,t);

                }//t

                A_up(i,j) += up / P;
                A_down(i,j) += down / P;

            }//j
        }//i


        // aggiornamento parametri gaussiane
        // gamma[t][j][k]
        double ***gamma;
        gamma = (double ***)malloc(current.size() * sizeof(double **));
        for (int t = 0; t < current.size(); t++)
        {
            gamma[t] = (double **)malloc(numStati * sizeof(double *));
            for (int j = 0; j < numStati; j++)
            {
			gamma[t][j] = (double *)malloc(mixture_vect.at(j).howmany * sizeof(double));
            }
        }

        // calcolo gamma
        for(int t=0; t<current.size(); t++){ // ciclo sui sample della gesture

            double sum = 0;
            for(int j=0; j<numStati; j++){
                sum += alpha(j,t)*beta(j,t);
            }

            for(int j=0; j<numStati; j++){ // ciclo sugli stati

                for(int k=0; k<mixture_vect.at(j).howmany; k++){ // ciclo sulle componenti della mixture dello stato corrente

                    gamma[t][j][k] = alpha(j,t) * beta(j,t) * mixture_vect.at(j).weight[k] *
                                        mixture_vect.at(j).components.at(k).pdf_3d(current.at(t)) /
                                        ( sum * B(j, current.at(t)) );

                }
            }

        }


        // aggiornamento pesi misture: mixture_vect.at(STATO).weight[MISTURA]
        for(int j=0; j<numStati; j++){

            int n_mix = mixture_vect.at(j).howmany;

            for(int k=0; k<n_mix; k++){

                double up = 0, down = 0;

                for(int t=0; t<current.size(); t++){

                    up += gamma[t][j][k];

                    for(int m=0; m<n_mix; m++){

                        down += gamma[t][j][m];
                    }

                }

                gaussians_up.at(j).weight[k] += up / P;
                gaussians_down.at(j).weight[k] += down / P;

                //mixture_vect.at(j).weight[k] = up / down;

            }
        }

        // aggiornamento medie
        for(int j=0; j<numStati; j++){

            int n_mix = mixture_vect.at(j).howmany;

            for(int k=0; k<n_mix; k++){

                double down = 0;

                for(int t=0; t<current.size(); t++){

                    down += gamma[t][j][k];

                }

                for(int n=0; n<3; n++){

                    double up = 0;

                    for(int t=0; t<current.size(); t++){

                        up += gamma[t][j][k] * current.at(t)[n];

                    }

                    gaussians_up.at(j).components.at(k).mean[n] += up / P;
                    gaussians_down.at(j).components.at(k).mean[n] += down / P;

                    //mixture_vect.at(j).components.at(k).mean[n] = up / down;

                }
            }
        }

        // aggiornamento covarianze
        for(int j=0; j<numStati; j++){

            int n_mix = mixture_vect.at(j).howmany;

            for(int k=0; k<n_mix; k++){

                double down = 0;

                for(int t=0; t<current.size(); t++){

                    down += gamma[t][j][k];

                }

                for(int n=0; n<3; n++){

                    double up = 0;

                    for(int t=0; t<current.size(); t++){

                        up += gamma[t][j][k] *
                                (current.at(t)[n] - mixture_vect.at(j).components.at(k).mean[n]) *
                                (current.at(t)[n] - mixture_vect.at(j).components.at(k).mean[n]);

                    }

                    // nota: si aggiornano solo le covarianze sulla diagonale, in quanto lavoriamo
                    // con matrici di covarianza diagonali
                    gaussians_up.at(j).components.at(k).cov(n,n) += up / P;
                    gaussians_down.at(j).components.at(k).cov(n,n) += down / P;

                    //mixture_vect.at(j).components.at(k).cov(n,n) = up / down;

                }
            }
        }

        // può servire check su covarianze, cioè se cov(i,i) < K, allora cov(i,i) = K

        // libera la memoria
        for (int t = 0; t < current.size(); t++){
            for (int j = 0; j < numStati; j++){
                free(gamma[t][j]);
            }
            free(gamma[t]);
        }
        free(gamma);

    }//data

    // aggiorna le matrici A e tutti i parametri delle gaussiane
    for(int i=0; i<numStati; i++){ // cicla sugli stati

        // aggiorna la matrice A
        for(int j=0; j<numStati; j++)
            A(i,j) = A_up(i,j) / A_down(i,j);

        // aggiorna i parametri delle gaussiane
        for(int k=0; k<mixture_vect.at(i).howmany; k++){

            // aggiorna i pesi
            mixture_vect.at(i).weight[k] = gaussians_up.at(i).weight[k] / gaussians_down.at(i).weight[k];

            for(int n=0; n<3; n++){ // cicla sulle dimensioni della gaussiana (3D)

                // aggiorna le medie
                mixture_vect.at(i).components.at(k).mean[n] = gaussians_up.at(i).components.at(k).mean[n] / gaussians_down.at(i).components.at(k).mean[n];

                // aggiorna le covarianze
                // nota: si aggiornano solo quelle sulla diagonale, in quanto si lavora, per ipotesi, con matrici diagonali
                mixture_vect.at(i).components.at(k).cov(n,n) = gaussians_up.at(i).components.at(k).cov(n,n) / gaussians_down.at(i).components.at(k).cov(n,n);
            }

        }

    }


}


void cHMM::print(){

    std::cout.precision(15);
    std::cout.setf(std::ios_base::scientific, std::ios_base::floatfield);

    std::cout<<"Matrice A"<<std::endl;

    for(int i=0; i<numStati; i++){
        for(int j=0; j<numStati; j++)
            std::cout <<" " << A(i,j);
        std::cout<<std::endl;
    }

    std::cout<<"Parametri gaussiane:"<<std::endl;
    for(int i=0; i<mixture_vect.size(); i++){
        for(int j=0; j<mixture_vect.at(i).howmany; j++){
            std::cout<<"Mistura "<<i<<" - gaussiana "<<j<<":"<<std::endl;
            std::cout<<mixture_vect.at(i).components.at(j)<<std::endl;
        }
    }

    /*
    std::cout<<std::endl<<"funzione B"<<std::endl;
    for(int i=0; i<numStati; i++){
        for(int j=0; j<numOss; j++)
            std::cout <<" " << B(i,mixture_vect.at(j));
        std::cout<<std::endl;
    }
    */

}


void cHMM::print_to_file(){

    std::ofstream outfile;
    outfile.open("out.txt");

    outfile.precision(15);
    outfile.setf(std::ios_base::scientific, std::ios_base::floatfield);

    outfile<<"Matrice A"<<std::endl;

    for(int i=0; i<numStati; i++){
        for(int j=0; j<numStati; j++)
            outfile <<" " << A(i,j);
        outfile<<std::endl;
    }

    /*
    outfile<<std::endl<<"Matrice B"<<std::endl;
    for(int i=0; i<numStati; i++){
        for(int j=0; j<numOss; j++)
            outfile <<" " << B(i,mixture_vect.at(j));
        outfile<<std::endl;
    }
    */

    outfile.close();

}

boost::numeric::ublas::matrix<double> cHMM::getA(){
    return A;
}

double* cHMM::getPi(){
    return pi;
}

int cHMM::getNumStati(){
    return numStati;
}
