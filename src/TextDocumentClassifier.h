#ifndef TEXTDOCUMENTCLASSIFIER_H_
#define TEXTDOCUMENTCLASSIFIER_H_

#include <iostream>
#include <fstream>
#include <list>
#include <tuple>
#include <string.h>
#include <vector>
using namespace std;

struct BOWLabel
{
	int label;
	list<tuple<string, int>> bow;
};

int main(int, char**);
const string readInstanceFile(const char*);
int processRawData(const string, BOWLabel**);
void processBOWLabel(const string, BOWLabel*);
const vector<string> split(const string, const char*);
#endif