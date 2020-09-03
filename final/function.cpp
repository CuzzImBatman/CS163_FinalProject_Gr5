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
	//if (key == 39) return 39;
	if (key == '#') return 40;
	if (key == '-') return 41;
	if (key == '%') return 39;

	return -1;
}
bool Engine::valid(char& key,bool state) {
	if (key >= 65 && key <= 90) {
		if (!state)key = int(key) + 32;
		return true;
	}
	if ((key >= 48 && key <= 57) || (key >= 97 && key <= 122)) return true;
	if (key == '\n') {
		key = ' '; return true;
	}
	if (key == '�') {
		key = '-'; return true;
	}
	if (key == 39 && state)return true;
	if (key == ' ' || key == '$' || key == '%' || key == '#' || key == '-') return true;
	return false;
}
vector<string> Engine::getSynon(string sen) {
	vector<string> res;
	res.push_back(sen);
	if (sen[0] >= 'a' && sen[0] <= 'z') sen[0] -= 32;
	for (int i = 1; i < sen.length(); ++i)
		if (sen[i] >= 'A' && sen[i] <= 'Z') sen[i] += 32;
	ifstream file; file.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\synonym.txt");
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
			if (tmp[0] >= 65 && tmp[0] <= 90)tmp[0] += 32;
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
			//|| cur == "*"
			|| cur == "filetype:txt"
			|| cur[0] == '"'
			)
			return false;
		for (int i = 0; i < cur.length() - 2; ++i)
			if (cur[i] == '.'&& cur[i + 1] == '.')return false;
		
	}
	return true;
}
vector<local> Engine::Sync(vector<local>& local1, vector<local>& local2) {
	if (local1.empty()) return local2;
	if (local2.empty()) return local1;
	vector<local> sync;
	int i = 0, j = 0, size1 = local1.size(), size2 = local2.size();
	while (i < size1 && j < size2)
	{
		if (local1[i].pos < local2[j].pos) sync.push_back(local1[i++]);
		else if (local2[j].pos < local1[i].pos) sync.push_back(local2[j++]);
		else { sync.push_back(local2[j++]); ++i; }
	}
	while (i < size1) sync.push_back(local1[i++]);
	while (j < size2) sync.push_back(local2[j++]);
	return sync;
}
void viewHistory(string query, vector<string> &history) {
	bool existQuery = false;
    ifstream in("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\history.txt");
    if (!in.is_open()) {
        cout <<"Cannot open file History!\n";
    }
	else {
		cout << "Suggestion: " << endl;
		string tmp; int i = 1;
		while (!in.eof()) {
			getline(in, tmp);
			if (tmp == query || tmp.find(query) != -1) {
				existQuery = true;
				continue;
			}
			history.push_back(tmp);
			cout << i++ << ". " << tmp << endl;
		}
		in.close();
	}
	if (!existQuery) {
		ofstream out("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\history.txt");
		if (!out.is_open()) {
			cout << "(history not updated)" << endl;
			return;
		}
		out << query << endl;
		out.close();
	}
}
void clearHistory() {
	ofstream ofs("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\history.txt", ofstream::out | ofstream::trunc);
	if (ofs.is_open())
		ofs.close();
}
void Engine::takeLocal(vector<local>& res1, vector<local>& res2, int cnt, vector<local>& place1, vector<local>& place2) {//Khanh
	int i = 0, j = 0, size1 = res1.size(), size2 = res2.size();
	while (i < size1 && j < size2) {
		if (res1[i].pos + cnt < res2[j].pos) ++i;
		else if (res2[j].pos < res1[i].pos + cnt) ++j;
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
void Engine::outputRes(Data file)
{
	ifstream in;
	in.open("D:\\CS163_FinalProject_Gr5\\final\\Search Engine-Data\\" + file.filename);
	string title,sentence,next;
	cout << endl << "===================================================================================" << endl;
	getline(in, title);
	makeColor(10);
	cout << title << endl;
	makeColor(7);
	in.seekg(0, ios::beg);
	int cur = 0, testLength = -1, pos = 0;

	bool remain = false;
	while (!Is_empty(in))
	{
		if (!remain)getline(in, sentence, '.');

		if (sentence.length() && isNumber(sentence.back()))
		{

			getline(in, next, '.');
			if (next.length() && isNumber(next[0]))
			{
				sentence = sentence + '.' + next;
				if (isNumber(next.back()))
				{
					remain = true; next = ""; continue;
				}
				else
				{
					outputPross(file.pos,cur, testLength, sentence);
					if (!file.pos.size())
					{
						in.close();
						return;
					}
					remain = false; next = "";
				}
			}
			else
			{
				outputPross(file.pos, cur, testLength, sentence);
				if (!file.pos.size())
				{
					in.close();
					return;
				}
				if (next.length() && isNumber(next.back()))
				{
					sentence = next;
					remain = true;
				}
				else
				{
					outputPross(file.pos, cur, testLength, next);
					if (!file.pos.size())
					{
						in.close();
						return;
					}
					remain = false;
				}
			}
		}
		else 
		{
			outputPross(file.pos, cur, testLength, sentence);
			if (!file.pos.size())
			{
				in.close();
				return;
			}
		}
		if (Is_empty(in))
			{
			outputPross(file.pos, cur, testLength, sentence);
			if (!file.pos.size())
			{
				in.close();
				return;
			}
		}
	}

}
void Engine::outputPross(vector<local> &local, int &cur, int &testlength,string sen)
{
	//if (sen.back() == '\n')sen.back() = ' ';
	sen = SenFilter(sen,true);
	int num = wordsNum(sen);
	testlength += num;
	if (local[0].pos > testlength)
	{
		cur += num;
		return;
	}
	makeColor(8);
	cout << "...";
	makeColor(7);

	stringstream ss(sen);
	while (ss >> sen)
	{
		if (!local.empty() && local[0].pos == cur)
		{
			Up(sen);
			for (int i = 0; i < sen.length(); i++)
			{
				
				if (sen[i] != 39 && valid(sen[i], true))makeColor(9);
				else if (sen[i] == 39 && i <= sen.length() - 2 && sen[i + 1] == 's')cout << sen[i++];
				cout << sen[i] ;
				makeColor(7);
			}
			local.erase(local.begin());
			cout << " ";
		}
		else
			cout << sen << " ";
			cur++;

		

	}
	makeColor(8);
	cout << "...";
	makeColor(7); 
	cout<< endl;

	

}
int wordsNum(string sen)
{
	stringstream ss(sen);
	string tmp;
	int num=0;
	while (ss >> tmp)num++;
	return num;
}
void Up(string word)
{
	for (int i = 0; i < word.length(); i++)
		if (word[i] >= 'a' && word[i] <= 'z')word[i] -= 32;
}
void makeColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
string wash(string sen)
{
	string res;
	int length = sen.length();
	for (int i = 0; i < length; ++i) {
		if (sen[i] == 39)
			if (i <= length - 3 && sen[i + 1] == 's' && sen[i + 2] == ' ') i++;
			else continue;//so thap phan


		else res.append(sen, i, 1);//get 1 
	}
	return res;
}