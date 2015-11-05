#ifndef TEXTDOCUMENTCLASSIFIER_H_
#define TEXTDOCUMENTCLASSIFIER_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "ClassificationTraining.h"

template<int M, template<typename> class F = std::less>
struct TupleComparator
{
    template<typename T>
    bool operator()(T const &t1, T const &t2)
    {
        return F<typename tuple_element<M, T>::type>()(std::get<M>(t1), std::get<M>(t2));
    }
};

int main(int, char**);
const string readInstanceFile(const char*);
#endif