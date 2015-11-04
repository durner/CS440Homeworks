#ifndef BERNOULLICALLSIFICATION_H_
#define BERNOULLICALLSIFICATION_H_

#include <tuple>
#include <vector>

using namespace std;

struct BernoulliLabel
{
	int label;
	int trainedLabel;
	int documentsCount;
	vector<tuple<string, int>> bow;
};

const int testBernoulli(const string, BernoulliLabel**, const BernoulliLabel**, const int);
void processTestBernoulliLabel(const string, BernoulliLabel&, const BernoulliLabel**, const int);
void trainLabelBernoulli(BernoulliLabel&, const BernoulliLabel**, const int);

#endif