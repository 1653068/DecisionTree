#include "Tree.h"


void ReadTrain(vector<Atribute*> &A, Atribute *&Target, string Path, vector <int> &TrainningSet)
{
	ifstream file;
	file.open(Path, ios::in);
	char *tmpA = new char[255];
	file.getline(tmpA, 255);
	SaparateAtribute(A, Target, tmpA);
	while (!file.eof())
	{
		for (int i = 0; i < A.size(); i++)
		{
			string tmp;
			file >> tmp;
			SetExampleName(A[i], tmp);
		}
		string tmp;
		file >> tmp;
		SetExampleName(Target, tmp);
	}
	SizeExample = Target->Ex.size();
	SizeAtribute = A.size();
	for (int i = 0; i < SizeExample; i++)
		TrainningSet.push_back(i);
}

void ReadTest(vector<Atribute*> &A, Atribute *&Target, string Path)
{
	fstream file;
	file.open(Path, ios::in);
	string tmp;
	for (int i = 0; i < SizeAtribute; i++)
	{
		file >> tmp;
		Atribute *Atr = new Atribute;
		Atr->Name = tmp;
		A.push_back(Atr);
	}
	
	file >> tmp;
	Target = new Atribute;
	Target->Name = tmp;
	while (!file.eof())
	{
		for (int i = 0; i < SizeAtribute; i++)
		{
			file >> tmp;
			Example *Ex = new Example;
			Ex->Name = tmp;
			A[i]->Ex.push_back(Ex);
		}
		file >> tmp;
		Example *Ex = new Example;
		Ex->Name = tmp;
		Target->Ex.push_back(Ex);
		SizePre++;
	}
	file.close();
}

void WriteTree(Node *root, string Path)
{
	ofstream file;
	file.open(Path, ios::out);
	for (int i = 0; i < root->BrachNode.size(); i++)
	{
		file << root->Name << " = " << root->BranchName[i];
		PrintTree(root->BrachNode[i], 1, file);
	}
	file.close();
}

void WritePrediction(string Path, Atribute *Target)
{
	fstream file;
	file.open(Path, ios::out);
	for (int i = 0; i < SizePre; i++)
		file << Target->Ex[i]->Name << endl;
	file.close();
}

string MostPupular(Atribute *Target, vector <int> TrainningSet)
{
	int Yes = 0, No = 0;
	for (int i = 0; i < TrainningSet.size(); i++)
		if (Target->Ex[TrainningSet[i]]->Name == Target->Classify[0]->Name)
			Yes++;
		else
			No++;

	return (Yes >= No) ? Target->Classify[0]->Name : Target->Classify[1]->Name;
}

void SaparateAtribute(vector<Atribute*> &A, Atribute *&Target, char *str)
{
	int len = strlen(str) + 1, i = 0, begin = 0, end = 0;

	while (i < len)
	{
		if (str[i] == ' ')
		{
			end = i;
			char tmp[50];
			for (int k = 0; k < end - begin; k++)
			{
				tmp[k] = str[begin + k];
			}
			tmp[end - begin] = '\0';
			Atribute *atr = new Atribute;
			atr->Name = tmp;
			atr->Entropy = 0;
			atr->Flag = true;
			A.push_back(atr);
			begin = i + 1;
		}
		i++;
	}
	char tmp[50];
	for (int k = 0; k < i - begin; k++)
	{
		tmp[k] = str[begin + k];
	}
	tmp[i - begin] = '\0';
	Atribute *atrtmp = new Atribute;
	atrtmp->Name = tmp;
	Target = atrtmp;
}

void SetExampleName(Atribute *&A, string name)
{
	int i;
	for (i = 0; i < A->Classify.size(); i++)
	{
		if (A->Classify[i]->Name == name)
		{
			A->Classify[i]->Size++;
			break;
		}
	}
	if (i == A->Classify.size())
	{
		Class *tmp = new Class;
		tmp->Name = name;
		tmp->Entropy = 0;
		tmp->Size = 0;
		tmp->Flag = true;
		A->Classify.push_back(tmp);
	}
	Example *tmpEx = new Example;
	tmpEx->Flag = true;
	tmpEx->Name = name;
	A->Ex.push_back(tmpEx);
}

void CountYesNo(Atribute *&Ai, Atribute *Target, vector <int> TrainningSet)
{	
	for (int i = 0; i < Ai->Classify.size(); i++)
	{
		if (Ai->Classify[i]->Flag)
		{
			Ai->Classify[i]->Yes = 0;
			Ai->Classify[i]->No = 0;
			Ai->Classify[i]->Size = 0;
			for (int j = 0; j < TrainningSet.size(); j++)
			{
				if (Ai->Classify[i]->Name == Ai->Ex[TrainningSet[j]]->Name &&
					Target->Ex[TrainningSet[j]]->Name == Target->Classify[0]->Name)
					Ai->Classify[i]->Yes++;
				if (Ai->Classify[i]->Name == Ai->Ex[TrainningSet[j]]->Name &&
					Target->Ex[TrainningSet[j]]->Name == Target->Classify[1]->Name)
					Ai->Classify[i]->No++;
				if (Ai->Classify[i]->Name == Ai->Ex[TrainningSet[j]]->Name)
					Ai->Classify[i]->Size++;
			}
		}
	}
}

