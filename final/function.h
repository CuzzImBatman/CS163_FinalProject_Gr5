#pragma once
#define MAX 123456
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<ctime>
using namespace std;
struct TrieNode {
	TrieNode* children[42];//0-9:number,10-35:words,36:maybe :,37:.(for so thap phan), 38:$, 39:%, 40:#, 41:-
	bool isLeaf, isTitle;
	vector<int> order;
};
struct Data {
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
	bool checkOperator(string query);
	vector<string> getSyn(string key);
};
bool Is_empty(ifstream& in);