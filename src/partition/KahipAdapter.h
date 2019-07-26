//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_KAHIPADAPTER_H
#define ALLPAIRREACH_KAHIPADAPTER_H


#include <property/fastpropertymap.h>
#include <graph/digraph.h>

class KahipAdapter {

public:
    static Algora::FastPropertyMap<unsigned long long> handlePartitioning(unsigned long long int k, Algora::DiGraph *graph);

private:
    static std::map<unsigned long long int, std::map<unsigned long long int, unsigned long long int>> generateVertexMapFromGraph( Algora::DiGraph *graph);
};


#endif //ALLPAIRREACH_KAHIPADAPTER_H
