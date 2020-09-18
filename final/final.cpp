
#include "function.h"

using namespace std;
int main()
{
	Engine search;
	
	TrieNode* root, * stopword = search.getNode();
	clock_t begin, begin_index, t_index, t_searchE;
	begin_index =clock();
	search.Init(root, stopword);
	t_index = clock();
	cout <<endl<<"INDEX TIMING: "<< (float)(t_index - begin_index) / CLOCKS_PER_SEC << endl;
	
	while (true) {

		cout << "0. Exit" << endl << "1. Input query " << endl << "2. Clear history" << endl << "3. Re-Index" << endl;

		int choice;
		cin >> choice;
		cin.ignore();
		if (!choice) break;
		if (choice == 2) {
			clearHistory();
			continue;
		}
		string query,type="";
		cout << "Query:";
		getline(cin, query);
		int i = 0;
		{while (query[query.length() - 1 - i] == ' ')i++;
		query.erase(query.length() - i, i);
		i = 0;
		while (query[i] == ' ')i++;
		query.erase(0, i);
		}
		cout << "0. Search" << endl << "1. Display history suggestion" << endl;
		cin >> choice;
		cin.ignore();
		if (choice)
		{
			vector<string>list;
			viewHistory(query, list);
			if (list.empty())
			{
				cout << "  (no suggestions)\n";
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
		type = query.substr(0, 9);
		if (type == "filetype:")
		{
			type = query.substr(9, 3);
			query = query.substr(12);
		}
		else type = "";
		//priority_queue <Data>first, final;
		TrieNode* first=search.getNode(), * final = search.getNode();
		int num = 5;

		priority_queue <Data>out1, out2;
		begin = clock();
		if (check)search.rootSearch(root, '"' + query + '"', stopword, first,type);
		
		search.rootSearch(root, query, stopword, final, type);
		search.filtRes(first, final);
		
		
		if(first)for (int i = 0; i < first->filePos.size(); i++)
		{
			Data data;
		    data.score = first->place[first->filePos[i].pos].size();
			if (!data.score)continue;
			data.filename = search.filenames[first->filePos[i].pos];
			data.pos = first->place[first->filePos[i].pos];
			out1.push(data);

		}
		if (final)for (int i = 0; i < final->filePos.size(); i++)
		{
			Data data;
			
			data.score = final->place[final->filePos[i].pos].size();
			if (!data.score)continue;
			data.filename = search.filenames[final->filePos[i].pos];
			data.pos = final->place[final->filePos[i].pos];

			out2.push(data);

		}

		//cout << final->place[final->filePos[0].pos].size() << endl;
		
				while (out1.size() && num)
				{
					Data output = out1.top();
					
					
						cout << output.filename << endl;
						search.outputRes(output);
						cout << endl << "Point: " << output.score << endl;
						num--;
					out1.pop();
				}

				while (out2.size() && num)
				{
					Data output = out2.top();
					
						cout << output.filename << endl;
						search.outputRes(output);
						makeColor(14);
						cout << "Point: " << output.score << endl;
						makeColor(7);
						num--;
					out2.pop();
				}

				t_searchE = clock();
				cout << endl << "TOTAL RESULT: " << num;
				cout << endl << "TOTAL TIME: " << (float)(t_searchE - begin) / CLOCKS_PER_SEC << endl;

	}
	
	search.deleteTrie(root);
	search.deleteTrie(stopword);
}

//