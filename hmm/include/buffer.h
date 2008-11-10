#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include "sample_3d.h";

class Buffer{

    private:

        static vector<Sample_3d> data;

    public:

        vector<Sample_3d> getData() { return data; }
        void add(Sample_3d item) { data.push_back(item); };

}

#endif // BUFFER_H_INCLUDED
