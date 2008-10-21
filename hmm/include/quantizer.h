#ifndef QUANTIZER_H_INCLUDED
#define QUANTIZER_H_INCLUDED

#include <map>
#include "gesture.h"

/**
 * Classe per la quantizzazione delle gesture.
 *
 * Usa l'algoritmo k-means per classificare i sample che compongono la gesture, discretizzandola
 */
class Quantizer{

private:

    double radius;
    int n_centroids;
    map<int, Sample_3d> map;
    void kmeans(Gesture gesture);

public:
    Quantizer(int num_centroids);
    int* getDiscreteSequence(Gesture gesture);

}

#endif // QUANTIZER_H_INCLUDED
