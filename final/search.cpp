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
	string cur, get;
	while (ss >> cur)
	{
		if (cur[0] == '"') {
			vector<int> local;
			TrieNode* res1 = wordSearch(root, cur, false), * res2;
			if (!res1) return false;
			if (cur.back() == '"')
			{
				pos = Sync(pos, res1->order);
				score += res1->order.size();//for ranking by the number of occurence
				continue;
			}
			string cur2;
			vector<int> tmp = res1->order;
			while (ss >> cur2) {
				int cnt = 1;
				if (cur2 == "") return true;
				if (cur2 == "*") {
					++cnt;
					while (ss >> cur2) {
						if (cur2 == "*") ++cnt;
						else break;
					}
				}
				res2 = wordSearch(root, cur2, false);
				if (!res2) return false;
				vector<int> out1, out2;
				takeLocal(tmp, res2->order, cnt, out1, out2);
				if (out1.empty()) return false;
				vector<int > carry = Sync(out1, out2);
				local=Sync(local, carry) ;
				if (cur2.back() == '"') {
					score += out1.size();
					break;
				}
				tmp = out2;
			}
			pos = Sync(pos, local);
			continue;
		}
		
		get = cur.substr(0, 8);//intitle:
		if (get == "intitle:") {
			get = cur.substr(8);
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
		if (cur[0] == '-') {//minus sign
			if (wordSearch(root, cur.substr(1), false)) return false;
			continue;
		}
		TrieNode* searchRes;
		if (cur == "OR") {
			ss >> cur;
			searchRes = wordSearch(root, cur, false);
			if (searchRes) {
				pos = Sync(pos, searchRes->order);
				score += searchRes->order.size();
			}
			continue;
		}
		searchRes = wordSearch(root, cur, false);
		if (cur[0] == '+') {
			if (!searchRes) return false;
			pos = Sync(pos, searchRes->order);
			score += searchRes->order.size();
		}
		if (cur == "AND" || wordSearch(stopword, cur, false)) continue;
		
		/*if (tmp.back() == '*') {
			vector<int> res;
			searchIncomplete(root, tmp, res);
			if (res.empty()) return false;
			pos = Sync(pos, res);
			continue;
		}*/
		if (isNumber(cur[0]) == '$' || cur[0] ) {
			double lowbound, upbound;
			if (searchRange(cur, lowbound, upbound)) {
				string number="";  vector<int> local;
				if (cur[0] == '$') {
					if (!root->children[38]) return false;
					numberSearch(root->children[38], number, lowbound, upbound, local,score);
					if (local.empty()) return false;
				}
				else numberSearch(root, number, lowbound, upbound, local,score);
				if (local.empty()) return false;
				pos = Sync(pos, local);
				
				continue;
			}
		}
		searchRes = wordSearch(root, cur, false);
		if (!searchRes) {
			bool check = false;
			while (ss >> cur) {
				if (cur == "OR") {
					ss >> cur;
					searchRes = wordSearch(root, cur, false);
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
void Engine::numberSearch(TrieNode* root, string number, double lowbound, double upbound, vector<int>& local, int& score)
{
	double comp;
	if (number!="" && number.back() == '.')
	{
		string tmp = number.substr(0, number.length() - 1);
		comp = atof(tmp.c_str());
		if (comp < lowbound) return;
	}
	else  comp = atof(number.c_str());
	if(comp > upbound) return;
	if (root->isEnd && comp >= lowbound)
	{
		local = Sync(local, root->order);
		score += root->order.size();
		return;
	}

	for (int i = 0; i <= 10; ++i)
	{
		if (i == 10)i += 37;
		if (root->children[i])
		{
			char add;
			if (i == 37)add = '.';
			else add = i + '0';
			numberSearch(root->children[i], number + add, lowbound, upbound, local,score);
		}
	}
	
}
bool searchRange(string tmp, double& lowbound, double& upbpound)
{
	string l, r;
	int i;
	for (i = 0; i < tmp.length() - 1; i++)
	{
		if (tmp[i] == '.' && tmp[i + 1] == '.')break;
		l = l + tmp[i];
	}
	if (i == tmp.length() - 1)return false;
	i += 2;
	while (i < tmp.length())r = r + tmp[i++];
	if (tmp[0] == '$')
	{
		r = r.substr(1);
		l = l.substr(1);
	}
	lowbound = atof(l.c_str());
	upbpound = atof(r.c_str());

}