#ifndef GESTUREMODEL_H
#define GESTUREMODEL_H

#include <vector>

#include "quantizer2"
#include "hmm"
#include "gesture"

class GestureModel
{
    public:
        GestureModel();
        virtual ~GestureModel();

        void setHMM(HMM* newHMM);
        HMM* getHMM();
        void setQuantizer(Quantizer2* newQuant);
        Quantizer2* getQuantizer();

        void trainQuantizer( Gesture metaGesture );
        bool isQuantizerTrained();

        void trainHMM(std::vector<Gesture > trainSet);
        std:vector<double > evalueteGestures(std::vector<Gesture > testSet);

    protected:
    private:
        HMM* gestureHMM;
        Quantizer2* quant;
        bool quantizerTrained;
};

#endif // GESTUREMODEL_H
