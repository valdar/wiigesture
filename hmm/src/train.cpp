#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "hmm.h"

// uso: prog_name <train_file> <test_file>
// dove <train_file> contiene il train set di gesture
//      <test_file> contiene le gesture da riconoscere (validation set)

int main(int argc, char** argv){

    if(argc < 3)
        exit(EXIT_FAILURE);

    // file di input
    char* train = argv[1];
    char* test = argv[2];
    std::ifstream trainfile(train);
    std::ifstream validfile(test);

    // HMM
    int n_stati = 8, n_simboli = 14;
    HMM hmm(n_stati, n_simboli);

    // dataset di train e validation
    std::vector< std::vector<int> > trainset;
    std::vector< std::vector<int> > validset;

    // stringhe per acquisizione temporanea dati
    std::string train_input, valid_input;

    // conta il n° di gesture processate
    int count = 0;

    // acquisizione training set
    while(!trainfile.eof()){

        // acquisisce la prima parola FORMATTATA
        trainfile >> train_input;

        // inizio di una gesture
        if(train_input == "<gesture>"){

            // crea una nuova gesture nel dataset
            trainset.push_back(std::vector<int>());

            // incrementa il n° di gesture processate
            count++;

        }
        // fase acquisizione dati della gesture
        else if(train_input != "<gesture>" && train_input != "</gesture>"){

            // converte la parola acquisita in mumero
            std::istringstream num(train_input);
            double value;
            num >> value;

            // inserisce il valore nella gesture corrente
            trainset.at(count-1).push_back(value);

        }
        // fine di una gesture
        else if(train_input == "</gesture>"){

        }

    }

    // una volta costruito il training set, addestro l'hmm
    hmm.trainMS(trainset);

    // poi testo col validation set
    count = 0;
    while(!validfile.eof()){

        // acquisisce la prima parola FORMATTATA
        validfile >> valid_input;

        // inizio di una gesture
        if(valid_input == "<gesture>"){

            // crea una nuova gesture nel dataset
            validset.push_back(std::vector<int>());

            // incrementa il n° di gesture processate
            count++;

        }
        // fase acquisizione dati della gesture
        else if(valid_input != "<gesture>" && valid_input != "</gesture>"){

            // converte la parola acquisita in mumero
            std::istringstream num(valid_input);
            double value;
            num >> value;

            // inserisce il valore nella gesture corrente
            validset.at(count-1).push_back(value);

        }
        // fine di una gesture: testa HMM su di essa
        else if(train_input == "</gesture>"){

            double P = hmm.getP(validset.at(count-1));
            std::cout << "Gesture " << count << " Prob: " << P << std::endl;

        }

    }

}
