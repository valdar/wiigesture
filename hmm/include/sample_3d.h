#ifndef SAMPLE_3D_H
#define SAMPLE_3D_H


class Sample_3d
{
    public:
        Sample_3d();
        Sample_3d(double x, double y, double z);
        double* getData() { return data; }
        double operator[](int index);

    private:
        double data[3];
};

#endif // SAMPLE_3D_H
