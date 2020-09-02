
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
		priority_queue <Data>first, final;
		int Output = 0;
		//for (int i = 0; i < filenames.size(); i++)
		for (int i = 0; i < MAX; i++)
			{
			Data store;
				if (check && search.rootSearch(root[i], '"' + query + '"', stopword, store.pos, store.score))
				{
						++Output;
						store.filename = filenames[i];
						first.push(store);	
				}
				else if (search.rootSearch(root[i], query, stopword, store.pos, store.score))
				{
					++Output;
					store.filename = filenames[i];
					final.push(store);
				}
			}
		/*sort(first.begin(), first.end(), scoreCompare);
		sort(final.begin(), final.end(), scoreCompare);
		for (int i = 0; i < first.size(); i++)
			cout << first[i].filename << endl;
		for (int i = 0; i < final.size(); i++)
			cout << final[i].filename << endl;*/
		while (first.size())
		{
			Data output = first.top();
			cout << output.filename << " " << output.score << endl;
			first.pop();
		}

		while (final.size())
		{
			Data output = final.top();
			cout <<output.filename<< " "<< output.score << endl;
			final.pop();
		}
		
	}
	
}

//