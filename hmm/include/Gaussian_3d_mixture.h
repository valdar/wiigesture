#ifndef GAUSSIAN_3D_MIXTURE_H_INCLUDED
#define GAUSSIAN_3D_MIXTURE_H_INCLUDED

#include "Gaussian_3d.h"

class Gaussian_3d_mixture{

public:
    int howmany;
    Gaussian_3d* components;
    double* weight;

    Gaussian_3d_mixture(int howmany);
    Gaussian_3d_mixture(int howmany, double* weight);
    Gaussian_3d_mixture(int howmany, double* weight, Gaussian_3d* components);
};


#endif // GAUSSIAN_3D_MIXTURE_H_INCLUDED
