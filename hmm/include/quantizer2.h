#ifndef Quantizer2_H_INCLUDED
#define Quantizer2_H_INCLUDED

#include <math.h>
#include <limits>
#include <vector>
#include "gesture.h"


class Quantizer2{

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
    Quantizer2();

    // calcola la sequenza discreta a partire dalla gesture
    // ritorna un vettore di interi con valori da 0 a 13
    std::vector<int> getDiscreteSequence(Gesture& gesture);

    // addestra i centroidi
    void train(std::vector<Gesture> gestures, int considered_gestures);

    ~Quantizer2();

};

#endif // Quantizer2_H_INCLUDED
