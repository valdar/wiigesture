#ifndef HMM_H_INCLUDED
#define HMM_H_INCLUDED

#include <vector>

class HMM {

private:

    // numero di stati
	int numStati;

	// numero di simboli osservabili
	int numOss;

	// true: HMM ergodico, false: HMM left-to-right
	bool isErgodic;

	// vettore probabilità stati iniziali
	double* pi;

	// matrice transizione: prob da stato i a stato j A[i][j]
	double** A;

	// matrice emissione: prob emettere simbolo k trovandosi in stato i B[i][k]
	double** B;

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
	 * @param osservazionu Numero di simboli osservabili
     * @param isErgodic Indica se il modello sarà ergodico (true) o left-to-right (false)
     * @param span Indica, nel modello left-to-right, quanti stati sono connessi a sx e dx con lo stato corrente (default=2)
	 */
	HMM(int stati, int osservazioni, bool isErgodic, int span = 2);

	/**
	 * Addestra l'HMM a partire da un dataset di gesture
	 *
	 * @param trainingset Vettore delle gesture
	 */
	void train(std::vector< std::vector<int> > trainingset);

	/**
	 * Procedura backward.
	 *
	 * @param O La sequenza osservata.
	 * @return Array[Stato][Tempo]
	 */
	double** backwardProc(std::vector<int> O);

	void print();
    void save(char* filename);
    void load(char* filename);


	double** getA();
	double** getB();
	double* getPi();

	int getNumStati();
	int getNumOss();

};


#endif // HMM_H_INCLUDED
