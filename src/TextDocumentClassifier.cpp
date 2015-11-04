#include "TextDocumentClassifier.h"

/*
* Train Methods
*/

void train(const string rawData, MultinomialLabel& mPositves, MultinomialLabel& mNegatives, BernoulliLabel& bPositves, BernoulliLabel& bNegatives) {
	vector<string> oneLine = split(rawData, "\n");
	vector<string>::iterator it = oneLine.begin();
	while (it != oneLine.end()) {
		processMegaLabel(*it, mPositves, mNegatives, bPositves, bNegatives);
		it++;
	}
}

void processMegaLabel(const string rawData, MultinomialLabel& mPositves, MultinomialLabel& mNegatives, BernoulliLabel& bPositves, BernoulliLabel& bNegatives) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	int label = stoi(*it);
	it++;
	mPositves.label = 1;
	bPositves.label = 1;
	mNegatives.label = 0;
	bNegatives.label = 0;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuples (tup[0],stoi(tup[1]));
		tuple<string,int> emptyTuple (tup[0],0);
		if (label > 0) {
			bPositves.documentsCount++;
			includeWords(tuples, mPositves, bPositves);
			includeWords(emptyTuple, mNegatives, bNegatives);
		} else {
			bNegatives.documentsCount++;
			includeWords(tuples, mNegatives, bNegatives);
			includeWords(emptyTuple, mPositves, bPositves);
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

/*
* Test Multinomial Methods
*/

const int test(const string rawData, MultinomialLabel** testBOW, const MultinomialLabel& megaPositves, const MultinomialLabel& megaNegatives) {
	vector<string> oneLine = split(rawData, "\n");
	*testBOW = new MultinomialLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	int i = 0;
	while (it != oneLine.end()) {
		processTestMultinomialLabel(*it, *(*(testBOW)+i), megaPositves, megaNegatives);
		i++;
		it++;
	}
	return oneLine.size();
}

void processTestMultinomialLabel(const string rawData, MultinomialLabel& bow, const MultinomialLabel& megaPositves, const MultinomialLabel& megaNegatives) {
	vector<string> oneLine = split(rawData, " ");
	vector<string>::iterator it = oneLine.begin();
	bow.label = stoi(*it) > 0 ? 1 : 0;
	it++;
	while (it != oneLine.end()) {
		vector<string> tup = split(*it, ":");
		tuple<string,int> tuples (tup[0],stoi(tup[1]));
		bow.bow.push_back(tuples);
		it++;
	}
	trainLabel(bow, megaPositves, megaNegatives);
}

void trainLabel(MultinomialLabel& bow, const MultinomialLabel& megaPositves, const MultinomialLabel& megaNegatives){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	long double percentagePositve = 1.0;
	long double percentageNegative = 1.0;
	int n = 0;
	int v = megaPositves.bow.size();
	while (it != bow.bow.end()) {
		vector<tuple<string, int>>::const_iterator itr = megaPositves.bow.begin();
		while (itr != megaPositves.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				percentagePositve *= ((long double) std::get<1>(*itr) + 1) / (long double) (megaPositves.wordsCount + v);
				break;
			}
			itr++;
		}
		itr = megaNegatives.bow.begin();
		while (itr != megaNegatives.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				percentageNegative *= ((long double)std::get<1>(*itr) + 1) / (long double) (megaNegatives.wordsCount + v);
				break;
			}
			itr++;
		}
		n++;
		it++;
	}
	percentagePositve > percentageNegative ? bow.trainedLabel = megaPositves.label : bow.trainedLabel = megaNegatives.label;
	//cout << megaPositves.label <<"Words: " << n << ": " << bow.label << " vs. " << bow.trainedLabel << " - Pct. " << percentagePositve << " - " << percentageNegative << endl;
}

/*
* Test Bernoulli Methods
*/


