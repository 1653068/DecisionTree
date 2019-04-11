#ifndef _TREE_H_
#define _TRE_H_
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

static int SizeExample = 0;
static int SizeAtribute = 0;
static int SizePre = 0;

struct Node		//Node of Tree
{
	string Name;
	vector <Node*> BrachNode;
	vector <string> BranchName;
	bool Mode;
};

struct Example  //All Example 
{
	string Name;
	bool Flag;
};

struct Class    //Classify different values in a Atribute
{
	string Name;
	float Entropy;
	int Size, Yes, No;
	bool Flag;
};

struct Atribute	
{
	string Name;
	vector <Class*> Classify ;
	vector <Example*> Ex;
	float Entropy;
	bool Flag;
};


void ReadTrain(vector<Atribute*> &A, Atribute *&Target, string Path, vector <int> &TrainningSet);
void ReadTest(vector<Atribute*> &A, Atribute *&Target, string Path);
void WriteTree(Node *root, string Path);
void WritePrediction(string Path, Atribute *Target);
string MostPupular(Atribute *Target, vector <int> TrainningSet);
void SaparateAtribute(vector<Atribute*> &A, Atribute *&Target, char *str);
void SetExampleName(Atribute *&A, string name);
void CountYesNo(Atribute *&Ai, Atribute *Target, vector <int> TrainningSet);
void CalAE(Atribute *&Ai, Atribute *Target, vector <int> TrainningSet);
void CalAG(vector<Atribute*> &A, Atribute *&Target, vector <int> TrainningSet);
int MinAE(vector<Atribute*> A);
bool AllYes(vector <int> TrainningSet, Atribute *Target);
bool AllNo(vector <int> TrainningSet, Atribute *Target);
bool AtributeEmpty(vector<Atribute*> A);
vector<int> SubTranningSet(vector <int> TrainningSet, Atribute *A, string Classify);
Node *ID3(vector <int> TrainningSet, vector<Atribute*> &A, Atribute *&Target);

void PrintTree(Node *root, int depth, ofstream &file);
string BrowseTree(Node *root, vector<Atribute*> Pre, int index);
void Prediction(Node *root, vector<Atribute*> Pre, Atribute *&Target);
#endif // !_TREE_H_
