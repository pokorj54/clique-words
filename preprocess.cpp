#include <bits/stdc++.h>
#include "fast_hash_table.hpp"

using namespace std;

using vertex_t = int;
using adj_list_t = vector<vector<vertex_t>>;
using adj_matrix_t = vector<vector<bool>>;

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

adj_list_t construct_edges(const vector<string> & word_list ){
	adj_list_t edges(word_list.size());
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

adj_matrix_t adj_list_to_matrix(const adj_list_t & edges){
	adj_matrix_t matrix(edges.size(), vector<bool>(edges.size(), false));
	for(size_t i = 0; i < edges.size(); ++i){
		for(size_t j = 0; j < edges[i].size(); ++j){
			matrix[i][edges[i][j]] = true;
		}
	}
	return matrix;
}

bool neighbors_all(const adj_matrix_t & adj_matrix, vertex_t v, const vector<vertex_t> & acc){
	for(vertex_t u: acc){
		if(!adj_matrix[v][u]){
			return false;
		}
	}
	return true;
}

vector<vertex_t> intersect(const vector<vertex_t> & current, const vector<bool> & neighbors){
	vector<vertex_t> intersection;
	for(vertex_t c : current){
		if(neighbors[c]){
			intersection.push_back(c);
		}
	}
	return intersection;
}

int add_to_footprint(int old_footprint, const string & to_add){
	for(char c: to_add){
		old_footprint = old_footprint | (1 << (c-'a'));
	}
	return old_footprint;
}

bool find_clique(const adj_matrix_t & adj_matrix, const vector<vertex_t> & intersection_vertices, 
									size_t k, vector<vertex_t> & acc,  vector<vector<vertex_t>> & cliques,
					 				const vector<string> & word_list, int current_footprint, fast_map<int, vertex_t> & DP){
	vertex_t last = acc[acc.size()-1];
	if(DP.count(current_footprint) == 1 && DP[current_footprint] < last){
		return false;
	}
	if(k == acc.size()){
		cliques.push_back(acc);
		return true;
	}
	bool result = false;
	for(vertex_t v : intersection_vertices){
		if(last < v && neighbors_all(adj_matrix, v, acc)){
			acc.push_back(v);
			vector<vertex_t> new_intersection = intersect(intersection_vertices, adj_matrix[v]);
			int v_footprint = add_to_footprint(current_footprint, word_list[v]);
			result = find_clique(adj_matrix, new_intersection, k, acc, cliques, word_list, v_footprint, DP) || result;
			acc.pop_back();
		}
	}
	if(!result){
		if(DP.count(current_footprint) > 0){
			DP[current_footprint] = std::min(last, DP[current_footprint]);
		} else{
			DP[current_footprint] = last;
		}
	}
	return result;
}

void print_cliques(const vector<vector<vertex_t>> & cliques, const vector<string> & word_list){
	for(size_t i = 0; i < cliques.size(); ++i){
		for(size_t j = 0; j < cliques[i].size(); ++j){
			cout << word_list[cliques[i][j]] << " ";
		}
		cout << endl;
	}
}

int main(void){
	ios_base::sync_with_stdio(0); cin.tie(0);
	vector<string> word_list = read_word_list();
	cerr << "read input" << endl;
	adj_list_t edges = construct_edges(word_list);
	cerr << "constructed graph" << endl;
	adj_matrix_t adj_matrix = adj_list_to_matrix(edges);
	cerr << "adj matrix" << endl;
	vector<vector<vertex_t>> cliques;
	fast_map<int, vertex_t> DP;
	for(vertex_t v = 0; v < (vertex_t)edges.size(); ++v){
		vector<vertex_t> acc{v};
		cerr << v << endl;
		find_clique(adj_matrix, edges[v], 5, acc, cliques, word_list, add_to_footprint(0, word_list[v]), DP);
	}
	print_cliques(cliques, word_list);
	return 0;	
}
