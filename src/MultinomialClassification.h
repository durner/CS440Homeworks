#ifndef MULTINOMIALCALLSIFICATION_H_
#define MULTINOMIALCALLSIFICATION_H_

#include <tuple>
#include <vector>

using namespace std;

struct MultinomialLabel
{
	int label;
	int trainedLabel;
	int wordsCount;
	vector<tuple<string, int>> bow;
};

const int test(const string, MultinomialLabel**, const MultinomialLabel**, const int);
void processTestMultinomialLabel(const string, MultinomialLabel&, const MultinomialLabel**, const int);
void trainLabel(MultinomialLabel&, const MultinomialLabel**, const int);
#endif