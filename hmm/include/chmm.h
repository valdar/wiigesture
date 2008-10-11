
#ifndef HMM_H_INCLUDED
#define HMM_H_INCLUDED

#include <vector>
#include <iostream>
#include <fstream>

#include "Gaussian_3d_mixture.h"

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

	/**
	 * Procedura forward.
	 *
	 * @param O la sequenza osservata
	 * @return Array[Stato][Tempo]
	 */
	double** forwardProc(std::vector<int> O);

	/**
	 * Riporta la probabilità della sequenza osservata.
	 *
	 * @param alpha Matrice delle variabili forward (calcolabile con forwardProc)
	 * @return Probabilità della sequenza osservata O dato il modello lambda: P(O|lambda)
	 */
	double getProbability(double** alpha);


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

	/**
	 * Addestra l'HMM a partire da un dataset di gesture
	 *
	 * @param trainingset Vettore delle gesture
	 */
	void train(std::vector< std::vector<int> > trainingset);

    /**
	 * Training con sequenze multiple
	 *
	 * @param trainingset Vettore delle gesture
	 */
	void trainMS(std::vector< std::vector<int> > trainingset);


	/**
	 * Procedura backward.
	 *
	 * @param O La sequenza osservata.
	 * @return Array[Stato][Tempo]
	 */
	double** backwardProc(std::vector<int> O);

    /**
     * Stampa a video il contenuto delle matrici A e B
     *
     */
	void print();

	void print_to_file();


    void save(char* filename);
    void load(char* filename);

	double** getA();
	double** getB();
	double* getPi();

	int getNumStati();
	int getNumOss();

};


#endif // HMM_H_INCLUDED
