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

        in >> word;
        std::cout << word << std::endl;

        if(word == "<gesture>"){
        // inizio di una gesture
            dataset.push_back(Gesture());
            count++;

        }
        else if(word != "<gesture>" && word != "</gesture>"){

        // dati della gesture
            std::istringstream num(word);
            double value;
            num >> value;
            temp[n_word%module] = value;

            if( (n_word % module) == 2)
                dataset.at(count-1).add(Sample_3d(temp));

            n_word++;
            //std::cout <<"ciao"<<std::endl;

        }
        else if(word == "</gesture>"){
        // fine di una gesture e sua discretizzazione

        Quantizer q;

        int current_size = dataset.at(count-1).getSize();

        int* disc_seq = q.getDiscreteSequence(dataset.at(count-1));

        discrete_dataset.push_back(disc_seq);

        for(int i=0; i<current_size -1; i++){

            out << disc_seq[i] << " ";

        }

        out << disc_seq[current_size-1] << std::endl;

        n_word = 0;

        }

    }

}
