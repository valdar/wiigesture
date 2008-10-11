#include "Gaussian_3d_mixture.h"

Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany){
    this->howmany = howmany;
    weight = new double[this->howmany];

    for(int i=0; i<this->howmany; i++){
        weight[i] = (double) 1.0 / this->howmany;
        components.push(*(new Gaussian_3d(true);));
    }
}


Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany, double* weight){
    this->howmany = howmany;
    this->weight = weight;
     for(int i=0; i<this->howmany; i++){
        components.push(*(new Gaussian_3d(true);));
    }
}


Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany, double* weight, vector<Gaussian_3d> components){
    this->howmany = howmany;
    this->weight = weight;
    this->components = components;
}

