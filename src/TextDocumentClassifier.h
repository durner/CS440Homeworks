#ifndef TEXTDOCUMENTCLASSIFIER_H_
#define TEXTDOCUMENTCLASSIFIER_H_

#include <iostream>
#include <fstream>
#include <tuple>
#include <string.h>
#include <vector>
using namespace std;

struct MultinomialLabel
{
	int label;
	int trainedLabel;
	int wordsCount;
	vector<tuple<string, int>> bow;
};

struct BernoulliLabel
{
	int label;
	int trainedLabel;
	int documentsCount;
	vector<tuple<string, int>> bow;
};


void train(const string, MultinomialLabel&, MultinomialLabel&, BernoulliLabel&, BernoulliLabel&);
void processMegaLabel(const string, MultinomialLabel&, MultinomialLabel&, BernoulliLabel&, BernoulliLabel&);
void includeWords(tuple<string, int>, MultinomialLabel&, BernoulliLabel&);

const int test(const string, MultinomialLabel**, const MultinomialLabel&, const MultinomialLabel&);
void processTestMultinomialLabel(const string, MultinomialLabel&, const MultinomialLabel&, const MultinomialLabel&);
void trainLabel(MultinomialLabel&, const MultinomialLabel&, const MultinomialLabel&);

const int testBernoulli(const string, BernoulliLabel**, const BernoulliLabel&, const BernoulliLabel&);
void processTestBernoulliLabel(const string, BernoulliLabel&, const BernoulliLabel&, const BernoulliLabel&);
void trainLabelBernoulli(BernoulliLabel&, const BernoulliLabel&, const BernoulliLabel&);

int main(int, char**);
const string readInstanceFile(const char*);
const vector<string> split(const string, const char*);
const int countUniqueWords(const MultinomialLabel&, const MultinomialLabel&);
#endif