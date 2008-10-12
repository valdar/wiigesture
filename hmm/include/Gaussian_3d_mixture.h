#ifndef GAUSSIAN_3D_MIXTURE_H_INCLUDED
#define GAUSSIAN_3D_MIXTURE_H_INCLUDED

#include <vector>

#include "Gaussian_3d.h"
#include "sample_3d.h"

class Gaussian_3d_mixture{

public:
    int howmany;
    std::vector<Gaussian_3d> components;
    double* weight;

    Gaussian_3d_mixture(int howmany);
    Gaussian_3d_mixture(int howmany, double* weight);
    Gaussian_3d_mixture(int howmany, double* weight, std::vector<Gaussian_3d> components);

    double mix_probability(Sample_3d x);

};


#endif // GAUSSIAN_3D_MIXTURE_H_INCLUDED
