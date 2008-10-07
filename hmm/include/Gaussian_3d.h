#ifndef GAUSSIAN_3D_H
#define GAUSSIAN_3D_H

#include <math.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


class Gaussian_3d{

public:
    double mean[3];
    boost::numeric::ublas::matrix<double> cov;

    Gaussian_3d() : cov(3,3) { init(); };
    double pdf_3d(double* x);

private:
    void init();
};

#endif // GAUSSIAN_3D_H
