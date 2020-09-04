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
		
	}
	return pNode;
}
string Engine::NumberToString(int num) {
	stringstream ss;
	
	ss << num;
	if (num < 10)return '0'+ ss.str();
	return ss.str();
}
void Engine::Init(TrieNode*& root, TrieNode*& stopword, vector<string>& filenames) {
	ifstream in;
	in.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\___index.txt");
	if (!in.is_open())cout << "cannot" << endl;
	while (!Is_empty(in))
	{
		string tmp;
		getline(in, tmp);
		filenames.push_back(tmp);
	}
	
		root = getNode();
	//for (int i = 0; i < MAX; i++)	root[i] = new TrieNode;

	InputListFile(root, filenames);
	LoadStopword(stopword);
	
}
void Engine::InputFile(TrieNode*& root, ifstream& file, int filePos)
{
	int start = 0;
	string sentence; 
	getline(file, sentence,'\n');
	InputSentence(root, sentence, start, true, filePos);//a trie for the inside title, true means title:
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
					InputSentence(root, sentence, start, false, filePos);
					remain = false;
				}
			}
			else
			{
				InputSentence(root, sentence, start, false, filePos);
				InputSentence(root, next, start, false, filePos);
				remain = false;
				
			}
		}
		else InputSentence(root, sentence, start, false, filePos);
		
	}
}
void Engine::InputSentence(TrieNode*& root, string sentence, int& start, bool valid,int filePos) {//start:place to start sentence
	sentence = SenFilter(sentence);
	if (!root) return;
	stringstream ss(sentence);
	while (ss >> sentence) {
		if (valid) insertWord(root, sentence, start, true, filePos);
		else insertWord(root, sentence, start, false, filePos);
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
	if (key == '—') {
		key = '-'; return true;
	}
	if (key == ' ' || key == '$' || key == '%' || key == '#' || key == '-') return true;
	return false;
}
void Engine::insertWord(TrieNode*& root, string key, int place, bool valid,int filePos) {
	TrieNode* cur = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;
		if (!cur->children[index]) cur->children[index] = getNode();
		cur = cur->children[index];
	}
	cur->isLeaf = true;
	if (cur->filePos.empty())cur->filePos.push_back(filePos);
	else if (cur->filePos.back() < filePos)cur->filePos.push_back(filePos);
	cur->loca[filePos].push_back(place);
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
void Engine::InputListFile(TrieNode*& root, vector<string>& filenames) {
	ifstream file;
	for (int i = 0; i < filenames.size(); ++i) 
	//r (int i = 0; i < MAX; ++i)
	{
		file.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" + filenames[i]);
		if (!file.is_open()) { cout << "Cannot open file " << filenames[i] << endl; continue; }
		cout << filenames[i] << endl;
		InputFile(root, file,i);
		file.close();
	}
	
}
string Engine::OpenFile(int i, int j)
{
	string group = NumberToString(i), number = NumberToString(j);
	if (i == 26)
		if (j < 10)return ("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" "Data" +  '0' + number + ".txt");
		else return("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" "Data" + number + ".txt");
	return ("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" "Group" + group + "News" + number + ".txt"); 

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