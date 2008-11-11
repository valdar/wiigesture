#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "quantizer.h"
#include "gesture.h"


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
    std::vector<int*> discrete_dataset;

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
        // fine di una gesture e sua discretizzazione
        else if(word == "</gesture>"){

        Quantizer q;
        int current_size = dataset.at(count-1).getSize();

        // calcola la sequenza discreta a partire dalla gesture
        int* disc_seq = q.getDiscreteSequence(dataset.at(count-1));
        discrete_dataset.push_back(disc_seq);

        // salva la gesture discreta sul file di output
        for(int i=0; i<current_size -1; i++){

            out << disc_seq[i] << " ";

        }

        out << disc_seq[current_size-1] << std::endl;

        // azzera il contatore delle parole processate
        n_word = 0;

        }

    }

}
