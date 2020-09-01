#include "function.h"
TrieNode* Engine::wordSearch(TrieNode* root, string key, bool title) {
	TrieNode* cur = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i)
	{
		index = convert(key[i]);
		if (index == -1) continue;
		if (!cur->children[index]) return NULL;
		cur = cur->children[index];
	}
	if (cur) 
	{
		if (title && !cur->isTitle) return NULL;
		if (cur->isEnd) return cur;
	}
	return NULL;
}
bool Engine::rootSearch(TrieNode* root, string query, TrieNode* stopword, vector<int>& pos, int& score) {// score=0 initially
	stringstream ss(query);
	string tmp, get;
	while (ss >> tmp)
	{
		if (tmp[0] == '"') {//exact match and wildcard
			vector<int> local;
			TrieNode* res1 = wordSearch(root, tmp, false), * res2;
			if (!res1) return false;
			if (tmp.back() == '"')
			{
				pos = Sync(pos, res1->order);
				score += res1->order.size();//for ranking by the number of occurence
				continue;
			}
			string tmp2;
			vector<int> tmp = res1->order;
			while (ss >> tmp2) {
				int cnt = 1;
				if (tmp2 == "") return true;
				if (tmp2 == "*") {
					++cnt;
					while (ss >> tmp2) {
						if (tmp2 == "*") ++cnt;
						else break;
					}
				}
				res2 = wordSearch(root, tmp2, false);
				if (!res2) return false;
				vector<int> out1, out2;
				takeLocal(tmp, res2->order, cnt, out1, out2);
				if (out1.empty()) return false;
				vector<int > carry = Sync(out1, out2);
				local=Sync(local, carry) ;
				if (tmp2.back() == '"') {
					score += out1.size();
					break;
				}
				tmp = out2;
			}
			pos = Sync(pos, local);
			continue;
		}
		if (tmp[0] == '-') {//minus sign
			if (wordSearch(root, tmp.substr(1), false)) return false;
			continue;
		}
		get = tmp.substr(0, 8);//intitle:
		if (get == "intitle:") {
			get = tmp.substr(8);
			TrieNode* title = wordSearch(root, get, true);
			if (!title) return false;
			score += title->order.size();
			pos = Sync(pos, title->order);
			while (ss >> get) {
				title = wordSearch(root, get, true);
				if (!title) return false;
				score += title->order.size();
				pos = Sync(pos, title->order);
			}
			continue;
		}
		TrieNode* searchRes;
		if (tmp == "OR") {
			ss >> tmp;
			searchRes = wordSearch(root, tmp, false);
			if (searchRes) {
				pos = Sync(pos, searchRes->order);
				score += searchRes->order.size();
			}
			continue;
		}
		searchRes = wordSearch(root, tmp, false);
		if (tmp[0] == '+') {
			if (!searchRes) return false;
			pos = Sync(pos, searchRes->order);
			score += searchRes->order.size();
		}
		if (tmp == "AND" || wordSearch(stopword, tmp, false)) continue;
		//.if (tmp == "AND" ) continue;
		/**/
		searchRes = wordSearch(root, tmp, false);
		if (!searchRes) {
			bool check = false;
			while (ss >> tmp) {
				if (tmp == "OR") {
					ss >> tmp;
					searchRes = wordSearch(root, tmp, false);
					if (searchRes) {
						pos = Sync(pos, searchRes->order);
						score += searchRes->order.size();
						check = true;
						break;
					}
				}
				else return false;
			}
			if (!check) return false;
		}
		else {
			pos = Sync(pos, searchRes->order);
			score += searchRes->order.size();
		}
	}
	return true;
}
