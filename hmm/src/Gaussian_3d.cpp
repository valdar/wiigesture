#include "Gaussian_3d.h"


void Gaussian_3d::init()
{

    for(int i=0; i<3; i++){
        mean[i] = 0.0;

        for(int j=0; j<3; j++)
            cov(i,j) = 0.0;
    }
}

double Gaussian_3d::pdf_3d(double* x){


    double det = 0;
    double pre = 1 / ( pow((2 * M_PI), 1.5) * pow(det, 0.5) );

    return 2.0;
}
