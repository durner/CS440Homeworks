#include "TextDocumentClassifier.h"

int main(int argc, char* argv[]){
	//read file in string
	if (argc <= 3) return -1;

	string trainData = readInstanceFile(argv[1]);
	string testData = readInstanceFile(argv[2]);
	int classes = stoi(argv[3]);

	// calloc the Labels already (reserve space and initialize with 0)
	MultinomialLabel* mTrainMegaSets = new MultinomialLabel[classes]();
	MultinomialLabel* mTestLabels;

	// calloc the Labels already (reserve space and initialize with 0)
	BernoulliLabel* bTrainMegaSets = new BernoulliLabel[classes]();
	BernoulliLabel* bTestLabels;

	cout << "Training Classifier..." << endl;

	train(trainData, &mTrainMegaSets, &bTrainMegaSets, classes);

	cout << "Classifier Trained" << endl;

	// const correctness enforcement
	const MultinomialLabel* mTrainMegaSetsConst = const_cast<const MultinomialLabel*>(mTrainMegaSets);
	const BernoulliLabel* bTrainMegaSetsConst = const_cast<const BernoulliLabel*>(bTrainMegaSets);

	cout << "==================================" << endl;
	cout << "Classficiation (Multinomial Naive Bayes) of test data in progress..." << endl;
	int testLabelSize = test(testData, &mTestLabels, &mTrainMegaSetsConst, classes);
	cout << "Classifier Tested" << endl;

	int correctCount = 0;
	for(int i = 0; i < testLabelSize; i++) {
		if ((mTestLabels+i)->label == (mTestLabels+i)->trainedLabel) {
			correctCount++;
		}
	}
	cout << "----------------------------------" << endl;
	float accuracy = (float) correctCount / (float) testLabelSize;
	cout << "Multinomial Naive Bayes Accuracy: " << accuracy << endl;

	cout << "==================================" << endl;

	cout << "Classficiation (Bernulli Naive Bayes) of test data in progress..." << endl;
	testLabelSize = testBernoulli(testData, &bTestLabels, &bTrainMegaSetsConst, classes);
	cout << "Classifier Tested" << endl;

	correctCount = 0;
	for(int i = 0; i < testLabelSize; i++) {
		if ((bTestLabels+i)->label == (bTestLabels+i)->trainedLabel) {
			correctCount++;
		}
	}
	cout << "----------------------------------" << endl;
	accuracy = (float) correctCount / (float) testLabelSize;
	cout << "Bernulli Naive Bayes Accuracy: " << accuracy << endl;

	cout << "==================================" << endl;
	cout << "Top 20 for Multinomial Naive Bayes:" << endl;
	for (int i = 0; i < classes; i++) {
		cout << "Class: " << (mTrainMegaSets+i)->label << endl;
		sort(begin((mTrainMegaSets+i)->bow), end((mTrainMegaSets+i)->bow), TupleComparator<1>());
		reverse(begin((mTrainMegaSets+i)->bow), end((mTrainMegaSets+i)->bow));
		for (int j = 0; j < 20; j++) {
			cout << (j+1) << ":\t" << get<0>((mTrainMegaSets+i)->bow[j]) << " = " << get<1>((mTrainMegaSets+i)->bow[j]) << endl;
		}
		cout << "------------------------------" << endl;
	}
	cout << "==================================" << endl;
	cout << "Top 20 for Bernulli Naive Bayes:" << endl;
	for (int i = 0; i < classes; i++) {
		cout << "Class: " << (mTrainMegaSets+i)->label << endl;
		sort(begin((bTrainMegaSets+i)->bow), end((bTrainMegaSets+i)->bow), TupleComparator<1>());
		reverse(begin((bTrainMegaSets+i)->bow), end((bTrainMegaSets+i)->bow));
		for (int j = 0; j < 20; j++) {
			cout << (j+1) << ":\t" << get<0>((bTrainMegaSets+i)->bow[j]) << " = " << get<1>((bTrainMegaSets+i)->bow[j]) << endl;
		}
		cout << "------------------------------" << endl;
	}

	delete[] mTestLabels;
	delete[] bTestLabels;
	delete[] mTrainMegaSets;
	delete[] bTrainMegaSets;

	return 0;
}

//read instance file into string
const string readInstanceFile(const char* file){
	ifstream ifs(file);
	string content((istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	return content;
}