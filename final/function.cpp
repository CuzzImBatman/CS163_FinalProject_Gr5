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
	if (num < 10)return '0' + ss.str();
	return ss.str();
}
bool Engine::isNumber(char key) 
{
	if (key >= 48 && key <= 57) return true;
	return false;
}
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
vector<string> Engine::getSyn(string sen) {
	vector<string> res;
	res.push_back(sen);
	if (sen[0] >= 'a' && sen[0] <= 'z') sen[0] -= 32;
	for (int i = 1; i < sen.length(); ++i)
		if (sen[i] >= 'A' && sen[i] <= 'Z') sen[i] += 32;
	ifstream file; file.open("synonym.txt");
	while (!Is_empty(file)) 
	{
		string tmp;
		getline(file, tmp);
		stringstream ss(tmp);
		ss >> tmp;
		if (tmp != "KEY:") continue;
		ss >> tmp;
		if (tmp != sen) continue;
		if (tmp > sen) return res;
		getline(file, tmp);
		ss.str("");  ss << tmp;
		ss >> tmp;
		if (tmp != "SYN:") return res;
		while (ss >> tmp)
		{
			if (tmp[0] == '[' || tmp[0] == '{') continue;
			res.push_back(tmp);
		}
		file.close();
		return res;
	}
	file.close();
	return res;
}
bool Engine::checkOperator(string query) {
	
	return true;
}

void Engine::viewHistory(string query, vector<string> &history){
    ifstream in;
    in.open("final/Search Engine-Data/history.txt");
    if (!in) {
        cout <<"Cannot open file History !\n";
        in.close();
        return;
    }
    string tmp; int i=1;
    while (!in.eof()){
        getline(in, tmp);
        if (tmp.find(query) != -1){
            history.push_back(tmp);
            cout<<i++<<". "<<tmp<<endl;
        }
    }
    in.close();
}
