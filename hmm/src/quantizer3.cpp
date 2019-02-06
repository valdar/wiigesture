#include "quantizer3.h"

Quantizer3::Quantizer3(){

    n_centroids = 14;
    radius = 0;
    centroids = new Sample_3d[n_centroids];

}

void Quantizer3::init(Gesture gesture){

    radius = (gesture.getMaxacc() + gesture.getMinacc()) / 2;

    double pi = M_PI;

    // inizializza i centroidi
    centroids[0] = Sample_3d(radius, 0, 0);
    centroids[1] = Sample_3d(cos(pi/4)*radius, 0, sin(pi/4)*radius);
    centroids[2] = Sample_3d(0, 0, radius);
    centroids[3] = Sample_3d(cos(pi*3/4)*radius, 0, sin(pi*3/4)*radius);
    centroids[4] = Sample_3d(-radius, 0, 0);
    centroids[5] = Sample_3d(cos(pi*5/4)*radius, 0, sin(pi*5/4)*radius);
    centroids[6] = Sample_3d(0, 0, -radius);
    centroids[7] = Sample_3d(cos(pi*7/4)*radius, 0, sin(pi*7/4)*radius);
    centroids[8] = Sample_3d(0, radius, 0);
    centroids[9] = Sample_3d(0, cos(pi/4)*radius, sin(pi/4)*radius);
    centroids[10] = Sample_3d(0, cos(pi*3/4)*radius, sin(pi*3/4)*radius);
    centroids[11] = Sample_3d(0, -radius, 0);
    centroids[12] = Sample_3d(0, cos(pi*5/4)*radius, sin(pi*5/4)*radius);
    centroids[13] = Sample_3d(0, cos(pi*7/4)*radius, sin(pi*7/4)*radius);

}

int* Quantizer3::getDiscreteSequence(Gesture gesture){

    this->init(gesture);
    std::vector<Sample_3d> data = gesture.getData();
    int size = data.size();

    // associazioni (componente della gesture) - centroide
    int* assoc = new int[size];
    for(int i=0; i<size; i++)
        assoc[i] = -1;


    for(int j=0; j<size; j++){

        double min_dist = std::numeric_limits<double>::max();
        Sample_3d item = data.at(j);

        for(int k=0; k<n_centroids; k++){

            double curr_dist = distance(item, centroids[k]);

            if(curr_dist < min_dist){
                min_dist = curr_dist;
                assoc[j] = k;
            }
        }
    }

    return assoc;

}

double Quantizer3::distance(Sample_3d a, Sample_3d b){

    return sqrt( pow(a[0] - b[0], 2.0) + pow(a[1] - b[1], 2.0) + pow(a[2] - b[2], 2.0) );

}

Quantizer3::~Quantizer3(){

    delete[] centroids;

}
