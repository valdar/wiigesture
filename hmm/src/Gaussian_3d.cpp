#include "Gaussian_3d.h"
#include <string>


void Gaussian_3d::zero_init(){

    for(int i=0; i<3; i++){
        mean[i] = 0.0;

        for(int j=0; j<3; j++){
            if(i==j)
                cov(i,j) = 1.0;
            else
                cov(i,j) = 0;
        }
    }
}

double Gaussian_3d::det_3d(boost::numeric::ublas::matrix<double> mat){

    return ( (mat(0,0) * (mat(1,1)*mat(2,2) - mat(1,2)*mat(2,1)))
           - (mat(1,0) * (mat(0,1)*mat(2,2) - mat(0,2)*mat(2,1)))
           + (mat(2,0) * (mat(0,1)*mat(1,2) - mat(0,2)*mat(1,1)))
           );

}

void Gaussian_3d::inv_3d(boost::numeric::ublas::matrix<double> mat, boost::numeric::ublas::matrix<double> &inv){

    double mat_det = det_3d(mat);
	double mat_det_inv = 1.0 / mat_det;

	inv(0,0) = mat_det_inv * (mat(1,1) * mat(2,2) - mat(1,2) * mat(2,1));
	inv(0,1) = mat_det_inv * (mat(0,2) * mat(2,1) - mat(0,1) * mat(2,2));
	inv(0,2) = mat_det_inv * (mat(0,1) * mat(1,2) - mat(0,2) * mat(1,1));
	inv(1,0) = mat_det_inv * (mat(1,2) * mat(2,0) - mat(1,0) * mat(2,2));
	inv(1,1) = mat_det_inv * (mat(0,0) * mat(2,2) - mat(0,2) * mat(2,0));
	inv(1,2) = mat_det_inv * (mat(0,2) * mat(1,0) - mat(0,0) * mat(1,2));
	inv(2,0) = mat_det_inv * (mat(1,0) * mat(2,1) - mat(1,1) * mat(2,0));
	inv(2,1) = mat_det_inv * (mat(0,1) * mat(2,0) - mat(0,0) * mat(2,1));
	inv(2,2) = mat_det_inv * (mat(0,0) * mat(1,1) - mat(0,1) * mat(1,0));

}

double Gaussian_3d::pdf_3d(Sample_3d x){

    /* determinante mat covarianza */
    double det = det_3d(cov);

    //std::cout<<"Determinante: "<<det<<std::endl;

    /* vettore X - Mu */
    double diff[3];
    diff[0] = x[0] - mean[0];
    diff[1] = x[1] - mean[1];
    diff[2] = x[2] - mean[2];

    /* calcolo matrice inversa delle covarianze */
    boost::numeric::ublas::matrix<double> mat_inv(3,3);
    inv_3d(cov, mat_inv);

    //std::cout<<"inversa delle covarianze: "<<mat_inv<<std::endl;

    /* calcolo esponente della funzione gaussiana: (X - Mu) * Cov^(-1) * (X - Mu)' */
    double mahalanobis_dis =
        (diff[0] * mat_inv(0,0) + diff[1] * mat_inv(1,0) + diff[2] * mat_inv(2,0)) * diff[0] +
		(diff[0] * mat_inv(0,1) + diff[1] * mat_inv(1,1) + diff[2] * mat_inv(2,1)) * diff[1] +
		(diff[0] * mat_inv(0,2) + diff[1] * mat_inv(1,2) + diff[2] * mat_inv(2,2)) * diff[2];

    //std::cout<<"esponente della gaussiana: "<<mahalanobis_dis<<std::endl;

    /* calcolo coefficiente gaussiana */
    double pre = pow((2 * M_PI), 1.5) * pow(det, 0.5);
    /* calcolo esponenziale */
    double exp = pow(M_E, -0.5 * mahalanobis_dis);

    //std::cout<<"esponenziale gaussiana: "<<exp<<std::endl;
    //std::cout<<"coefficiente gaussiana: "<<pre<<std::endl;

    return pre/exp;
}

void Gaussian_3d::rand_init(){

    int i, j;

	srand((unsigned)(time(0)));

	for (i = 0; i < 3; i++)
		mean[i] = MAX_ACC * rand() / ((double)(RAND_MAX) + 1.0);


	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			if (i == j)
				cov(i,j) = MAX_ACC * rand() / ((double)(RAND_MAX) + 1.0);
			else
				cov(i,j) = 0.0;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Gaussian_3d& g){

    os<<"Covarianze: "<<g.cov<<std::endl<<"Medie ";

    for(int i=0; i<3; i++)
        os<<g.mean[i]<<";";

    os<<std::endl;

    return os;
}
