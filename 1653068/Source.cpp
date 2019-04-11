#include "Tree.h"

void main()
{
	vector<Atribute *>Train;
	Atribute* Target;
	vector <int> TrainningSet;
	//Read file train.txt
	ReadTrain(Train, Target, "train.txt", TrainningSet);

	//Read file test.txt
	vector<Atribute *>Pre;
	Atribute* TargetPre;
	ReadTest(Pre, TargetPre, "test.txt");

	//ID3 : Function ID3 return to a Node Tree
	Node *root = new Node;
	root = ID3(TrainningSet, Train, Target);

	//Prediction
	Prediction(root, Pre, TargetPre);

	//Write Tree to tree.txt
	WriteTree(root, "tree.txt");
	
	//Write Prediction to prediction.txt
	WritePrediction("prediction.txt", TargetPre);

	system("pause");
}