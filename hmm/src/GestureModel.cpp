#include "GestureModel.h"

GestureModel::GestureModel()
{   int n_stati = 5, n_simboli = 14 ,span = 1;
    this->gestureHMM = new HMM(n_stati, n_simboli, span);
    this->quant = new Quantizer2();
    this->quantizerTrained = false;
}

GestureModel::~GestureModel()
{
   delete this->gestureHMM;
   delete this->quant;
}

void GestureModel::setHMM(HMM* newHMM){
    delete this->gestureHMM;
    this->gestureHMM = newHMM;
}

HMM* GestureModel::getHMM(){
    return this->gestureHMM;
}

void GestureModel::setQuantizer(Quantizer2* newQuant){
    delete this->quant;
    this->quant = newQuant;
}

Quantizer2* GestureModel::getQuantizer(){
    return this->quant;
}

void GestureModel::trainQuantizer( Gesture metaGesture ){
    quant->train(metaGesture);
    this->quantizerTrained = true;
}

bool GestureModel::isQuantizerTrained(){
    return this->quantizerTrained;
}

void GestureModel::trainHMM(std::vector<Gesture > trainSet){
    gestureHMM->trainMS(trainSet);
}

std:vector<double > GestureModel::evalueteGestures(std::vector<Gesture > testSet){
    std:vector<double > results;
    for( int i=0; i<testSet.size(); i++ ){
        int* discretization;
        discretization = quant->getDiscreteSequence(testSet.at(i));
        std::vector<int > discVector;
        for(int j; j<(testSet.at(i)).getSize(); j++ ){
            discVector.push_back(discretization[j]);
        }

        result.push_back(gestureHMM->getP(discVector));
        delete discretization;
    }

    return result;
}

