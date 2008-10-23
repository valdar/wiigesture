#ifndef QUANTIZER_H_INCLUDED
#define QUANTIZER_H_INCLUDED

#include <map>
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

    // mappa dei centroidi
    std::map<int, Sample_3d> map;

    // algoritmo di clustering
    void kmeans(Gesture gesture);

    // calcola la distanza tra 2 vettori
    double distance(Sample_3d a, Sample_3d b);

    void init(Gesture gesture);

public:

    // costruttore
    Quantizer();

    // riporta la sequenza discreta a partire dalla gesture
    int* getDiscreteSequence(Gesture gesture);

};

#endif // QUANTIZER_H_INCLUDED
