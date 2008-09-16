#ifndef HMM_H_INCLUDED
#define HMM_H_INCLUDED



public class HMM {

	private:


	int numStati;
	int numOss;
	/** p[state] */
	double* pi;
	/** from state i to state j: A[i][j] */
	double* A;
	/** The probability to emit symbol S in state i: B[i][symbolS] */
	double* B;


	void init_left-to-right(int span);
	void init_ergodic();

	/**
	 * Traditional Forward Algorithm.
	 *
	 * @param o the observationsequence O
	 * @return Array[State][Time]
	 *
	 */
	double* forwardProc(int* O);


    public:

	/**
	 * Initialize the Hidden Markov Model in a left-to-right version.
	 *
	 * @param numStates Number of states
	 * @param sigmaSize Number of observations
	 */
	HMM(int stati, int osservazioni);



	/**
	 * Trains the Hidden Markov Model with multiple sequences.
	 */
	void train(Vector<int*> trainsequence);



	/**
	 * Returns the probability that a observation sequence O belongs
	 * to this Hidden Markov Model without using the bayes classifier.
	 * Internally the well known forward algorithm is used.
	 *
	 * @param o observation sequence
	 * @return probability that sequence o belongs to this hmm
	 */
	double getProbability(int* O);

	/**
	 * Backward algorithm.
	 *
	 * @param o observation sequence o
	 * @return Array[State][Time]
	 */
	double* backwardProc(int* O);

	/**
	 * Prints everything about this model, including
	 * all values. For debug purposes or if you want
	 * to comprehend what happend to the model.
	 *
	 */
	void print();

    void save(char* filename);
    void load(char* filename);


	double* getA();
	double* getB();
	double* getPi();

	int getNumStati();
	int getNumOss();

}



#endif // HMM_H_INCLUDED
