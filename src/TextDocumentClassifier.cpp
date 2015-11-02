#include "TextDocumentClassifier.h"

int main(int argc, char* argv[]){
	//read file in string
	if (argc <= 1) return -1;
	
	string content = readInstanceFile(argv[1]);
	cout << content << endl;

	return 0;
}

//read instance file into string
string readInstanceFile(char* file){
	ifstream ifs(file);
	string content((istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	return content;
}