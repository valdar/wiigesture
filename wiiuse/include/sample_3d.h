#ifndef SAMPLE_3D_H
#define SAMPLE_3D_H


/**
 * Rappresenta un vettore tridimensionale di accelerazioni.
 *
 */
class Sample_3d{

    public:
        Sample_3d();
        Sample_3d(double x, double y, double z);
        Sample_3d(double* data_3d);
        double* getData() { return data; }
        double getModule() { return module; }

        double operator[](int index);
        Sample_3d& operator+=(Sample_3d a);
        Sample_3d operator+(Sample_3d a);
        Sample_3d& operator/=(int a);
        Sample_3d operator/(int a);

    private:
        // componenti x,y,z
        double data[3];
        // modulo del vettore
        double module;

};




#endif // SAMPLE_3D_H
