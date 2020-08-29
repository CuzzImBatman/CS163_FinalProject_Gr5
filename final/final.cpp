
#include "function.h"

using namespace std;
int main()
{
	Engine search;
	TrieNode*** root, * stopword = search.getNode();
	search.Init(root, stopword);
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
			vector<string> list;
			viewHistory(query, list);
			if (list.size()) {
				do {
					cin >> choice;
					if (choice > list.size()) {
						cout << "Invalid input" << endl;
						viewHistory(query, list);
						continue;
					}
					query = list[choice];
				} while (choice > list.size());
			}
		}
		else
		{
			ofstream output; output.open("history.txt", ios::app);
			output << query << endl;
			output.close();
		}
		vector<string> synonyms;
		if (query[0] == '~') synonyms = search.getSyn(query.substr(1));
		bool check = search.checkOperator(query);


	}
}

//