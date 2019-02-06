#ifndef QUANTIZER_H_INCLUDED
#define QUANTIZER_H_INCLUDED

#include <math.h>
#include <limits>
#include "gesture.h"

/**
 * Classe per la quantizzazione delle gesture.
 *
 * Usa l'algoritmo k-means per classificare i sample che compongono la gesture, discretizzandola
 */
class Quantizer{

private:

    // raggio iniziale dei centroidi
    double radius;

    // n° di centroidi
    int n_centroids;

    // centroidi
    Sample_3d* centroids;

    // calcola la distanza tra 2 vettori
    double distance(Sample_3d a, Sample_3d b);

    // inizializza per la gesture fornita
    void init(Gesture gesture);

public:

    // costruttore
    Quantizer();

    // calcola la sequenza discreta a partire dalla gesture
    // ritorna un vettore di interi con valori da 0 a 13
    int* getDiscreteSequence(Gesture gesture);

    ~Quantizer();

};

#endif // QUANTIZER_H_INCLUDED
