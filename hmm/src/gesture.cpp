#include "gesture.h"

Gesture::Gesture(){
    maxacc = std::numeric_limits<double>::min();
    minacc = std::numeric_limits<double>::max();
}

Gesture::Gesture(std::vector<Sample_3d> data){
    Gesture();
    this->data = data;

    int size = data.size();

    for(int i=0; i<size; i++){

        double mod = data.at(i).getModule();

        if(mod < minacc)
            minacc = mod;

        if(mod > maxacc)
            maxacc = mod;

    }
}

void Gesture::add(Sample_3d sample){

    double mod = sample.getModule();

    if(mod < minacc)
        minacc = mod;

    if(mod > maxacc)
        maxacc = mod;

    data.push_back(sample);
}
