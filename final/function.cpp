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
	for (int i = 0; i < 15; ++i) 
		 root[i] = new TrieNode * [100];
	
	for (int i = 0; i < 15; ++i) {
		
			for (int j = 0; j < 100; ++j) root[i][j] = getNode();
		
	
	
	//InputListFile(root);
	//InsertStopword(stopword);
	
}