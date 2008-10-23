#include "gesture.h"

Gesture::Gesture(){
    maxacc = minacc = 0;
}

void Gesture::add(Sample_3d sample){

    double mod = sample.getModule();

    if(mod < minacc)
        minacc = mod;

    if(mod > maxacc)
        maxacc = mod;

    data.push_back(sample);
}
