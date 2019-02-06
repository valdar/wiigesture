#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

#include "hmm.h"
#include "sample_3d.h"
#include "gesture.h"
#include "GestureModel.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

/**
 *TODO: leggere "Types of HMM" (266)
 *      leggere parte riguardo scaling (272 + errata)
 *      leggere riguardo training con sequenze multiple (273)
*/

std::vector<Gesture> importDataFile(char* filename){
    int module = 3; //i valori sono a 3 a 3.

    std::ifstream train_fs(filename);

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

return dataset;
}

int main(int argc, char** argv)
{
    //parsing degli input
    if(argc < 11){
        std::cout<<"USO: wiiGesture <train_file1> <train_file2> <train_file3> <validation_file1> <validation_file2> <validation_file3> <num_stati> <span> <numero_epoche> <numero_gesture_considerate>"<<std::endl;
        return 1;
    }

    char* train1 = argv[1];
    char* train2 = argv[2];
    char* train3 = argv[3];

    char* valid1 = argv[4];
    char* valid2 = argv[5];
    char* valid3 = argv[6];

    std::istringstream s_stati(argv[7]);
    int stati;
    s_stati >> stati;

    std::istringstream s_span(argv[8]);
    int span;
    s_span >> span;

    std::istringstream s_epoche(argv[9]);
    int required_epoche;
    s_epoche >> required_epoche;

    std::istringstream s_consGest(argv[10]);
    int gesture_considerate;
    s_consGest >> gesture_considerate;

    std::cout<<"numero stati: "<<stati<<" | span: "<<span<<" | epoche: "<<required_epoche<<" | gesture considerate: "<<gesture_considerate<<std::endl;

    //Costruzione dei Trainset
    std::vector<Gesture> trainset1 = importDataFile(train1);
    std::vector<Gesture> trainset2 = importDataFile(train2);
    std::vector<Gesture> trainset3 = importDataFile(train3);

    //Costruzione del Validation
    std::vector<Gesture> validationset1 = importDataFile(valid1);
    std::vector<Gesture> validationset2 = importDataFile(valid2);
    std::vector<Gesture> validationset3 = importDataFile(valid3);

    //Costruzione dei GesturModel
    GestureModel* gesture1;
    gesture1 = new GestureModel(stati, span);

    GestureModel* gesture2;
    gesture2 = new GestureModel(stati, span);

    GestureModel* gesture3;
    gesture3 = new GestureModel(stati, span);

    //Addestramento dei quantizzatore
    gesture1->trainQuantizer(trainset1, gesture_considerate);
    gesture2->trainQuantizer(trainset2, gesture_considerate);
    gesture3->trainQuantizer(trainset3, gesture_considerate);

    std::cout<<"quantizerS: OK"<<std::endl;

    //Addestramento HMMs
    for(int epoche =0; epoche<=required_epoche; epoche++){
        gesture1->trainHMM(trainset1, gesture_considerate);
        gesture2->trainHMM(trainset2, gesture_considerate);
        gesture3->trainHMM(trainset3, gesture_considerate);
    }

    std::cout<<"hmmS: TRAINED"<<std::endl;

    //Tests (validation)
    std::cout<<std::endl;

    std::vector<double> prob11 = gesture1->evaluateGestures(validationset1);
    std::vector<double> prob12 = gesture1->evaluateGestures(validationset2);
    std::vector<double> prob13 = gesture1->evaluateGestures(validationset3);

    std::vector<double> prob21 = gesture2->evaluateGestures(validationset1);
    std::vector<double> prob22 = gesture2->evaluateGestures(validationset2);
    std::vector<double> prob23 = gesture2->evaluateGestures(validationset3);

    std::vector<double> prob31 = gesture3->evaluateGestures(validationset1);
    std::vector<double> prob32 = gesture3->evaluateGestures(validationset2);
    std::vector<double> prob33 = gesture3->evaluateGestures(validationset3);

    std::cout<<"validation 1 size: "<<prob11.size()<<std::endl;
    for(int i=0; i<prob11.size(); i++){
        double a,b,c;
        a=prob11.at(i);
        b=prob21.at(i);
        c=prob31.at(i);

        double max = a;
        int max_id = 1;
        if(b>max){
            max = b;
            max_id = 2;
        }
        if(c>max){
            max = c;
            max_id = 3;
        }

        std::cout<<"validation 1 gesture "<<i<<" classe: "<<max_id<<" ("<<a<<", "<<b<<", "<<c<<")"<<std::endl;
    }

    std::cout<<"validation 2 size: "<<prob12.size()<<std::endl;
    for(int i=0; i<prob12.size(); i++){
        double a,b,c;
        a=prob12.at(i);
        b=prob22.at(i);
        c=prob32.at(i);

        double max = a;
        int max_id = 1;
        if(b>max){
            max = b;
            max_id = 2;
        }
        if(c>max){
            max = c;
            max_id = 3;
        }

        std::cout<<"validation 2 gesture "<<i<<" classe: "<<max_id<<" ("<<a<<", "<<b<<", "<<c<<")"<<std::endl;
    }

    std::cout<<"validation 3 size: "<<prob13.size()<<std::endl;
    for(int i=0; i<prob13.size(); i++){
        double a,b,c;
        a=prob13.at(i);
        b=prob23.at(i);
        c=prob33.at(i);

        double max = a;
        int max_id = 1;
        if(b>max){
            max = b;
            max_id = 2;
        }
        if(c>max){
            max = c;
            max_id = 3;
        }

        std::cout<<"validation 3 gesture "<<i<<" classe: "<<max_id<<" ("<<a<<", "<<b<<", "<<c<<")"<<std::endl;
    }



    return 0;
}
