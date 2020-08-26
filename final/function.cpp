#include "function.h"
TrieNode* Engine::getNode() {
	TrieNode* pNode = NULL;
	pNode = new TrieNode;
	if (pNode) {
		pNode->isLeaf = false;
		pNode->isTitle = false;
		for (int i = 0; i < 42; ++i) pNode->children[i] = NULL;
	}
	return pNode;
}
string Engine::NumberToString(int num) {
	stringstream ss;
	
	ss << num;
	if (num < 10)return '0'+ ss.str();
	return ss.str();
}
void Engine::Init(TrieNode***& root, TrieNode*& stopword) {
	root = new TrieNode * *[15];
	for (int i = 0; i < 15; ++i) 
		if(i==14)root[i] = new TrieNode * [MAX];
		 else root[i] = new TrieNode * [100];
	
	for (int i = 0; i < 15; ++i)
		if (i != 14)
			for (int j = 0; j < 100; ++j) root[i][j] = getNode();
		else
			for (int j = 0; j < MAX; ++j)root[i][j] = getNode();
		
	
	
	InputListFile(root);
	//InsertStopword(stopword);
	
}
void Engine::InputFile(TrieNode*& root, ifstream& file)
{
}
void Engine::InputListFile(TrieNode***& root) {
	ifstream file;
	for (int i = 1; i <= 15; ++i) 
			for (int j = 1; j <= 100; ++j) {
				string filename = OpenFile(i, j);
				file.open(filename);
				if (!file.is_open()) { cout << "Cannot open file " << filename << endl; continue; }
				cout << i << " " << j << endl;
				InputFile(root[i - 1][j - 1], file);
				file.close();
			}
	
}
	string Engine::OpenFile(int i, int j) {
		string group = NumberToString(i), number = NumberToString(j);
		if (i == 14)
			if (j < 10)return ("Data" + '0' + number + ".txt");
			else return("Data" + number + ".txt");
	 return ("Group" + group + "News" + number + ".txt");
	
	}