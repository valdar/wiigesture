
#ifndef HMM_H_INCLUDED
#define HMM_H_INCLUDED

#include <vector>
#include <iostream>
#include <fstream>

#include "Gaussian_3d_mixture.h"
#include "sample_3d.h"

class cHMM {

private:

    // numero di stati
	int numStati;

	// true: HMM ergodico, false: HMM left-to-right
	bool isErgodic;

	// vettore probabilità stati iniziali
	double* pi;

	// matrice transizione: prob da stato i a stato j A[i][j]
	boost::numeric::ublas::matrix<double> A;

    // vettore contenente una mistura di gaussiane per ogni stato
	std::vector<Gaussian_3d_mixture> mixture_vect;

    // inizializza come HMM left-to-right
	void init_left_to_right(int span);

	// inizializza come HMM ergodico
	void init_ergodic();

    // probability density function
	double B(int stato, Sample_3d sample);

	/**
	 * Procedura forward.
	 *
	 * Calcola i coefficienti alpha, usando scaling
	 *
	 * @param O la sequenza osservata
	 * @param alpha matrice dei coefficienti da calcolare
	 */
	void forwardProc(std::vector< Sample_3d > O, boost::numeric::ublas::matrix<double> &alpha);

    /**
	 * Procedura forward.
	 *
	 * Calcola i coefficienti alpha, usando scaling, e salva i coefficienti di scaling
	 *
	 * @param O la sequenza osservata
	 * @param alpha matrice dei coefficienti da calcolare
	 * @param scale vettore coefficienti di scaling
	 */
	void forwardProc_scale(std::vector< Sample_3d > O, boost::numeric::ublas::matrix<double> &alpha, double* scale);

	/**
	 * Riporta la probabilità della sequenza osservata.
	 *
	 * @param alpha Matrice delle variabili forward (calcolabile con forwardProc)
	 * @return Probabilità della sequenza osservata O dato il modello lambda: P(O|lambda)
	 */
	double getProbabilityFromAlpha(boost::numeric::ublas::matrix<double> alpha);

	double getProbabilityFromScale(double* scale, int size);

	/**
	 * Procedura backward.
	 *
	 * @param O La sequenza osservata.
	 * @param beta matrice che conterrà il risultato [Stato][Tempo]
	 * @param scale vettore dei coefficienti di scaling, calcolati nella procedura forward
	 */
	void backwardProc(std::vector< Sample_3d > O, boost::numeric::ublas::matrix<double> &beta, double* scale);


public:

	/**
	 * Inizializza l'HMM.
	 *
	 * @param stati Numero di stati
     * @param isErgodic Indica se il modello sarà ergodico (true) o left-to-right (false)
     * @param gaussPerMixture numero di gaussiane per mistura (default=1)
     * @param span Indica, nel modello left-to-right, quanti stati sono connessi a sx e dx con lo stato corrente (default=2)
	 */
	cHMM(int stati, bool isErgodic, int gaussPerMixture = 1, int span = 2);


    double getProbability(std::vector< Sample_3d > O);

	/**
	 * Addestra l'HMM a partire da un dataset di gesture
	 *
	 * @param trainingset Vettore delle gesture
	 */
	void train(std::vector< std::vector< Sample_3d > > trainingset);

    /**
	 * Training con sequenze multiple
	 *
	 * @param trainingset Vettore delle gesture
	 */
	void trainMS(std::vector< std::vector< Sample_3d > > trainingset);

    /**
     * Stampa a video il contenuto delle matrici A e B
     *
     */
	void print();

	void print_to_file();


    void save(char* filename);
    void load(char* filename);

	boost::numeric::ublas::matrix<double> getA();
	double* getPi();
	int getNumStati();


};


#endif // HMM_H_INCLUDED
