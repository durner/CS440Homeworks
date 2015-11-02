#include "TextDocumentClassifier.h"

int main(int argc, char* argv[]){
	//read file in string
	if (argc <= 1) return -1;

	string content = readInstanceFile(argv[1]);
	//cout << content << endl;

	BOWLabel* bow = NULL;
	int size = processRawData(content, &bow);

	cout << bow->label << endl;

	return 0;
}

//read instance file into string
const string readInstanceFile(const char* file){
	ifstream ifs(file);
	string content((istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	return content;
}

int processRawData(const string rawData, BOWLabel** bow) {
	vector<string> oneLine = split(rawData, "\n");
	*bow = new BOWLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	int i = 0;
	while (it != oneLine.end()) {
		processBOWLabel(*it, *(bow)+i);
		it++;
		i++;
	}
	return oneLine.size();
}

void processBOWLabel(const string rawData, BOWLabel* bow) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	bow->label = stoi(*it);
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuple (tup[0],stoi(tup[1]));
		bow->bow.push_back(tuple);
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