const int testBernoulli(const string rawData, BernoulliLabel** testBOW, const BernoulliLabel& megaPositves, const BernoulliLabel& megaNegatives) {
	vector<string> oneLine = split(rawData, "\n");
	*testBOW = new BernoulliLabel[oneLine.size()];
	vector<string>::iterator it = oneLine.begin();
	int i = 0;
	while (it != oneLine.end()) {
		processTestBernoulliLabel(*it, *(*(testBOW)+i), megaPositves, megaNegatives);
		i++;
		it++;
	}
	return oneLine.size();
}

void processTestBernoulliLabel(const string rawData, BernoulliLabel& bow, const BernoulliLabel& megaPositves, const BernoulliLabel& megaNegatives) {
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
	trainLabelBernoulli(bow, megaPositves, megaNegatives);
}

void trainLabelBernoulli(BernoulliLabel& bow, const BernoulliLabel& megaPositves, const BernoulliLabel& megaNegatives){
	vector<tuple<string, int>>::iterator it = bow.bow.begin();
	long double percentagePositve = 1.0;
	long double percentageNegative = 1.0;
	int n = 0;
	int v = megaPositves.bow.size();
	while (it != bow.bow.end()) {
		vector<tuple<string, int>>::const_iterator itr = megaPositves.bow.begin();
		while (itr != megaPositves.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				percentagePositve *= (long double) ((long double) std::get<1>(*itr) + 1) / (long double) (megaPositves.documentsCount + v);
				break;
			}
			itr++;
		}

		itr = megaNegatives.bow.begin();
		while (itr != megaNegatives.bow.end()) {
			if (std::get<0>(*itr) == std::get<0>(*it)) {
				percentageNegative *= (long double) ((long double) std::get<1>(*itr) + 1) / ((long double) (megaPositves.documentsCount + v));
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



/*
* Helper Methods
*/

int main(int argc, char* argv[]){
	//read file in string
	if (argc <= 2) return -1;

	string trainData = readInstanceFile(argv[1]);
	string testData = readInstanceFile(argv[2]);

	MultinomialLabel mTrainMegaPositives{};
	MultinomialLabel mTrainMegaNegatives{};
	MultinomialLabel* mTestLabels;

	BernoulliLabel bTrainMegaPositives{};
	BernoulliLabel bTrainMegaNegatives{};
	BernoulliLabel* bTestLabels;

	cout << "Training Classifier..." << endl;

	train(trainData, mTrainMegaPositives, mTrainMegaNegatives, bTrainMegaPositives, bTrainMegaNegatives);

	cout << "Classifier Trained" << endl;

	cout << "Classficiation (Multinomial Naive Bayes) of test data in progress..." << endl;
	int testLabelSize = test(testData, &mTestLabels, mTrainMegaPositives, mTrainMegaNegatives);
	cout << "Classifier Tested" << endl;

	int correctCount = 0;
	for(int i = 0; i < testLabelSize; i++) {
		if ((mTestLabels+i)->label == (mTestLabels+i)->trainedLabel) {
			correctCount++;
		}
	}

	float accuracy = (float) correctCount / (float) testLabelSize;
	cout << "Multinomial Naive Bayes Accuracy: " << accuracy << endl;


	cout << "Classficiation (Bernulli Naive Bayes) of test data in progress..." << endl;
	testLabelSize = testBernoulli(testData, &bTestLabels, bTrainMegaPositives, bTrainMegaNegatives);
	cout << "Classifier Tested" << endl;

	correctCount = 0;
	for(int i = 0; i < testLabelSize; i++) {
		if ((bTestLabels+i)->label == (bTestLabels+i)->trainedLabel) {
			correctCount++;
		}
	}
	accuracy = (float) correctCount / (float) testLabelSize;
	cout << "Bernulli Naive Bayes Accuracy: " << accuracy << endl;

	return 0;
}


//read instance file into string
const string readInstanceFile(const char* file){
	ifstream ifs(file);
	string content((istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	return content;
}

const int countUniqueWords(const MultinomialLabel& pos, const MultinomialLabel& neg){
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