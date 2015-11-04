#ifndef CLASSIFICATIONTRAINING_H_
#define CLASSIFICATIONTRAINING_H_

#include <string.h>
#include "MultinomialClassification.h"
#include "BernoulliClassification.h"

void train(const string, MultinomialLabel**, BernoulliLabel**, const int);
void processMegaLabel(const string, MultinomialLabel**, BernoulliLabel**, const int);
void includeWords(tuple<string, int>, MultinomialLabel&, BernoulliLabel&);
const vector<string> split(const string, const char*);

#endif