#pragma once
#define MAX 123
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<ctime>
#include<unordered_map>
using namespace std;
struct TrieNode {

	unordered_map<int, TrieNode*> children;
	bool isLeaf, isTitle;
	vector<int> order;
};
struct Store {
	string filename;
	vector<int> pos;
	int score;
};

class Engine {
public:
	TrieNode* getNode();
	string NumberToString(int num);
	void Init(TrieNode***& root, TrieNode*& stopword);
	void LoadStopword(TrieNode*& stopword);
	void InsertStopword(TrieNode*& stopword,string word);
	void InputFile(TrieNode*& root, ifstream& file);
	void InputListFile(TrieNode***& root);
	string OpenFile(int i, int j);
	int convert(char key);
	void InputSentence(TrieNode*& root, string sen, int& start, bool valid);
	string SenFilter(string sen);
	void insertWord(TrieNode*& root, string key, int place, bool isTitle);
	bool valid(char& key);
	bool isNumber(char key);
};
bool Is_empty(ifstream& in);