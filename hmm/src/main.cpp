#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

#include "chmm.h"
#include "hmm.h"
#include "sample_3d.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


using namespace std;

/**
 *TODO: leggere "Types of HMM" (266)
 *      leggere parte riguardo scaling (272 + errata)
 *      leggere riguardo training con sequenze multiple (273)
*/

int main(int argc, char** argv)
{
    //parsing degli input
    if(argc < 5)
        return 1;

    char* train = argv[1];
    char* valid = argv[2];

    std::istringstream s_stati(std::string(argv[3]));
    int stati;
    s_stati >> stati;

    std::istringstream s_span(std::string(argv[4]));
    int span;
    s_span >> span;

    //Costruzione del Trainset
    int module = 3; //i valori sono a 3 a 3.

    std::ifstream train_fs(train);

    std::string word; // parola correntemente acquisita

    std::vector<Gesture> dataset; // dataset di gesture (non discrete) da ricostruire

    int count = 0;// n° di gesture correntemente processate

    int n_word = 0;// n° dati acquisiti per la gesture corrente

    double temp[module];// terna di accelerazioni corrente

    while(!train_fs.eof()){

        // acquisisce la prima parola FORMATTATA
        train_fs >> word;

        // inizio di una gesture
        if(word == "<gesture>"){

            // crea una nuova gesture nel dataset
            dataset.push_back(Gesture());

            // incrementa il n° di gesture processate
            count++;

        }
        // fase acquisizione dati della gesture
        else if(word != "<gesture>" && word != "</gesture>"){

            // converte la parola acquisita in mumero
            std::istringstream num(word);
            double value;
            num >> value;

            // memorizza il numero
            temp[n_word%module] = value;

            // se ho memorizzato una terna di numeri, crea da essi un Sample_3d
            // e inseriscilo nella gesture corrente
            if( (n_word % module) == 2){
                //temp[2] = temp[2] - 1;
                dataset.at(count-1).add(Sample_3d(temp));
            }

            // incrementa il contatore delle parole processate
            n_word++;

        }
        // fine di una gesture
        else if(word == "</gesture>"){

            n_word = 0;

        }

    }

    //Costruzione del Validation

    std::ifstream valid_fs(valid);

    word = ""; // parola correntemente acquisita (azzerata per sicurezza)

    std::vector<Gesture> testset; // dataset di gesture (non discrete) da ricostruire

    count = 0;// n° di gesture correntemente processate (azzerata per sicurezza)

    n_word = 0;// n° dati acquisiti per la gesture corrente (azzerata per sicurezza)

    //double temp[module];// terna di accelerazioni corrente

    while(!valid_fs.eof()){

        // acquisisce la prima parola FORMATTATA
        valid_fs >> word;

        // inizio di una gesture
        if(word == "<gesture>"){

            // crea una nuova gesture nel dataset
            testset.push_back(Gesture());

            // incrementa il n° di gesture processate
            count++;

        }
        // fase acquisizione dati della gesture
        else if(word != "<gesture>" && word != "</gesture>"){

            // converte la parola acquisita in mumero
            std::istringstream num(word);
            double value;
            num >> value;

            // memorizza il numero
            temp[n_word%module] = value;

            // se ho memorizzato una terna di numeri, crea da essi un Sample_3d
            // e inseriscilo nella gesture corrente
            if( (n_word % module) == 2){
                //temp[2] = temp[2] - 1;
                testset.at(count-1).add(Sample_3d(temp));
            }

            // incrementa il contatore delle parole processate
            n_word++;

        }
        // fine di una gesture
        else if(word == "</gesture>"){

            n_word = 0;

        }

    }


    //Costruzione del GesturModel
    GestureModel* gesture1;
    gesture1 = new GestureModel(stati, span);

    //Addestramento del quantizzatore
    gesture1->trainQuantizer(dataset);

    std::cout<<"quantizer: OK"<<std:endl;

    //Addestramento HMM
    gesture1->trainHMM(dataset);

    std::cout<<"hmm: TRAINED"<<std:endl;

    //Tests (validation)
    std::cout<<std::endl;
    for(int g=0; g<testset.size(); g++){
        double prob;
        prob = gesture1->evaluateGestures(testset.at(g));

        std::cout<<"prob gesture "<<g<<": "<<prob<<std::endl;
    }

    return 0;
}



