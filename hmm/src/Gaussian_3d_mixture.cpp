#include "Gaussian_3d_mixture.h"

Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany){
    this->howmany = howmany;
    weight = new double[this->howmany];

    for(int i=0; i<this->howmany; i++){
        weight[i] = (double) 1.0 / this->howmany;
        components.push_back(*(new Gaussian_3d(true)));
    }
}


Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany, double* weight){
    this->howmany = howmany;
    this->weight = weight;
     for(int i=0; i<this->howmany; i++){
        components.push_back(*(new Gaussian_3d(true)));
    }
}


Gaussian_3d_mixture::Gaussian_3d_mixture(int howmany, double* weight, std::vector<Gaussian_3d> components){
    this->howmany = howmany;
    this->weight = weight;
    this->components = components;
}

double Gaussian_3d_mixture::mix_probability(Sample_3d x){

    double sum = 0;

    for(int i=0; i<3; i++)
        sum += weight[i] * components.at(i).pdf_3d(x);

    return sum;
}

