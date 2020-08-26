#include "function.h"
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
	return ss.str();
}
void Engine::Init(TrieNode***& root, TrieNode*& stopword) {
	root = new TrieNode * *[15];
	for (int i = 0; i < 15; ++i) {
		if (i == 9 || i == 13) root[i] = new TrieNode * [100];
		else if (i != 14) root[i] = new TrieNode * [50];
		else root[i] = new TrieNode * [800];
	}
	for (int i = 0; i < 15; ++i) {
		if (i == 9 || i == 13)
			for (int j = 0; j < 100; ++j) root[i][j] = getNode();
		else if (i != 14)
			for (int j = 0; j < 50; ++j) root[i][j] = getNode();
		else for (int j = 800; j < 1600; ++j) root[i][j - 800] = getNode();
	}
	
	//InputListFile(root);
	//InsertStopword(stopword);
	
}