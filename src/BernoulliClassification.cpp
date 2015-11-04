#include "TextDocumentClassifier.h"

const int testBernoulli(const string rawData, BernoulliLabel** testBOW, const BernoulliLabel** bernoulli, const int classes) {
	vector<string> oneLine = split(rawData, "\n");
	*testBOW = new BernoulliLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	int i = 0;
	while (it != oneLine.end()) {
		processTestBernoulliLabel(*it, *(*(testBOW)+i), bernoulli, classes);
		i++;
		it++;
	}
	return oneLine.size();
}

void processTestBernoulliLabel(const string rawData, BernoulliLabel& bow, const BernoulliLabel** bernoulli, const int classes) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	bow.label = stoi(*it) < 0 ? 0 : stoi(*it);
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuple (tup[0],stoi(tup[1]));
		bow.bow.push_back(tuple);
		it++;
	}
	trainLabelBernoulli(bow, bernoulli, classes);
}

void trainLabelBernoulli(BernoulliLabel& bow, const BernoulliLabel** bernoulli, const int classes){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	long double* percentages = new long double[classes];
	for (int i = 0; i < classes; i++) {
		*(percentages+i) = 1.0;
	}
	int n = 0;
	int v = (*bernoulli)->bow.size();
	while (it != bow.bow.end()) {
		for (int i = 0; i < classes; i++) {
			vector<tuple<string, int>>::const_iterator itr = (*bernoulli+i)->bow.begin();
			while (itr != (*bernoulli+i)->bow.end()) {
				if (std::get<0>(*itr) == std::get<0>(*it)) {
					*(percentages+i) *= ((long double) std::get<1>(*itr) + 1) / (long double) ((*bernoulli+i)->documentsCount + v);
					break;
				}
				itr++;
			}
		}
		n++;
		it++;
	}
	int biggestLabel = -1;
	long double highestPercentage = -1.0;
	for (int i = 0; i < classes; i++) {
		if (highestPercentage < *(percentages+i)) {
			biggestLabel = i;
			highestPercentage = *(percentages+i);
		}
	}
	bow.trainedLabel = biggestLabel;
	delete[] percentages;
	//cout << "Words: " << n << ": " << bow.label << " vs. " << bow.trainedLabel << " - Pct. " << *(percentages+0) << " - " << *(percentages+1) << endl;
}
