
#pragma once
#define MAX 1234

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include <queue>
using namespace std;
struct TrieNode {
	TrieNode* children[42];//0-9:number,10-35:words,36:maybe :,37:.(for so thap phan), 38:$, 39:%, 40:#, 41:-
	bool isLeaf, isTitle;
	vector<int> order;
};
struct Data {
	string filename;
	vector<int> pos;
	int score=0;
	bool operator<(const Data& o) const
	{
		return score < o.score;
	}
};

class Engine {
public:
	TrieNode* getNode();
	string NumberToString(int num);
	void Init(TrieNode**& root, TrieNode*& stopword, vector<string> &filenames);
	void LoadStopword(TrieNode*& stopword);
	void InsertStopword(TrieNode*& stopword,string word);
	void InputFile(TrieNode*& root, ifstream& file);
	void InputListFile(TrieNode**& root, vector<string>& filenames);
	int convert(char key);
	void InputSentence(TrieNode*& root, string sen, int& start, bool valid);
	string SenFilter(string sen);
	void insertWord(TrieNode*& root, string key, int place, bool isTitle);
	bool valid(char& key);
	bool isNumber(char key);
	bool checkOperator(string query);
	vector<string> getSyn(string key);
	TrieNode* wordSearch(TrieNode* root, string key, bool title);
	bool rootSearch(TrieNode* root, string query, TrieNode* stopword, vector<int>& pos, int& score);
	vector<int> Sync(vector<int>& v1, vector<int>& v2);

	void takeLocal(vector<int>& res1, vector<int>& res2, int cnt, vector<int>& place1, vector<int>& place2);
   // void deleteTrie(TrieNode ***&root);
};
bool Is_empty(ifstream& in);
void viewHistory(string query, vector<string>& history);
bool scoreCompare(const Data a, const Data b);

