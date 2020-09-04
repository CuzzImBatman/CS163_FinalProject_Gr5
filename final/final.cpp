
#include "function.h"

int main()
{
	Engine search;
	vector<string> filenames;
	TrieNode* root, * stopword = search.getNode();
	search.Init(root, stopword, filenames);

}

//