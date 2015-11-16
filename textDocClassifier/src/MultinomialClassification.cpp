#include "TextDocumentClassifier.h"

/*
* Test Multinomial Methods
*/

const int test(const string rawData, MultinomialLabel** testBOW, const MultinomialLabel** multinomial, const int classes) {
	vector<string> oneLine = split(rawData, "\n");
	*testBOW = new MultinomialLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	int i = 0;
	const int v = countUniqueWords(multinomial, classes);
	while (it != oneLine.end()) {
		processTestMultinomialLabel(*it, *(*(testBOW)+i), multinomial, classes, v);
		i++;
		it++;
	}
	return oneLine.size();
}

void processTestMultinomialLabel(const string rawData, MultinomialLabel& bow, const MultinomialLabel** multinomial, const int classes, const int v){
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	bow.label = stoi(*it) < 0 ? 0 : stoi(*it);
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		if (wordInOriginalData(tup[0], multinomial, classes)) {
			tuple<string,int> tuples (tup[0],stoi(tup[1]));
			bow.bow.push_back(tuples);
		}
		it++;
	}
	trainLabel(bow, multinomial, classes, v);
}

void trainLabel(MultinomialLabel& bow, const MultinomialLabel** multinomial, const int classes, const int v){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	double* percentages = new double[classes];
	for (int i = 0; i < classes; i++) {
		*(percentages+i) = 0.0;
	}
	while (it != bow.bow.end()) {
		for (int i = 0; i < classes; i++) {
			vector<tuple<string, int>>::const_iterator itr = (*multinomial+i)->bow.begin();
			bool found = false;
			while (itr != (*multinomial+i)->bow.end()) {
				if (std::get<0>(*itr) == std::get<0>(*it)) {
					*(percentages+i) += log(((double) (std::get<1>(*itr) + 1)) / ((*multinomial+i)->wordsCount + v));
					found = true;
					break;
				}
				itr++;
			}
			if (!found) {
				*(percentages+i) += log(((double) 1) /  ((*multinomial+i)->wordsCount + v));
			}
		}
		it++;
	}
	int biggestLabel = -1;
	double highestPercentage = strtod("-Inf", NULL);
	for (int i = 0; i < classes; i++) {
		if (highestPercentage < *(percentages+i)) {
			biggestLabel = i;
			highestPercentage = *(percentages+i);
		}
	}
	bow.trainedLabel = biggestLabel;
	//cout << bow.label << " vs. " << bow.trainedLabel << " - Pct. " << *(percentages+0) << " - " << *(percentages+1) << endl;
	delete[] percentages;
}

const int countUniqueWords(const MultinomialLabel** multinomial, const int classes){
	set<string> s;
	for (int i = 0; i < classes; i++) {
		vector<tuple<string, int>>::const_iterator it = (*multinomial+i)->bow.begin();
		while (it != (*multinomial+i)->bow.end()) {
			s.insert(std::get<0>(*it));
			it++;
		}
	}
	return s.size();
}

const bool wordInOriginalData(const string word, const MultinomialLabel** multinomial, const int classes){
	bool found = false;
	for (int i = 0; i < classes; i++) {
		vector<tuple<string, int>>::const_iterator it = (*multinomial+i)->bow.begin();
		while (it != (*multinomial+i)->bow.end()) {
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
