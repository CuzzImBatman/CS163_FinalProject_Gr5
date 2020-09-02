#include "function.h"
bool Is_empty(ifstream& in)
{
	return in.peek() == ifstream::traits_type::eof();
}
TrieNode* Engine::getNode() {
	TrieNode* pNode = NULL;
	pNode = new TrieNode;

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
vector<string> Engine::getSyno(string sen) {
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
	
	stringstream line(query);
	string cur;
	while (line >> cur)
	{
		if (
			cur.substr(0, 8) == "intitle:" 
			|| cur == "AND"
			|| cur == "OR"
			|| cur[0] == '-'
			|| cur == "*"
			|| cur == "filetype:txt"
			|| cur[0] == '"'
			)
			return false;
		for (int i = 0; i < cur.length() - 2; ++i)
			if (cur[i] == '.'&& cur[i + 1] == '.')return false;
		
	}
	return true;
}
vector<int> Engine::Sync(vector<int>& local1, vector<int>& local2) {
	if (local1.empty()) return local2;
	if (local2.empty()) return local1;
	vector<int> sync;
	int i = 0, j = 0, size1 = local1.size(), size2 = local2.size();
	while (i < size1 && j < size2)
	{
		if (local1[i] < local2[j]) sync.push_back(local1[i++]);
		else if (local2[j] < local1[i]) sync.push_back(local2[j++]);
		else sync.push_back(local2[j++]); ++i;
	}
	while (i < size1) sync.push_back(local1[i++]);
	while (j < size2) sync.push_back(local2[j++]);
	return sync;
}
void viewHistory(string query, vector<string> &history){
    ifstream in;
    in.open("final/Search Engine-Data/history.txt");
    if (!in) {
        cout <<"Cannot open file History !\n";
        in.close();
        return;
    }
	cout << "suggestion: " << endl;
    string tmp; int i=1;
    while (!in.eof()){
        getline(in, tmp);
		if (tmp == query || tmp.find(query) == -1)continue;
		
            history.push_back(tmp);
            cout<<i++<<". "<<tmp<<endl;
        
    }
	in.close();
	ofstream out;
	out.open("final/Search Engine-Data/history.txt");
	out << query << endl;
	out.close();
	
    
}
void Engine::takeLocal(vector<int>& res1, vector<int>& res2, int cnt, vector<int>& place1, vector<int>& place2) {//Khanh
	int i = 0, j = 0, size1 = res1.size(), size2 = res2.size();
	while (i < size1 && j < size2) {
		if (res1[i] + cnt < res2[j]) ++i;
		else if (res2[j] < res1[i] + cnt) ++j;
		else {
			place1.push_back(res1[i++]);
			place2.push_back(res2[j++]);
		}
	}
}
bool scoreCompare(const Data a, const Data b)
{
	return a.score > b.score;
}
