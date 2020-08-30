
#include "function.h"

using namespace std;
int main()
{
	Engine search;
	vector<string> filenames;
	TrieNode** root, * stopword = search.getNode();
	search.Init(root, stopword, filenames);
	while (1) {

		cout << "0. Exit" << endl << "1. Input query " << endl << "2. Clear history" << endl;
		int choice;
		cin >> choice;
		cin.ignore();
		if (!choice) break;
		if (choice == 2) {
			//clearHistory();
			continue;
		}
		string query;
		cout << "Query:";
		getline(cin, query);
		cout << "0. Search" << endl << "1. Display history suggestion" << endl ;
		cin >> choice;
		cin.ignore();
		if (choice)
		{
			vector<string>list;
		viewHistory(query, list);
		cin >> choice;
		if (choice > list.size()-1)cout << "Invalid input" << endl;
		query = list[choice];
		}
		else
		{
			ofstream output; output.open("history.txt", ios::app);
			output << query << endl;
			output.close();
		}
		//vector<string> synonyms;
		//if (query[0] == '~') synonyms = search.getSyn(query.substr(1));
		bool check = search.checkOperator(query);
		for (int i = 0; i < filenames.size()-1; i++)
			{
				if (check);
			}

	}
}

//