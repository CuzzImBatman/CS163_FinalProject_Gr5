#pragma once
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
struct Store {
	string filename;
	vector<int> pos;
	int score;
};
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
class Engine {
public:
	TrieNode* getNode();
	string NumberToString(int num);
	void Init(TrieNode***& root, TrieNode*& stopword);
	//void insertWord
	//void insertStopword
	
};
