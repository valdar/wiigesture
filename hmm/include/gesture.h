#ifndef GESTURE_H_INCLUDED
#define GESTURE_H_INCLUDED

#include <vector>
#include "sample_3d.h"

class Gesture{

private:

    // massima accelerazione (in modulo) tra le componenti della gesture
    double maxacc;
    // minima accelerazione
    double minacc;
    // componenti della gesture
    std::vector<Sample_3d> data;

public:

    Gesture();
    void add(Sample_3d sample);

    int getSize() { return data.size(); }
    std::vector<Sample_3d> getData() { return data; }
    Sample_3d getSample(int index) { return data.at(index); }
    double getMaxacc() { return maxacc; }
    double getMinacc() { return minacc; }

};

#endif
