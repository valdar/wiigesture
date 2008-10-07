#include "Gaussian_3d_mixture.h"

Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany){
    this->howmany = howmany;
}


Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany, double* weight){
    this->howmany = howmany;
    this->weight = weight;
}


Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany, double* weight, Gaussian_3d* components){
    this->howmany = howmany;
    this->weight = weight;
    this->components = components;
}
