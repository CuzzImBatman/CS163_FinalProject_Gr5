
#pragma once
#define MAX 500
#define ColorCode_DarkWhite		7
#define ColorCode_Grey			8
#define ColorCode_Blue			9
#define ColorCode_Green			10
#define ColorCode_Yellow		14
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include <queue>
#include <unordered_map> 
#include <stdio.h>
#include <conio.h>
#include <windows.h>
using namespace std;
struct local
{
	int pos;
	bool isTitle = false;
};
struct TrieNode {
	unordered_map<int, TrieNode*> children;//0-9:number,10-35:words,36:maybe :,37:.(for so thap phan), 38:$, 39:%, 40:#, 41:-
	bool isTitle= false, isEnd =false;
	vector<local> order;
};
struct Data {
	string filename;
	vector<local> pos;
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
	void InputFiles(TrieNode**& root, vector<string>& filenames);
	void InputSentence(TrieNode*& root, string sen, int& start, bool valid);
	string SenFilter(string sen ,bool state);
	void insertWord(TrieNode*& root, string key, int place, bool isTitle);
	bool valid(char& key,bool state);
	bool isNumber(char key);
	bool checkOperator(string query);
	vector<string> getSynon(string key);
	TrieNode* wordSearch(TrieNode* root, string key, bool title);
	bool rootSearch(TrieNode* root, string query, TrieNode* stopword, vector<local>& pos, int& score);
	vector<local> Sync(vector<local>& v1, vector<local>& v2);
	void numberSearch(TrieNode* root, string number, double lowbound, double upbound, vector<local>& local, int& score);
	void takeLocal(vector<local>& res1, vector<local>& res2, int cnt, vector<local>& place1, vector<local>& place2);
	TrieNode* synonSearch(TrieNode* root, string key);
   // void deleteTrie(TrieNode ***&root);
	void outputRes(Data file);
	void outputPross(vector<local>& local, int& cur, int& testlength, string sen);
	int convert(char key);
};
bool Is_empty(ifstream& in);
void viewHistory(string query, vector<string>& history);
void clearHistory();
bool scoreCompare(const Data a, const Data b);
bool searchRange(string tmp, double& lowbound, double& upbpound);
int wordsNum(string sen);
void Up(string word);
void makeColor(int color);
string wash(string sen);