#include "TextDocumentClassifier.h"

const int testBernoulli(const string rawData, BernoulliLabel** testBOW, const BernoulliLabel** bernoulli, const int classes) {
	vector<string> oneLine = split(rawData, "\n");
	*testBOW = new BernoulliLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	const int v = countUniqueWords(bernoulli, classes);
	int i = 0;
	while (it != oneLine.end()) {
		processTestBernoulliLabel(*it, *(*(testBOW)+i), bernoulli, classes, v);
		i++;
		it++;
	}
	return oneLine.size();
}

void processTestBernoulliLabel(const string rawData, BernoulliLabel& bow, const BernoulliLabel** bernoulli, const int classes, const int v) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	bow.label = stoi(*it) < 0 ? 0 : stoi(*it);
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		if (wordInOriginalData(tup[0], bernoulli, classes)) {
			tuple<string,int> tuple (tup[0],stoi(tup[1]));
			bow.bow.push_back(tuple);
		}
		it++;
	}
	trainLabelBernoulli(bow, bernoulli, classes, v);
}

void trainLabelBernoulli(BernoulliLabel& bow, const BernoulliLabel** bernoulli, const int classes, const int v){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	long double* percentages = new long double[classes];
	for (int i = 0; i < classes; i++) {
		*(percentages+i) = 1.0;
	}
	while (it != bow.bow.end()) {
		for (int i = 0; i < classes; i++) {
			vector<tuple<string, int>>::const_iterator itr = (*bernoulli+i)->bow.begin();
			bool found = false;
			while (itr != (*bernoulli+i)->bow.end()) {
				if (std::get<0>(*itr) == std::get<0>(*it)) {
					*(percentages+i) *= ((long double) (std::get<1>(*itr) + 1)) / ((*bernoulli+i)->documentsCount + v);
					found = true;
					break;
				}
				itr++;
			}
			if (!found) {
				*(percentages+i) *= ((long double) 1) / ((*bernoulli+i)->documentsCount + v);
			}
		}
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
	//cout << bow.label << " vs. " << bow.trainedLabel << " - Pct. " << *(percentages+0) << " - " << *(percentages+1) << endl;
}

const int countUniqueWords(const BernoulliLabel** bernoulli, const int classes){
	set<string> s;
	for (int i = 0; i < classes; i++) {
		vector<tuple<string, int>>::const_iterator it = (*bernoulli+i)->bow.begin();
		while (it != (*bernoulli+i)->bow.end()) {
			s.insert(std::get<0>(*it));
			it++;
		}
	}
	return s.size();
}

const bool wordInOriginalData(const string word, const BernoulliLabel** bernoulli, const int classes){
	bool found = false;
	for (int i = 0; i < classes; i++) {
		vector<tuple<string, int>>::const_iterator it = (*bernoulli+i)->bow.begin();
		while (it != (*bernoulli+i)->bow.end()) {
			if (word == std::get<0>(*it)) {
				found = true;
				break;
			}
			it++;
		}
		if (found) break;
	}
	return found;
}
