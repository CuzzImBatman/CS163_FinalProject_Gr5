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
	for (int i = 0; i < 26; ++i) 
		if(i==25)root[i] = new TrieNode * [MAX];
		 else root[i] = new TrieNode * [100];
	
	for (int i = 0; i < 26; ++i)
		if (i != 25)
			for (int j = 0; j < 100; ++j) root[i][j] = getNode();
		else
			for (int j = 0; j < MAX; ++j)root[i][j] = getNode();
		
	
	
	InputListFile(root);
	LoadStopword(stopword);
	
}
void Engine::InputFile(TrieNode*& root, ifstream& file)
{
}
void Engine::InsertStopword(TrieNode*& root, string key) {
	TrieNode* cur = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;
		if (!cur->children[index]) cur->children[index] = getNode();
		cur = cur->children[index];
	}
	cur->isLeaf = true;
}
void Engine::LoadStopword(TrieNode*& root) {
	ifstream file;
	string word;
	file.open("stopword.txt");
	if (!file.is_open()) return;
	while (!file.eof()) {
		getline(file, word);
		InsertStopword(root, word);
	}
	file.close();
}
void Engine::InputListFile(TrieNode***& root) {
	ifstream file;
	for (int i = 1; i <= 26; ++i)
	{
		int limit = 100;
		if (i == 26)limit = MAX;

			for (int j = 1; j <= limit; ++j) {
				string filename = OpenFile(i, j);
				file.open(filename);
				if (!file.is_open()) { cout << "Cannot open file " << filename << endl; continue; }
				cout << i << " " << j << endl;
				InputFile(root[i - 1][j - 1], file);
				file.close();
			}
	}
	
}
string Engine::OpenFile(int i, int j)
{
	string group = NumberToString(i), number = NumberToString(j);
	if (i == 26)
		if (j < 10)return ("Data" + '0' + number + ".txt");
		else return("Data" + number + ".txt");
	return ("Group" + group + "News" + number + ".txt");

};
int Engine::convert(char key) {
		if (key >= 48 && key <= 57) return (key - '0');
		if (key >= 97 && key <= 122) return (int(key) - 87);
		if (key >= 65 && key <= 90) return (int(key) - 55);
		if (key == ' ') return 36;
		if (key == '.') return 37;
		if (key == '$') return 38;
		if (key == '%') return 39;
		if (key == '#') return 40;
		if (key == '-') return 41;
		return -1;
	}