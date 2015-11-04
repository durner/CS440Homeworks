#include "TextDocumentClassifier.h"

/*
* Test Multinomial Methods
*/

const int test(const string rawData, MultinomialLabel** testBOW, const MultinomialLabel** multinomial, const int classes) {
	vector<string> oneLine = split(rawData, "\n");
	*testBOW = new MultinomialLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	int i = 0;
	while (it != oneLine.end()) {
		processTestMultinomialLabel(*it, *(*(testBOW)+i), multinomial, classes);
		i++;
		it++;
	}
	return oneLine.size();
}

void processTestMultinomialLabel(const string rawData, MultinomialLabel& bow, const MultinomialLabel** multinomial, const int classes) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	bow.label = stoi(*it) < 0 ? 0 : stoi(*it);
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuples (tup[0],stoi(tup[1]));
		bow.bow.push_back(tuples);
		it++;
	}
	trainLabel(bow, multinomial, classes);
}

void trainLabel(MultinomialLabel& bow, const MultinomialLabel** multinomial, const int classes){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	long double* percentages = new long double[classes];
	for (int i = 0; i < classes; i++) {
		*(percentages+i) = 1.0;
	}
	int n = 0;
	int v = (*multinomial)->bow.size();
	while (it != bow.bow.end()) {
		for (int i = 0; i < classes; i++) {
			vector<tuple<string, int>>::const_iterator itr = (*multinomial+i)->bow.begin();
			while (itr != (*multinomial+i)->bow.end()) {
				if (std::get<0>(*itr) == std::get<0>(*it)) {
					*(percentages+i) *= ((long double) std::get<1>(*itr) + 1) / (long double) ((*multinomial+i)->wordsCount + v);
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
