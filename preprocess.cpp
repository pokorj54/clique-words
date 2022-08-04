#include <bits/stdc++.h>
#include "fast_hash_table.hpp"

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

template <typename T>
size_t get_total_size(const vector<T> & objects){
	size_t sum = 0;
	for(size_t i = 0; i < objects.size(); ++i){
		sum += objects[i].size();
	}
	return sum;
}

void print_METIS(const vector<vector<size_t>> & edges){
	size_t total_edges = get_total_size(edges)/2;
	cout << edges.size() << " " << total_edges << " " << 1 << endl;
	for(size_t i = 0; i < edges.size(); ++i){
		if(edges[i].size() > 0){
			cout << edges[i][0];
		}
		for(size_t j = 1; j < edges[i].size(); ++j){
			cout << " " << 1+edges[i][j];
		}
		cout << endl;
	}
}

void print_custom(const vector<vector<size_t>> & edges){
	size_t total_edges = get_total_size(edges);
	cout << edges.size() << endl << total_edges << endl;
	for(size_t i = 0; i < edges.size(); ++i){
		for(size_t j = 0; j < edges[i].size(); ++j){
			cout << i << "," << edges[i][j] << endl;
		}
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


vector<fast_set<size_t>> transform(const vector<vector<size_t>> & edges){
	vector<fast_set<size_t>> result;
	for(const vector<size_t> & v: edges){
		result.emplace_back(v.begin(), v.end());
	}
	return result;
}

vector<vector<size_t>> retransform(const vector<fast_set<size_t>> & edges){
	vector<vector<size_t>> result;
	for(const fast_set<size_t> & u: edges){
		result.emplace_back(u.begin(), u.end());
	}
	return result;
}

bool neighborhood_intersection_at_least(const vector<fast_set<size_t>> & edges, size_t a, size_t b, size_t k){
	if(edges[a].size() > edges[b].size()){
		return neighborhood_intersection_at_least(edges, b, a, k);
	}
	size_t intersection = 0;
	for(size_t n: edges[a]){
		if(edges[b].count(n) == 1){
			intersection += 1;
			if(intersection >= k){
				return true;
			}
		}
	}
	return false;
}

vector<vector<size_t>> remove_redundant_edges(const vector<vector<size_t>> & edges){
	int count = 0;
	int prev_count = -1;
	vector<fast_set<size_t>> transformed = transform(edges);
	while(count != prev_count){
		prev_count = count;
		for(size_t i= 0; i <transformed.size(); ++i){
			fast_set<size_t> replacement;
			for(size_t j : transformed[i]){
				if(!neighborhood_intersection_at_least(transformed, i, j,3)){
					count += 1;
					transformed[j].erase(i);
				} else{
					replacement.insert(j);
				}
			}
			transformed[i] = replacement;
		}
		cerr << count << endl;
	}
	cerr << "pruning done" << endl;
	return retransform(transformed);
}

int main(void){
	ios_base::sync_with_stdio(0); cin.tie(0);
	vector<string> word_list = read_word_list();
	vector<vector<size_t>> edges = construct_edges(word_list);
	vector<vector<size_t>> pruned = remove_redundant_edges(edges);
	print_custom(pruned);
	return 0;	
}
