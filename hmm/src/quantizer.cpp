#include "quantizer.h"

Quantizer::Quantizer(){

    n_centroids = 14;
    radius = 0;

}

void Quantizer::init(Gesture gesture){

    radius = (gesture.getMaxacc() + gesture.getMinacc()) / 2;

    double pi = M_PI;

    // inizializza i centroidi
    Sample_3d c0(radius, 0, 0);
    Sample_3d c1(cos(pi/4)*radius, 0, sin(pi/4)*radius);
    Sample_3d c2(0, 0, radius);
    Sample_3d c3(cos(pi*3/4)*radius, 0, sin(pi*3/4)*radius);
    Sample_3d c4(-radius, 0, 0);
    Sample_3d c5(cos(pi*5/4)*radius, 0, sin(pi*5/4)*radius);
    Sample_3d c6(0, 0, -radius);
    Sample_3d c7(cos(pi*7/4)*radius, 0, sin(pi*7/4)*radius);
    Sample_3d c8(0, radius, 0);
    Sample_3d c9(0, cos(pi/4)*radius, sin(pi/4)*radius);
    Sample_3d c10(0, cos(pi*3/4)*radius, sin(pi*3/4)*radius);
    Sample_3d c11(0, -radius, 0);
    Sample_3d c12(0, cos(pi*5/4)*radius, sin(pi*5/4)*radius);
    Sample_3d c13(0, cos(pi*7/4)*radius, sin(pi*7/4)*radius);

    map[0] = c0;
    map[1] = c1;
    map[2] = c2;
    map[3] = c3;
    map[4] = c4;
    map[5] = c5;
    map[6] = c6;
    map[7] = c7;
    map[8] = c8;
    map[9] = c9;
    map[10] = c10;
    map[11] = c11;
    map[12] = c12;
    map[13] = c13;
}

// da terminare
int* Quantizer::getDiscreteSequence(Gesture gesture){

    this->init(gesture);

    std::vector<Sample_3d> data = gesture.getData();

    int size = data.size();

    // associazioni (componente della gesture) - centroide
    int assoc[size];

    // per ogni elemento della gesture:
    for(int i=0; i<size; i++){

        // minima distanza finora riscontrata
        double min_dist = std::numeric_limits<double>::max();
        // centroide a minima distanza
        int centroid_count = -1;

        // per ogni centroide:
        for(int j=0; j<n_centroids; j++){

            // calcola la distanza dell'elemento dal centroide
            double d = distance(data.at(i), map[j]);

            if(d < min_dist){
                min_dist = d;
                centroid_count++;
            }

        }//j

        assoc[i] = centroid_count;


    }//i
}

double Quantizer::distance(Sample_3d a, Sample_3d b){

    return 0;

}