void CalAE(Atribute *&Ai, Atribute *Target, vector <int> TrainningSet)
{
	CountYesNo(Ai, Target, TrainningSet);

	Ai->Entropy = 0;
	for (int i = 0; i < Ai->Classify.size(); i++)
	{
		if (Ai->Classify[i]->Flag)
		{
			if (Ai->Classify[i]->Yes == Ai->Classify[i]->No)
				Ai->Classify[i]->Entropy = 1;
			else if (Ai->Classify[i]->Yes == 0 || Ai->Classify[i]->No == 0)
				Ai->Classify[i]->Entropy = 0;
			else
				Ai->Classify[i]->Entropy = -((float)Ai->Classify[i]->Yes / Ai->Classify[i]->Size) * log2f((float)Ai->Classify[i]->Yes / Ai->Classify[i]->Size)
				- ((float)Ai->Classify[i]->No / Ai->Classify[i]->Size) * log2f((float)Ai->Classify[i]->No / Ai->Classify[i]->Size);
		}
		Ai->Entropy += ((float) Ai->Classify[i]->Size/TrainningSet.size())*Ai->Classify[i]->Entropy;
	}
}

void CalAG(vector<Atribute*> &A, Atribute *&Target, vector <int> TrainningSet)
{
	for (int i = 0; i < A.size(); i++)
	{
		if (A[i]->Flag)
			CalAE(A[i], Target, TrainningSet);
	}
}

int MinAE(vector<Atribute*> A)
{
	int index = -1;
	float Min = 2;
	for(int i=0;i<A.size();i++)
		if (A[i]->Flag && A[i]->Entropy < Min)
		{
			Min = A[i]->Entropy;
			index = i;
		}
	return index;
}

bool AllYes(vector <int> TrainningSet, Atribute *Target)
{
	for (int i = 0; i < TrainningSet.size(); i++)
		if (Target->Ex[TrainningSet[i]]->Name != Target->Classify[0]->Name)
			return false;
	return true;
}

bool AllNo(vector <int> TrainningSet, Atribute *Target)
{
	for (int i = 0; i < TrainningSet.size(); i++)
		if (Target->Ex[TrainningSet[i]]->Name != Target->Classify[1]->Name)
			return false;
	return true;
}

bool AtributeEmpty(vector<Atribute*> A)
{
	for (int i = 0; i < A.size(); i++)
		if (A[i]->Flag)
			return false;
	return true;
}

vector<int> SubTranningSet(vector <int> TrainningSet, Atribute *A, string Classify)
{
	vector<int> tmp;
	for (int i = 0; i < TrainningSet.size(); i++)
		if (A->Ex[TrainningSet[i]]->Name == Classify)
			tmp.push_back(TrainningSet[i]);
	return tmp;
}

Node *ID3(vector <int> TrainningSet, vector<Atribute*> &A, Atribute *&Target)
{
	Node *a = new Node;
	if (AllYes(TrainningSet, Target))
	{
		a->Mode = false;
		a->Name = Target->Classify[0]->Name;
		return a;
	}
	if (AllNo(TrainningSet, Target))
	{
		a->Mode = false;
		a->Name = Target->Classify[1]->Name;
		return a;
	}
	if (AtributeEmpty(A))
	{
		a->Mode = false;
		a->Name = MostPupular(Target,TrainningSet);
		return a;
	}
	CalAG(A, Target, TrainningSet);
	int index = MinAE(A);
	a->Name = A[index]->Name;
	a->Mode = true;
	for (int i = 0; i < A[index]->Classify.size(); i++)
	{
		vector<int> tmp;
		tmp = SubTranningSet(TrainningSet, A[index], A[index]->Classify[i]->Name);
		a->BranchName.push_back(A[index]->Classify[i]->Name);
		if (tmp.empty())
		{
			Node *b = new  Node;
			b->Name = MostPupular(Target,TrainningSet);
			b->Mode = false;
			a->BrachNode.push_back(b);
		}
		else
		{
			A[index]->Flag = false;
			a->BrachNode.push_back(ID3(tmp, A, Target));
		}
	}
	return a;
}

void PrintTree(Node *root, int depth, ofstream &file)
{
	if (root->Mode)
	{
		file << endl;
		for (int i = 0; i < root->BrachNode.size(); i++)
		{
			for (int j = 0; j < depth; j++)
				file << "| \t";
			file << root->Name << " = " << root->BranchName[i] ;
			PrintTree(root->BrachNode[i],depth+1, file);
		}
	}
	else
	{
		file << ": " << root->Name << endl;
	}
}

string BrowseTree(Node *root, vector<Atribute*> Pre, int index)
{
	if (root->Mode == false)
		return root->Name;
	string tmp;
	for (int i = 0; i < Pre.size(); i++)
		if (Pre[i]->Name == root->Name)
		{
			tmp = Pre[i]->Ex[index]->Name;
			break;
		}
	for (int i = 0; i < root->BrachNode.size(); i++)
	{
		if (tmp == root->BranchName[i])
			return BrowseTree(root->BrachNode[i], Pre, index);
	}
}

void Prediction(Node *root, vector<Atribute*> Pre, Atribute *&Target)
{
	for (int i = 0; i < SizePre; i++)
	{
		Target->Ex[i]->Name = BrowseTree(root, Pre, i);
	}
}