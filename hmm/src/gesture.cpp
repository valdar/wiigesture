Gesture::Gesture(){
    maxacc = minacc = 0;
}

Gesture::add(Sample_3d sample){

    double mod = sample.getModule();

    if(mod < minacc)
        minacc = mod;

    if(mod > maxacc)
        maxacc = mod;

    data.push_back(sample);
}
