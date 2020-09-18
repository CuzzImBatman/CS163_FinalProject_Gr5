
#pragma once
#define MAX 500
#define ColorCode_DarkWhite		7
#define ColorCode_Grey			8
#define ColorCode_Blue			9
#define ColorCode_Green			10
#define ColorCode_Yellow		14
#define _CRT_SECURE_NO_WARNINGS
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
#include<ctime>
using namespace std;
struct local
{
	int pos;
	bool isTitle = false;
	
};
struct TrieNode {
	unordered_map<int, TrieNode*> children;
	bool isTitle= false, isEnd =false;
	vector<local> filePos;
	unordered_map<int, vector<local>>place;
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
	vector<string> filenames;
	TrieNode* getNode();
	string NumberToString(int num);
	void Init(TrieNode*& root, TrieNode*& stopword);
	void LoadStopword(TrieNode*& stopword);
	void InsertStopword(TrieNode*& stopword,string word);
	void InputFile(TrieNode*& root, ifstream& file, int filePos, string type);
	void InputFiles(TrieNode*& root);
	void InputSentence(TrieNode*& root, string sen, int& start, bool valid, int filePos,string type);
	string SenFilter(string sen ,bool state);
	void insertWord(TrieNode*& root, string key, int place, bool isTitle ,int filePos,string type);
	bool valid(char& key,bool state);
	bool isNumber(char key);
	bool checkOperator(string query);
	vector<string> getSynon(string key);
	TrieNode* wordSearch(TrieNode* root, string key, bool title,string type);
	bool rootSearch(TrieNode* root, string query, TrieNode* stopword, TrieNode*& data,string type);
	TrieNode* IncompleteSearch(TrieNode* root, string key, int x, bool title,string type);
	vector<local> Sync(vector<local>& v1, vector<local>& v2);
	void posFilter(TrieNode* word1, TrieNode* word2,int cnt, TrieNode*&pos1, TrieNode*& pos2);
	void numberSearch(TrieNode* root, string number, double lowbound, double upbound, TrieNode*& local);
	void takeLocal(vector<local>& res1, vector<local>& res2, int cnt, vector<local>& place1, vector<local>& place2);
	TrieNode* synonSearch(TrieNode* root, string key);
	TrieNode* Unify(TrieNode* word1, TrieNode* word2);
    void deleteTrie(TrieNode*&root);
	void outputRes(Data file);
	void outputPross(vector<local>& local, int& cur, int& testlength, string sen);
	int convert(char key);
	void filtRes(TrieNode*& res1, TrieNode*& res2);
	vector<local> typeFilter(vector<local> filePos, string type);
};
bool Is_empty(ifstream& in);
void viewHistory(string query, vector<string>& history);
bool scoreCompare(const Data a, const Data b);
bool searchRange(string tmp, double& lowbound, double& upbpound);
int wordsNum(string sen);
void upCase(string &word);
void makeColor(int color);
string filter(string sen);
vector<local> searchTrue(vector<local> pos);
void lowCase(string &sen);

