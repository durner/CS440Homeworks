#include "ClassificationTraining.h"

void train(const string rawData, MultinomialLabel** multinomial, BernoulliLabel** bernoulli, const int classes) {
	vector<string> oneLine = split(rawData, "\n");
	vector<string>::iterator it = oneLine.begin();
	while (it != oneLine.end()) {
		processMegaLabel(*it, multinomial, bernoulli, classes);
		it++;
	}
}

void processMegaLabel(const string rawData, MultinomialLabel** multinomial, BernoulliLabel** bernoulli, const int classes) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	int label = stoi(*it) < 0 ? 0 : stoi(*it);
	it++;
	for (int i = 0; i < classes; i++) {
		(*multinomial+i)->label = i;
		(*bernoulli+i)->label = i;
	}
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuples (tup[0],stoi(tup[1]));
		tuple<string,int> emptyTuple (tup[0],0);
		for (int i = 0; i < classes; i++) {
			if (i == label) {
				(*bernoulli+i)->documentsCount++;
				includeWords(tuples, *(*multinomial+i), *(*bernoulli+i));
			}
			else includeWords(emptyTuple, *(*multinomial+i), *(*bernoulli+i));
		}
		it++;
	}
}

void includeWords(tuple<string, int> word, MultinomialLabel& multinomial, BernoulliLabel& bernoulli) {
	bool foundMultinomial = false;
	bool foundBernoulli = false;
	for (int i = 0; i < multinomial.bow.size(); i++) {
		if (std::get<0>(multinomial.bow[i]) == std::get<0>(word)) {
			multinomial.wordsCount += std::get<1>(word);
			std::get<1>(multinomial.bow[i]) += std::get<1>(word);
			foundMultinomial = true;
			break;
		}
	}
	for (int i = 0; i < bernoulli.bow.size(); i++) {
		if (std::get<0>(bernoulli.bow[i]) == std::get<0>(word)) {
			int update = std::get<1>(word) > 0 ? 1 : 0;
			std::get<1>(bernoulli.bow[i]) += update;
			foundBernoulli = true;
			break;
		}
	}
	if (!foundMultinomial) {
		multinomial.bow.push_back(word);
		multinomial.wordsCount += std::get<1>(word);
	}
	if (!foundBernoulli) bernoulli.bow.push_back(word);
}

const vector<string> split(const string s, const char* delim) {
    vector<string> elems;
    char * writable = new char[s.size() + 1];
	std::copy(s.begin(), s.end(), writable);
	writable[s.size()] = '\0';
	char* pch = strtok (writable,delim);
	while (pch != NULL)
	{
		elems.push_back(pch);
		pch = strtok (NULL, delim);
	}
    return elems;
}