#include "sample_3d.h"
#include <math.h>

Sample_3d::Sample_3d()
{
    for(int i=0; i<3; i++)
        data[i] = 0;
    module = 0;
}

Sample_3d::Sample_3d(double x, double y, double z){
    data[0] = x;
    data[1] = y;
    data[2] = z;

    module = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
}

double Sample_3d::operator[](int index){

    return data[index];

}

Sample_3d& Sample_3d::operator+=(Sample_3d a){

    this->data[0] += a[0];
    this->data[1] += a[1];
    this->data[2] += a[2];

    return *this;
}

Sample_3d Sample_3d::operator+(Sample_3d a){

        Sample_3d b = *this;
        return b += a;

}

Sample_3d& Sample_3d::operator/=(int a){

    this->data[0] /= a;
    this->data[1] /= a;
    this->data[2] /= a;

    return *this;

}

Sample_3d Sample_3d::operator/(int a){

    Sample_3d b = *this;

    return b /= a;
}

