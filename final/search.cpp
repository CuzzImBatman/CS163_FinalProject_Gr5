#include "function.h"
TrieNode* Engine::wordSearch(TrieNode* root, string key, bool title,string type) {
	TrieNode* cur = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i)
	{
		index = convert(key[i]);
		if (key[i] == '*')
		{
			TrieNode* res = IncompleteSearch(root, key, i, title,type);
			if (res)return res;
			return NULL;

		}
		if (index == -1) continue;
		if (!cur->children[index]) return NULL;
		cur = cur->children[index];
	}
	if (cur) 
	{
		if (title && !cur->isTitle) return NULL;
		if(type!="")cur->filePos = typeFilter(cur->filePos, type);
		if (cur->isEnd)
			if (title)
			{
				TrieNode* res = getNode();
				res->isEnd = true;
				res->isTitle = true;
				int i = 0;
				res->filePos = searchTrue(cur->filePos);
				if (res->filePos.empty())return NULL;
				while (i < res->filePos.size())
				{
					res->place[res->filePos[i].pos] = searchTrue(cur->place[res->filePos[i].pos]);
					i++;
				}
				return res;
			}
			else return cur;
	}
	return NULL;
}
TrieNode* Engine::IncompleteSearch(TrieNode* root, string key, int x, bool title,string type)
{
	TrieNode* res;
	if (x == key.length()-1)
		res = wordSearch(root, "", title, type);
	else res = wordSearch(root, key.substr(x + 1), title, type);

	for (int i = 0; i < 42; i++)
		if (root->children[i])
			if (x == key.length()-1)
				res = Unify(res, wordSearch(root->children[i], "", title, type));
			else res = Unify(res, wordSearch(root->children[i], key.substr(x + 1), title, type));
	return res;
}
vector<local> searchTrue(vector<local> pos)
{
	vector<local>res;
	int i = 0;
	while (i < pos.size())
	
		if (pos[i].isTitle)res.push_back(pos[i++]);
	return res;
}
TrieNode* Engine::synonSearch(TrieNode* root, string key)
{
	vector<string>store = getSynon(key.substr(1));
	TrieNode* synon=getNode();
	for (int i = 0; i < store.size(); i++)
	{
		TrieNode* cur = wordSearch(root, store[i],false,"");
		if (!cur)continue;
		synon = Unify(synon, cur);
	}
	return synon;
}
bool Engine::rootSearch(TrieNode* root, string query, TrieNode* stopword, TrieNode*& data,string type) {// score=0 initially
	stringstream ss(query);
	string cur, get;
	data = getNode();
	bool isTitle = false, search = false;

	while (ss >> cur)
	{
		if (cur[0] == '"') {
			search = true;
			TrieNode* Local=getNode();
			TrieNode* res1 = wordSearch(root, cur, isTitle, type), * res2;
			if (!res1) return false;
			if (cur.back() == '"')
			{
				data = Unify(data, res1);
				//score += res1->order.size();//for ranking by the number of occurence
				continue;
			}
			string cur2;
			TrieNode* tmp = res1;
			while (ss >> cur2) 
			if(search)
			{
				int space = 1;
				if (cur2 == "")
					break;
				if (cur2 == "*") {
					++space;
					while (ss >> cur2) {
						if (cur2 == "*") ++space;
						else break;
					}
				}
				res2 = wordSearch(root, cur2, isTitle, type);
				if (!res2) search= false;
				TrieNode* out1, *out2;
				posFilter(tmp, res2, space, out1, out2);
				if (out1->filePos.empty()) search = false;
				TrieNode* carry = Unify(out1, out2);
				Local= Unify(Local, carry) ;
				if (cur2.back() == '"') 
						break;
				
				tmp = out2;
			}

			data = Unify(data, Local);
			continue;
		}
		
		get = cur.substr(0, 8);//intitle:
		if (get == "intitle:") {
			/*get = cur.substr(8);
			TrieNode* title = wordSearch(root, get, true);
			if (!title) return false;
			
			data = Unify(data, title);
			

			while (ss >> get) {
				title = wordSearch(root, get, true);
				if (!title) return false;
				data = Unify(data, title);
				}*/
			cur = cur.substr(8);
			isTitle = true;
			continue;
		}
		
		if (cur[0] == '-') {//minus sign
			if (wordSearch(root, cur.substr(1), isTitle, type)) return false;
			continue;
		}
		if (cur == "OR" || wordSearch(stopword, cur, isTitle, type)) continue;
		TrieNode* searchRes;
		if (cur == "AND") {
			
			ss >> cur;
			if (search == false)continue;
			searchRes = wordSearch(root, cur, isTitle, type);
			if (searchRes)
				data = Unify(data, searchRes);
			//score = pos.size();
			else return false;
		}
		if (cur[0] == '~')searchRes = synonSearch(root, cur);
		else searchRes = wordSearch(root, cur, isTitle, type);
		if (cur[0] == '+') {
			if (!searchRes) return false;
			data = Unify(data, searchRes);
			//score = pos.size();
		}
		
		
		if (isNumber(cur[0])  || cur[0] == '$') {
			double lowbound, upbound;
			if (searchRange(cur, lowbound, upbound)) {
				string number=""; TrieNode* local=getNode();
				if (cur[0] == '$') {
					if (!root->children[38]) return false;
					numberSearch(root->children[38], number, lowbound, upbound, local);
					if (local->filePos.empty()) return false;
				}
				else numberSearch(root, number, lowbound, upbound, local);
				if (local->filePos.empty()) return false;
				data = Unify(data, local);
				
				continue;
			}
		}
		if (!searchRes) 
			search = false;
		
		else
		{
			data = Unify(data, searchRes);
			search = true;
		}
			//score = pos.size();
		
	}
	return true;
}
void Engine::numberSearch(TrieNode* root, string number, double lowbound, double upbound, TrieNode*& local)
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
		local = Unify(local, root);
		//score += root->order.size();
	}

	for (int i = 0; i <= 10; ++i)
	{
		if (i == 10)i = 37;
		if (root->children[i])
		{
			char add;
			if (i == 37)add = '.';
			else add = i + '0';
			numberSearch(root->children[i], number + add, lowbound, upbound, local);
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
	return true;
}