#include "function.h"

void Engine::Init(TrieNode**& root, TrieNode*& stopword, vector<string>& filenames) {
	ifstream in;
	in.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\___index.txt");
	if (!in.is_open())cout << "cannot" << endl;
	while (!Is_empty(in))
	{
		string tmp;
		getline(in, tmp);
		filenames.push_back(tmp);
	}
	root = new TrieNode * [filenames.size()];
	for (int i = 0; i < filenames.size(); i++)	root[i] =new TrieNode;
	//for (int i = 0; i < MAX; i++)	root[i] = new TrieNode;

	InputFiles(root, filenames);
	LoadStopword(stopword);

}
void Engine::InputFile(TrieNode*& root, ifstream& file)
{
	int start = 0;
	string sentence;
	getline(file, sentence, '\n');
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
				if (isNumber(next.back()))remain = true;
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

void Engine::insertWord(TrieNode*& root, string key, int place, bool valid) {
	TrieNode* cur = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;

		if (cur->children.find(index) == cur->children.end())
			cur->children[index] = getNode();
		cur = cur->children[index];
	}
	
	cur->isEnd = true;
	cur->order.push_back(place);
	if (valid) cur->isTitle = true;
}
void Engine::InsertStopword(TrieNode*& root, string key) {
	TrieNode* cur = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;
		
		if (cur->children.find(index) == cur->children.end())
			cur->children[index] = getNode();
		cur = cur->children[index];
	}
	cur->isEnd = true;
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
void Engine::InputFiles(TrieNode**& root, vector<string>& filenames) {
	ifstream file;
	//for (int i = 0; i < filenames.size(); ++i) 
	for (int i = 0; i < MAX; ++i) 
	{
		file.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\"+filenames[i]);
		if (!file.is_open()) { cout << "Cannot open file " << filenames[i] << endl; continue; }
		//cout << filenames[i] << endl;
		InputFile(root[i], file);
		file.close();
	}
}

/*void deleteRoot(TrieNode *&root){
    for (int i=0;i<42;++i){
        if (root.children[i])
            deleteRoot(root.children[i]);
    }
    delete root;
}*/
void deleteTrie(TrieNode**& root, int n) {
	for (int i = 0; i < n; ++i);
        //deleteRoot(root[i]);
}

