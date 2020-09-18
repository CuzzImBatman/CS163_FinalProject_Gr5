#include "function.h"

void Engine::Init(TrieNode*& root, TrieNode*& stopword) {
	ifstream in;
	//in.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\text.txt");
	in.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\___index.txt");
	if (!in.is_open())cout << "cannot" << endl;
	while (!Is_empty(in))
	{
		string tmp;
		getline(in, tmp);
		filenames.push_back(tmp);
	}
	root = new TrieNode ;
	

	InputFiles(root);
	LoadStopword(stopword);

}
void Engine::InputFile(TrieNode*& root, ifstream& file, int filePos, string type)
{
	int start = 0;
	FILE* input = fopen(("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" +filenames[filePos]).c_str(), "r");
	if (!input)return;
	char key;
	bool title = true;
	int  isDec = -1;
	TrieNode* cur = root;
	do
	{
		key = getc(input);
		if (key == 39)
		{
			key = getc(input);
			if(key=='s')
				key = getc(input);
		}
		
		if ((key == '.' || key == '\n') && title) title = false;
		if (key == ' ' || key == '\n' || key == EOF || (key == '.' && isDec == -1))
		{
			if (cur != root)
			{

				cur->isEnd = true;
				local tmp = { filePos,title };
				if (cur->filePos.empty())cur->filePos.push_back(tmp);
				else if (cur->filePos.back().pos < filePos)cur->filePos.push_back(tmp);
				if (title)cur->isTitle = true;
				tmp.pos = start;
				cur->place[filePos].push_back(tmp);
				start++;
				cur = root;
			}
			
		}
		else if (key == '.' && isDec == 0)
		{
			isDec = 1; continue;
		}
		else if (isDec==1 && key >= '0' && key <= '9')
		{

			if (cur->children.find(39) == cur->children.end())
				cur->children[39] = getNode();
			cur = cur->children[39];
			if (cur->children.find(key - 48) == cur->children.end())
				cur->children[key - 48] = getNode();
			cur = cur->children[key - 48];
			isDec = -1;
			continue;
			
		}
		else if (isDec==1)
		{
			isDec = -1;
			if (cur != root)
			{
				cur->isEnd = true;
				local tmp = { filePos,title };
				if (cur->filePos.empty())cur->filePos.push_back(tmp);
				else if (cur->filePos.back().pos < filePos)cur->filePos.push_back(tmp);
				if (title)cur->isTitle = true;
				tmp.pos = start;
				cur->place[filePos].push_back(tmp);
			}
			start++;
			cur = root;
			if (!valid(key, false))continue;
			int index = convert(key);
			if (index == -1)continue;
			if (cur->children.find(index) == cur->children.end())
				cur->children[index] = getNode();
			cur = cur->children[index];
			continue;
		}

		else if (!valid(key, false))continue;
		else
		{
			int index= convert(key);
			if (cur->children.find(index) == cur->children.end())
				cur->children[index] = getNode();
			cur = cur->children[index];

		}
		if (key >= '0' && key <= '9')isDec = 0;
		else isDec = -1;

	} while (key != EOF);
	//string sentence;
	//
	//getline(file, sentence, '\n');
	//InputSentence(root, sentence, start, true,  filePos ,type);//a trie for the inside title, true means title:
	//bool remain = false;
	//while (!Is_empty(file)) {

	//	if (!remain)getline(file, sentence, '.');

	//	if (sentence.length() && isNumber(sentence.back()))
	//	{
	//		string next;
	//		getline(file, next, '.');
	//		if (next.length() && isNumber(next[0]))
	//		{
	//			sentence = sentence + '.' + next;
	//			if (isNumber(next.back()))remain = true;
	//			else
	//			{
	//				InputSentence(root, sentence, start, false, filePos, type);
	//				remain = false;
	//			}
	//		}
	//		else
	//		{
	//			InputSentence(root, sentence, start, false, filePos, type);
	//			if (next.length() && isNumber(next.back()))
	//			{
	//				remain = true; 
	//				sentence = next;
	//			}
	//			else
	//			{
	//				InputSentence(root, next, start, false, filePos, type);
	//				remain = false;
	//			}
	//			

	//		}
	//	}
	//	
	//	else InputSentence(root, sentence, start, false, filePos, type);
	//	if (Is_empty(file))
	//		InputSentence(root, sentence, start, false, filePos, type);

	//}
	fclose(input);
}
/*void Engine::InputFile(TrieNode*& root, ifstream& file)
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
}*/
void Engine::InputSentence(TrieNode*& root, string sentence, int& start, bool valid,int filePos,string type) {//start:place to start sentence
	//sentence = SenFilter(sentence,false);
	if (!root) return;
	stringstream ss(sentence);
	while (ss >> sentence) {
		if (valid) insertWord(root, sentence, start, true, filePos, type);
		else insertWord(root, sentence, start, false, filePos, type);
		++start;
	}
}
string Engine::SenFilter(string sen,bool state) {
	string res;
	int length = sen.length();
	for (int i = 0; i < length; ++i) {
		if (!valid(sen[i],state)) {
			if (sen[i] == 39 && i <= length - 3  && sen[i+1] == 's' && sen[i+2] == ' ')
                i++;
			else if (i + 1 != length && sen[i] == '.' && isNumber(sen[i - 1]) && isNumber(sen[i + 1]))//fixed number
				res.append(sen, i, 1);
		}
		else res.append(sen, i, 1);//get 1 
	}
	return res;
}

void Engine::insertWord(TrieNode*& root, string key, int place, bool valid, int filePos,string type) {
	TrieNode* cur = root, *pre=cur;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;
		
		if (cur->children.find(index) == cur->children.end())
			cur->children[index] = getNode();
		
		cur = cur->children[index];
	}
	
	cur->isEnd = true;
	local tmp = { filePos,valid };
	if (cur->filePos.empty())cur->filePos.push_back(tmp);
	else if (cur->filePos.back().pos < filePos)cur->filePos.push_back(tmp);
	tmp.pos =  place;
	
	cur->place[filePos].push_back(tmp);
	
	

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
	file.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\stopword.txt");
	if (!file.is_open()) return;
	while (!file.eof()) {
		getline(file, word);
		InsertStopword(root, word);
	}
	file.close();
}
void Engine::InputFiles(TrieNode*& root) {
	ifstream file;
	//for (int i = 0; i < filenames.size(); ++i)
	for (int i = 0; i < 500; ++i)
	{
		/*if (filenames[i] == "Vietnam.txt")
			cout << 1;*/
		file.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" + filenames[i]);
		if (!file.is_open()) { /*cout << "Cannot open file " << filenames[i] << endl;*/ continue; }
		//cout << filenames[i] << endl;
		InputFile(root, file, i, filenames[i].substr(filenames[i].size() - 3));
		file.close();
	}
}




