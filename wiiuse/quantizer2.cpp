#include "quantizer2.h"

Quantizer2::Quantizer2(){

    n_centroids = 14;
    radius = 0;
    centroids = new Sample_3d[n_centroids];

}

void Quantizer2::init(Gesture gesture){

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


void Quantizer2::train(std::vector<Gesture> gestures){
    //metagesture: contiene tutte le componenti di tutte le gesture.
    Gesture gesture;

    //costruzione della metagesture
    for(int k=0; k<gestures.size(); k++){
        std::vector<Sample_3d> curGesturData = gestures.at(k).getData();
        for(int w=0; w<curGesturData.size(); w++){
            gesture.add( curGesturData.at(w) );
        }
    }

    this->init(gesture);
    std::vector<Sample_3d> data = gesture.getData();
    int size = data.size();

     // associazioni (componente della gesture) <-> centroide
    int* assoc = new int[size];
    for(int i=0; i<size; i++)
        assoc[i] = -1;

    // n° vettori associati a ciascun centroide
    // contando il centroide stesso! (infatti si inizializza a 1)
    int numVectPerCentroid[n_centroids];
    for(int i=0; i<n_centroids; i++ )
        numVectPerCentroid[i] = 1;

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

        n_iter++;

    }
    while(modified && n_iter < 100);

}


std::vector<int> Quantizer2::getDiscreteSequence(Gesture& gesture){

    this->init(gesture);
    std::vector<Sample_3d> data = gesture.getData();
    int size = data.size();

    // associazioni (componente della gesture) <-> centroide
    std::vector<int> assoc;
    for(int i=0; i<size; i++)
        assoc.push_back(-1);

    // n° vettori associati a ciascun centroide
    int numVectPerCentroid[n_centroids];
    for(int i=0; i<n_centroids; i++ )
        numVectPerCentroid[i] = 0;


    // determina centroide a distanza minima per ogni vettore
    for(int i=0; i<size; i++){

        // minima distanza finora riscontrata
        double min_dist = std::numeric_limits<double>::max();
        // centroide a minima distanza
        int centroid_count = -1;

        for(int j=0; j<n_centroids; j++){

            // calcola la distanza dell'elemento dal centroide corrente
            double d = distance(data.at(i), centroids[j]);

            // se la nuova distanza è minore della minima finora riscontrata,
            // allora il centroide è il più vicino
            if(d < min_dist){
                min_dist = d;
                centroid_count = j;
            }

        }//j

        // associa il sample al centroide più vicino
        if(assoc.at(i) != centroid_count){
            assoc.at(i) = centroid_count;
        }
    }//i

    return assoc;

}

double Quantizer2::distance(Sample_3d a, Sample_3d b){

    return sqrt( pow(a[0] - b[0], 2.0) + pow(a[1] - b[1], 2.0) + pow(a[2] - b[2], 2.0) );

}

Quantizer2::~Quantizer2(){

    delete[] centroids;

}
