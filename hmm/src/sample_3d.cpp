#include "sample_3d.h"

Sample_3d::Sample_3d()
{
    for(int i=0; i<3; i++)
        data[i] = 0;
}

Sample_3d::Sample_3d(double x, double y, double z){
    data[0] = x;
    data[1] = y;
    data[2] = z;
}

double Sample_3d::operator[](int index){
    return data[index];
}

