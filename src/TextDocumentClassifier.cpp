#include "TextDocumentClassifier.h"

int main(int argc, char* argv[]){
	//read file in string
	if (argc <= 1) return -1;

	string content = readInstanceFile(argv[1]);

	BOWLabel positives;
	BOWLabel negatives;

	processRawData(content, positives, negatives);


	cout << positives.wordsCount << endl;

	return 0;
}


void includeWords(tuple<string, int> word, BOWLabel& bow) {
	bool found = false;
	for (int i = 0; i < bow.bow.size(); i++) {
		if (std::get<0>(bow.bow[i]) == std::get<0>(word)) {
			bow.wordsCount += std::get<1>(word);
			std::get<1>(bow.bow[i]) = std::get<1>(word)+std::get<1>(bow.bow[i]);
			found = true;
			break;
		}
	}
	if (!found) {
		bow.bow.push_back(word);
		bow.wordsCount += std::get<1>(word);
	}
}

//read instance file into string
const string readInstanceFile(const char* file){
	ifstream ifs(file);
	string content((istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	return content;
}

void processRawData(const string rawData, BOWLabel& positves, BOWLabel& negatives) {
	vector<string> oneLine = split(rawData, "\n");
	vector<string>::iterator it = oneLine.begin();
	while (it != oneLine.end()) {
		processBOWLabel(*it, positves, negatives);
		it++;
	}
}

void processBOWLabel(const string rawData, BOWLabel& positves, BOWLabel& negatives) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	int label = stoi(*it);
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuple (tup[0],stoi(tup[1]));
		if (label >= 0) {
			includeWords(tuple, positves);
		} else {
			includeWords(tuple, negatives);
		}
		it++;
	}
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
