#include "quantizer.h"

Quantizer::Quantizer(){

    n_centroids = 14;
    radius = 0;
    centroids = new Sample_3d[n_centroids];

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

    centroids[0] = c0;
    centroids[1] = c1;
    centroids[2] = c2;
    centroids[3] = c3;
    centroids[4] = c4;
    centroids[5] = c5;
    centroids[6] = c6;
    centroids[7] = c7;
    centroids[8] = c8;
    centroids[9] = c9;
    centroids[10] = c10;
    centroids[11] = c11;
    centroids[12] = c12;
    centroids[13] = c13;
}

// da terminare
int* Quantizer::getDiscreteSequence(Gesture gesture){

    this->init(gesture);
    std::vector<Sample_3d> data = gesture.getData();
    int size = data.size();

    // associazioni (componente della gesture) - centroide
    int* assoc = new int[size];

    for(int i=0; i<size; i++)
        assoc[i] = -1;

    // n° vettori associati a ciascun centroide
    int numVectPerCentroid[n_centroids];

    for(int i=0; i<n_centroids; i++ )
        numVectPerCentroid[i] = 0;


    bool modified;
    int n_iter = 0;

    do{
        // determina centroide a distanza minima per ogni vettore
        modified = false;

        for(int i=0; i<size; i++){

            // minima distanza finora riscontrata
            double min_dist = std::numeric_limits<double>::max();
            // centroide a minima distanza
            int centroid_count = -1;

            for(int j=0; j<n_centroids; j++){

                // calcola la distanza dell'elemento dal centroide corrente
                double d = distance(data.at(i), centroids[j]);

                // se la nuova distanza è minore della minima finora riscontrata,
                // aggiorna il centroide più vicino
                if(d < min_dist){
                    min_dist = d;
                    centroid_count = j;
                }

            }//j

            // associa il sample al centroide più vicino
            if(assoc[i] != centroid_count){
                assoc[i] = centroid_count;
                modified = true;
            }

        }//i


        // ricalcola posizione dei centroidi

        for(int i=0; i<size; i++){

            numVectPerCentroid[assoc[i]]++;
            centroids[assoc[i]] += data.at(i);

        }

        for(int j=0; j<n_centroids; j++)
            centroids[j] /= numVectPerCentroid[j];


        /*
        for(int i=0; i<n_centroids; i++){
            numVectPerCentroid[i] = 0;

            for(int j=0; j<size; j++){

                if(assoc[j] == i){
                    numVectPerCentroid[i]++;
                    centroids[i] += data.at(j);
                }

            }//j

            centroids[i] /= numVectPerCentroid[i];

        }//i
        */

        n_iter++;

    }
    while(modified && n_iter < 10);

    return assoc;

}

double Quantizer::distance(Sample_3d a, Sample_3d b){

    return sqrt( pow(a.getData()[0] - b.getData()[0], 2.0) + pow(a.getData()[1] - b.getData()[1], 2.0) + pow(a.getData()[2] - b.getData()[2], 2.0) );

}


bool Quantizer::equalVectors(int* a, int* b, int size){

    return false;

}

Quantizer::~Quantizer(){

    delete centroids;

}
