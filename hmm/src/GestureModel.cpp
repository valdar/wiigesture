#include "GestureModel.h"

GestureModel::GestureModel()
{   int n_stati = 5, n_simboli = 14 ,span = 1;
    this->gestureHMM = new HMM(n_stati, n_simboli, span);
    this->quant = new Quantizer2();
    this->quantizerTrained = false;
}

GestureModel::GestureModel(int n_stati, int span)
{   int n_simboli = 14;
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

void GestureModel::trainQuantizer( std::vector<Gesture> dataset ){
    quant->train(dataset);
    this->quantizerTrained = true;
}

bool GestureModel::isQuantizerTrained(){
    return this->quantizerTrained;
}

void GestureModel::trainHMM(std::vector<Gesture > trainSet){
    std::vector<std::vector<int> > discreteTrainSet;
    for(int i=0; i<trainSet.size(); i++){
        discreteTrainSet.push_back( quant->getDiscreteSequence(trainSet.at(i)) );
    }
    gestureHMM->trainMS( discreteTrainSet );
}

std::vector<double > GestureModel::evaluateGestures(std::vector<Gesture >& testSet){
    std::vector<double > results;
    for( int i=0; i<testSet.size(); i++ ){
        //std::vector<int> discVector = quant->getDiscreteSequence(testSet.at(i));

        results.push_back(gestureHMM->getP( quant->getDiscreteSequence(testSet.at(i)) ));
    }

    return results;
}

