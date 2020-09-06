
#include "function.h"

using namespace std;
int main()
{
	Engine search;
	vector<string> filenames;
	TrieNode* root, * stopword = search.getNode();
	search.Init(root, stopword, filenames);
	while (true) {

		cout << "0. Exit" << endl << "1. Input query " << endl << "2. Clear history" << endl;
		int choice;
		cin >> choice;
		cin.ignore();
		if (!choice) break;
		if (choice == 2) {
			clearHistory();
			continue;
		}
		string query;
		cout << "Query:";
		getline(cin, query);
		lowCase(query);
		cout << "0. Search" << endl << "1. Display history suggestion" << endl;
		cin >> choice;
		cin.ignore();
		if (choice)
		{
			vector<string>list;
			viewHistory(query, list);
			if (list.empty())
			{
				cout << "No suggestion!";
				continue;
			}
			cin >> choice;
			if (choice > list.size())cout << "Invalid input" << endl;
			query = list[choice];
		}
		else
		{
			ofstream output; output.open("history.txt", ios::app);
			output << query << endl;
			output.close();
		}
		bool check = search.checkOperator(query);
		query = filter(query);
		//priority_queue <Data>first, final;
		TrieNode* first, * final = search.getNode();
		int Output = 0;

		Data store;

		if (check && search.rootSearch(root, '"' + query + '"', stopword, first))
		{
			++Output;
			//store.filename = filenames[i];
			//first.push(store);
		}
		else if (search.rootSearch(root, query, stopword, final))
		{
			++Output;
			//store.filename = filenames[i];
			//final.push(store);
		}


		//cout << final->place[final->filePos[0].pos].size() << endl;
		//sort(first->filePos.begin(), first->filePos.end(),
			/*	while (first.size())
				{
					Data output = first.top();
					if (output.filename == "Group05News26.txt")
					{
						cout << output.filename << endl;
						search.outputRes(output);
						cout << endl << "Point: " << output.score << endl;
					}
					first.pop();



				}

				while (final.size())
				{
					Data output = final.top();
					if (output.filename == "Group05News26.txt")
					{
						cout << output.filename << endl;
						search.outputRes(output);
						makeColor(14);
						cout << "Point: " << output.score << endl;
						makeColor(7);
					}
					final.pop();
				}*/



	}
	search.deleteTrie(root);
	search.deleteTrie(stopword);
}

//