#ifndef GESTUREMODEL_H
#define GESTUREMODEL_H

#include <vector>

#include "quantizer2.h"
#include "hmm.h"
#include "gesture.h"

class GestureModel
{
    public:
        GestureModel();
        GestureModel(int n_stati, int span);
        virtual ~GestureModel();

        void setHMM(HMM* newHMM);
        HMM* getHMM();
        void setQuantizer(Quantizer2* newQuant);
        Quantizer2* getQuantizer();

        void trainQuantizer( Gesture metaGesture );
        bool isQuantizerTrained();

        void trainHMM(std::vector<Gesture > trainSet);
        std::vector<double > evaluateGestures(std::vector<Gesture > testSet);

    protected:
    private:
        HMM* gestureHMM;
        Quantizer2* quant;
        bool quantizerTrained;
};

#endif // GESTUREMODEL_H
