#ifndef CGAUSSIAN_H_INCLUDED
#define CGAUSSIAN_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif
/* trivariate gaussian distribution */
typedef struct gauss_3d_t {
	double mean[3];     /* mean vector */
	double covar[3][3]; /* covariance matrix */
} gauss_3d_t;


/* sample vector */
typedef struct sample_3d_t {
	float val[3];
} sample_3d_t;

}
#endif // CGAUSSIAN_H_INCLUDED
