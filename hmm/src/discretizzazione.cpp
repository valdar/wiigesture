#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "quantizer.h"
#include "quantizer2.h"
#include "gesture.h"

// uso: prog_name <input_file> <output_file>
// dove <input_file> contiene le gesture da discretizzare
//      <output_file> conterrà le gesture discrete

int main(int argc, char** argv){

    char* inputfile = argv[1];
    char* outputfile = argv[2];

    int module = 3;

    std::ifstream in(inputfile);
    std::ofstream out(outputfile);

    // parola correntemente acquisita
    std::string word;

    // dataset di gesture (non discrete) da ricostruire
    std::vector<Gesture> dataset;

    // nuovo dataset discretizzato
    std::vector< std::vector<int> > discrete_dataset;

    // n° di gesture correntemente processate
    int count = 0;

    // n° dati acquisiti per la gesture corrente
    int n_word = 0;

    // terna di accelerazioni corrente
    double temp[module];

    while(!in.eof()){

        // acquisisce la prima parola FORMATTATA
        in >> word;

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
        /*
        Quantizer q;
        int current_size = dataset.at(count-1).getSize();

        // calcola la sequenza discreta a partire dalla gesture
        int* disc_seq = q.getDiscreteSequence(dataset.at(count-1));
        discrete_dataset.push_back(disc_seq);

        // salva la gesture discreta sul file di output

        out << "<gesture>" << std::endl;

        for(int i=0; i<current_size -1; i++){

            out << disc_seq[i] << " ";

        }

        out << disc_seq[current_size-1] << std::endl << "</gesture>" << std::endl;
        */
        // azzera il contatore delle parole processate
        n_word = 0;

        }

    }

    Gesture globalGesture;
    int dataset_size = dataset.size();

    for(int i=0; i<dataset_size; i++){

        Gesture current_gesture = dataset.at(i);

        int curr_size = current_gesture.getSize();

        for(int j=0; j<curr_size; j++){
            globalGesture.add(current_gesture.getSample(j));
        }

    }

    Quantizer2 q;
    q.train(globalGesture);

    for(int k=0; k<dataset_size; k++){

        // calcola la sequenza discreta a partire dalla gesture
        std::vector<int> disc_seq = q.getDiscreteSequence(dataset.at(k));
        discrete_dataset.push_back(disc_seq);

        int gesture_size = dataset.at(k).getSize();

        // salva la gesture discreta sul file di output
        out << "<gesture>" << std::endl;

        for(int i=0; i<gesture_size -1; i++){

            out << disc_seq[i] << " ";

        }

        out << disc_seq[gesture_size-1] << std::endl << "</gesture>" << std::endl;

    }


}

