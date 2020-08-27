#include "function.h"
bool Is_empty(ifstream& in)
{
	return in.peek() == ifstream::traits_type::eof();
}
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
	root = new TrieNode * *[25];
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
	int start = 0;
	string sentence; 
	getline(file, sentence,'\n');
	InputSentence(root, sentence, start, true);//a trie for the inside title, true means title:
	bool remain = false;
	while (!Is_empty(file)) {
		
		if (!remain)getline(file, sentence, '.');
		else
		{
			string carry;
			getline(file, carry, '.');
			sentence = sentence + '.' + carry;
		}

		if (sentence.length() && isNumber(sentence.back())) 
		{
			string next; 
			getline(file, next, '.');
			if (next.length() && isNumber(next[0])) 
			{
				sentence = sentence + '.' + next;
				if(isNumber(next.back()))remain = true;
				else
				{
					InputSentence(root, sentence, start, false);
					remain = false;
				}
			}
			else
			{
				InputSentence(root, sentence, start, false);
				InputSentence(root, next, start, false);
				remain = false;
				
			}
		}
		else InputSentence(root, sentence, start, false);
		
	}
}
void Engine::InputSentence(TrieNode*& root, string sentence, int& start, bool valid) {//start:place to start sentence
	sentence = SenFilter(sentence);
	if (!root) return;
	stringstream ss(sentence);
	while (ss >> sentence) {
		if (valid) insertWord(root, sentence, start, true);
		else insertWord(root, sentence, start, false);
		++start;
	}
}
bool Engine::isNumber(char key) {
	if (key >= 48 && key <= 57) return true;
	return false;
}
string Engine::SenFilter(string sen) {
	string res;
	int length = sen.length();
	for (int i = 0; i < length; ++i) {
		if (!valid(sen[i])) {
			if (sen[i] == 39 && sen[i + 1] == 's' && sen[i + 2] == ' ') ++i;
			else if (i + 1 != length && sen[i] == '.' && isNumber(sen[i - 1]) && isNumber(sen[i + 1]))//so thap phan
				res.append(sen, i, 1);
		}
		else res.append(sen, i, 1);//get 1 
	}
	return res;
}

bool Engine::valid(char& key) {
	if (key >= 65 && key <= 90) {
		key = int(key) + 32;
		return true;
	}
	if ((key >= 48 && key <= 57) || (key >= 97 && key <= 122)) return true;
	if (key == '\n') {
		key = ' '; return true;
	}
	if (key == '�') {
		key = '-'; return true;
	}
	if (key == ' ' || key == '$' || key == '%' || key == '#' || key == '-') return true;
	return false;
}
void Engine::insertWord(TrieNode*& root, string key, int place, bool valid) {
	TrieNode* cur = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;
		if (!cur->children[index]) cur->children[index] = getNode();
		cur = cur->children[index];
	}
	cur->isLeaf = true;
	cur->order.push_back(place);
	if (valid) cur->isTitle = true;
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
		if (j < 10)return ("C:\\Users\\User\\Documents\\APCS material\\CS163\\Project\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" "Data" +  '0' + number + ".txt");
		else return("C:\\Users\\User\\Documents\\APCS material\\CS163\\Project\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" "Data" + number + ".txt");
	return ("C:\\Users\\User\\Documents\\APCS material\\CS163\\Project\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" "Group" + group + "News" + number + ".txt"); 

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