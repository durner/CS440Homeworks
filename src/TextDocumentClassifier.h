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
	int wordsCount;
	vector<tuple<string, int>> bow;
};

int main(int, char**);
const string readInstanceFile(const char*);
void processRawData(const string, BOWLabel&, BOWLabel&);
void processBOWLabel(const string, BOWLabel&, BOWLabel&);
void includeWords(vector<tuple<string, int>>, BOWLabel&);
const vector<string> split(const string, const char*);
#endif