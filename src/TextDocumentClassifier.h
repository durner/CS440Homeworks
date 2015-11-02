#ifndef TEXTDOCUMENTCLASSIFIER_H_
#define TEXTDOCUMENTCLASSIFIER_H_

#include <iostream>
#include <fstream>
#include <tuple>
#include <string.h>
#include <vector>
using namespace std;

struct BOWLabel
{
	int label;
	int trainedLabel;
	int wordsCount;
	int bernulliCount;
	vector<tuple<string, int>> bow;
};

int main(int, char**);
const string readInstanceFile(const char*);
void train(const string, BOWLabel&, BOWLabel&);
const int test(const string, BOWLabel**, const BOWLabel&, const BOWLabel&, const bool);
void processBOWTestLabel(const string, BOWLabel&, const BOWLabel&, const BOWLabel&, const bool);
void processBOWMegaLabel(const string, BOWLabel&, BOWLabel&);
void includeWords(vector<tuple<string, int>>, BOWLabel&);
const vector<string> split(const string, const char*);
void trainLabel(BOWLabel&, const BOWLabel&, const BOWLabel&);
void trainLabelBernulli(BOWLabel&, const BOWLabel&, const BOWLabel&);
const int countUniqueWords(const BOWLabel&, const BOWLabel&);
#endif