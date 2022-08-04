#include <bits/stdc++.h>

using ll = long long int;
using ld = long double;

#define fr(i, x) for(ll i = 0; i < (x); ++i)

using namespace std;


bool compatible(const string & a, const string &b){
	for(char c : a){
		for(char d : b){
			if (c == d){
				return false;
			}
		}
	}
	return true;
}

vector<vector<size_t>> construct_edges(const vector<string> & word_list ){
	vector<vector<size_t>> edges(word_list.size());
	for(size_t i = 0; i < word_list.size(); ++i){
		for(size_t j = i; j < word_list.size(); ++j){
			if(compatible(word_list[i], word_list[j])){
				edges[i].push_back(j);
				edges[j].push_back(i);
			}
		}
	}
	return edges;
}

void print_METIS(const vector<vector<size_t>> & edges){
	size_t sum = 0;
	for(size_t i = 0; i < edges.size(); ++i){
		sum += edges[i].size();
	}
	cout << edges.size() << " " << sum << 1 << endl;
	for(size_t i = 0; i < edges.size(); ++i){
		if(edges[i].size() > 0){
			cout << edges[i][0];
		}
		for(size_t j = 1; j < edges[i].size(); ++j){
			cout << " " << edges[i][j];
		}
		cout << endl;
	}
}

vector<string> read_word_list(){
	vector<string> word_list;
	while(true){
		string word;
		cin >> word;
		if(!cin.eof()){
			word_list.push_back(word);
		} else{
			break;
		}
	}
	return word_list;
}


int main(void){
	ios_base::sync_with_stdio(0); cin.tie(0);
	vector<string> word_list = read_word_list();
	vector<vector<size_t>> edges = construct_edges(word_list);
	print_METIS(edges);
	return 0;	
}
