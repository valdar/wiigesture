#ifndef HMM_H_INCLUDED
#define HMM_H_INCLUDED

#include <vector>
#include <iostream>
#include <fstream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

class HMM {

private:

    // numero di stati
	int numStati;

	// numero di simboli osservabili
	int numSimboli;

	// true: HMM ergodico, false: HMM left-to-right
	bool isErgodic;

	// n° stati raggiungibili in avanti nel modello left-to-right
	int forwardLimit;

	// vettore probabilità stati iniziali
	double* pi;

	// matrice transizione: prob da stato i a stato j A[i][j]
	boost::numeric::ublas::matrix<double> A;

	// matrice emissione: prob emettere simbolo k trovandosi in stato i B[i][k]
	boost::numeric::ublas::matrix<double> B;

    // inizializza come HMM left-to-right
	void init_left_to_right();

	// inizializza come HMM ergodico
	void init_ergodic();

	// reset
	void reset();

	/**
	 * Procedura forward.
	 *
	 * @param O la sequenza osservata
	 * @return Array[Stato][Tempo]
	 */
	void forwardProc(std::vector<int> O, boost::numeric::ublas::matrix<double>& alpha);

	/**
	 * Riporta la probabilità della sequenza osservata.
	 *
	 * @param alpha Matrice delle variabili forward (calcolabile con forwardProc)
	 * @return Probabilità della sequenza osservata O dato il modello lambda: P(O|lambda)
	 */
	double getProbability(const boost::numeric::ublas::matrix<double> &alpha);




public:

	/**
	 * Inizializza l'HMM.
	 *
	 * @param stati Numero di stati
	 * @param osservazionu Numero di simboli osservabili
     * @param isErgodic Indica se il modello sarà ergodico (true) o left-to-right (false)
     * @param span Indica, nel modello left-to-right, quanti stati sono connessi a sx e dx con lo stato corrente (default=2)
	 */
	HMM(int stati, int osservazioni, bool ergodic, int span = 2);

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

	void trainMS2(std::vector< std::vector<int> > trainingset);


	/**
	 * Procedura backward.
	 *
	 * @param O La sequenza osservata.
	 * @return Array[Stato][Tempo]
	 */
	void backwardProc(std::vector<int> O, boost::numeric::ublas::matrix<double>& beta);


	double getP(std::vector<int> O);

    /**
     * Stampa a video il contenuto delle matrici A e B
     *
     */
	void print();

	void print_to_file();


    void save(char* filename);
    void load(char* filename);


	boost::numeric::ublas::matrix<double> getA();
	boost::numeric::ublas::matrix<double> getB();
	double* getPi();

	int getNumStati();
	int getNumSimboli();

};


#endif // HMM_H_INCLUDED
