#include "TextDocumentClassifier.h"

int main(int argc, char* argv[]){
	//read file in string
	if (argc <= 2) return -1;

	string trainData = readInstanceFile(argv[1]);
	string testData = readInstanceFile(argv[2]);

	BOWLabel trainMegaPositives;
	BOWLabel trainMegaNegatives;
	BOWLabel* testLabels;

	cout << "Training Classifier..." << endl;

	train(trainData, trainMegaPositives, trainMegaNegatives);

	cout << "Classifier Trained" << endl;

	cout << "Classficiation (Multinomial Naive Bayes) of test data in progress..." << endl;
	int testLabelSize = test(testData, &testLabels, trainMegaPositives, trainMegaNegatives, false);
	cout << "Classifier Tested" << endl;

	int correctCount = 0;
	for(int i = 0; i < testLabelSize; i++) {
		//cout << (testLabels+i)->trainedLabel << endl;
		if ((testLabels+i)->label == (testLabels+i)->trainedLabel) {
			correctCount++;
		}
	}

	float accuracy = (float) correctCount / (float) testLabelSize;
	cout << "Multinomial Naive Bayes Accuracy: " << accuracy << endl;


	cout << "Classficiation (Bernulli Naive Bayes) of test data in progress..." << endl;
	testLabelSize = test(testData, &testLabels, trainMegaPositives, trainMegaNegatives, true);
	cout << "Classifier Tested" << endl;

	correctCount = 0;
	for(int i = 0; i < testLabelSize; i++) {
		//cout << (testLabels+i)->trainedLabel << endl;
		if ((testLabels+i)->label == (testLabels+i)->trainedLabel) {
			correctCount++;
		}
	}
	accuracy = (float) correctCount / (float) testLabelSize;
	cout << "Bernulli Naive Bayes Accuracy: " << accuracy << endl;

	return 0;
}


void includeWords(tuple<string, int> word, BOWLabel& bow) {
	bool found = false;
	for (int i = 0; i < bow.bow.size(); i++) {
		if (std::get<0>(bow.bow[i]) == std::get<0>(word)) {
			if (std::get<1>(bow.bow[i]) == 0 && std::get<1>(word) > 0) bow.bernulliCount++;
			bow.wordsCount += std::get<1>(word);
			std::get<1>(bow.bow[i]) = std::get<1>(word)+std::get<1>(bow.bow[i]);
			found = true;
			break;
		}
	}
	if (!found) {
		bow.bow.push_back(word);
		bow.wordsCount += std::get<1>(word);
		if (std::get<1>(word) > 0) bow.bernulliCount++;
	}
}

//read instance file into string
const string readInstanceFile(const char* file){
	ifstream ifs(file);
	string content((istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	return content;
}

const int test(const string rawData, BOWLabel** testBOW, const BOWLabel& megaPositves, const BOWLabel& megaNegatives, const bool bernulli) {
	vector<string> oneLine = split(rawData, "\n");
	*testBOW = new BOWLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	int i = 0;
	while (it != oneLine.end()) {
		processBOWTestLabel(*it, *(*(testBOW)+i), megaPositves, megaNegatives, bernulli);
		i++;
		it++;
	}
	return oneLine.size();
}

void train(const string rawData, BOWLabel& positves, BOWLabel& negatives) {
	vector<string> oneLine = split(rawData, "\n");
	vector<string>::iterator it = oneLine.begin();
	while (it != oneLine.end()) {
		processBOWMegaLabel(*it, positves, negatives);
		it++;
	}
}

void processBOWTestLabel(const string rawData, BOWLabel& bow, const BOWLabel& megaPositves, const BOWLabel& megaNegatives, const bool bernulli) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	bow.label = stoi(*it) > 0 ? 1 : 0;
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuple (tup[0],stoi(tup[1]));
		bow.bow.push_back(tuple);
		it++;
	}
	if (!bernulli) trainLabel(bow, megaPositves, megaNegatives);
	else trainLabelBernulli(bow, megaPositves, megaNegatives);
}

void trainLabel(BOWLabel& bow, const BOWLabel& megaPositves, const BOWLabel& megaNegatives){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	long double percentagePositve = 1.0;
	long double percentageNegative = 1.0;
	int n = 0;
	int v = megaPositves.bow.size();
	while (it != bow.bow.end()) {
		vector<tuple<string, int>>::const_iterator itr = megaPositves.bow.begin();
		while (itr != megaPositves.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				percentagePositve *= ((double) std::get<1>(*itr) + 1) / (double) (megaPositves.wordsCount + v);
				break;
			}
			itr++;
		}
		itr = megaNegatives.bow.begin();
		while (itr != megaNegatives.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				percentageNegative *= (double)(std::get<1>(*itr) + 1) / (double) (megaNegatives.wordsCount + v);
				break;
			}
			itr++;
		}
		n++;
		it++;
	}
	percentagePositve > percentageNegative ? bow.trainedLabel = megaPositves.label : bow.trainedLabel = megaNegatives.label;
	//cout << "Words: " << n << ": " << bow.label << " vs. " << bow.trainedLabel << " - Pct. " << percentagePositve << " - " << percentageNegative << endl;
}

void trainLabelBernulli(BOWLabel& bow, const BOWLabel& megaPositves, const BOWLabel& megaNegatives){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	long double percentagePositve = 1.0;
	long double percentageNegative = 1.0;
	int n = 0;
	int v = megaPositves.bow.size();
	while (it != bow.bow.end()) {
		vector<tuple<string, int>>::const_iterator itr = megaPositves.bow.begin();
		while (itr != megaPositves.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				int update = std::get<1>(*itr) > 0 ? 1 : 0;
				percentagePositve *= ((double) update + 1) / (double) (megaPositves.bernulliCount + v);
				break;
			}
			itr++;
		}
		
		itr = megaNegatives.bow.begin();
		while (itr != megaNegatives.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				int update = std::get<1>(*itr) > 0 ? 1 : 0;
				percentageNegative *= (double)(update + 1) / (double) (megaNegatives.bernulliCount + v);
				break;
			}
			itr++;
		}
		n++;
		it++;
	}
	percentagePositve > percentageNegative ? bow.trainedLabel = megaPositves.label : bow.trainedLabel = megaNegatives.label;
	cout << "Words: " << n << ": " << bow.label << " vs. " << bow.trainedLabel << " - Pct. " << percentagePositve << " - " << percentageNegative << endl;
}


void processBOWMegaLabel(const string rawData, BOWLabel& positves, BOWLabel& negatives) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	int label = stoi(*it);
	it++;
	positves.label = 1;
	negatives.label = 0;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuples (tup[0],stoi(tup[1]));
		tuple<string,int> emptyTuple (tup[0],0);
		if (label > 0) {
			includeWords(tuples, positves);
			includeWords(emptyTuple, negatives);
		} else {
			includeWords(tuples, negatives);
			includeWords(emptyTuple, positves);
		}
		it++;
	}
}

const int countUniqueWords(const BOWLabel& pos, const BOWLabel& neg){
	int i = 0;
	vector<tuple<string, int>>::const_iterator it = pos.bow.begin();
	while (it != pos.bow.end()) {
		vector<tuple<string, int>>::const_iterator itr = neg.bow.begin();
		bool found = false;
		while (itr != neg.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				found = true;
				break;
			}
			itr++;
		}
		if (!found) i++;
		it++;
	}

	it = neg.bow.begin();
	while (it != neg.bow.end()) {
		i++;
		it++;
	}
	return i;
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