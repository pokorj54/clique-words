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


void print_edge_per_line(const vector<vector<size_t>> & edges, char separator){
	for(size_t i = 0; i < edges.size(); ++i){
		for(size_t j = 0; j < edges[i].size(); ++j){
			cout << i << separator << edges[i][j] << endl;
		}
	}
}

void print_quick_clique_format(const vector<vector<size_t>> & edges){
	size_t total_edges = get_total_size(edges);
	cout << edges.size() << endl << total_edges << endl;
	print_edge_per_line(edges, ',');
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

vector<vector<bool>> get_adj_matrix(const vector<vector<size_t>> & edges){
	vector<vector<bool>> matrix(edges.size(), vector<bool>(edges.size(), false));
	for(size_t i = 0; i < edges.size(); ++i){
		for(size_t j = 0; j < edges[i].size(); ++j){
			matrix[i][edges[i][j]] =true;
		}
	}
	return matrix;
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

void remove_redundant_edges(vector<fast_set<size_t>> & edges){
	int count = 0;
	int prev_count = -1;
	while(count != prev_count){
		prev_count = count;
		for(size_t i= 0; i <edges.size(); ++i){
			fast_set<size_t> replacement;
			for(size_t j : edges[i]){
				if(!neighborhood_intersection_at_least(edges, i, j,3)){
					count += 1;
					edges[j].erase(i);
				} else{
					replacement.insert(j);
				}
			}
			edges[i] = replacement;
		}
		cerr << count << endl;
	}
	cerr << "pruning done" << endl;
}

bool neighbors_all(const vector<vector<bool>> & adj_matrix, size_t v, const vector<size_t> & acc){
	for(size_t u: acc){
		if(!adj_matrix[v][u]){
			return false;
		}
	}
	return true;
}

vector<size_t> intersect(const vector<size_t> & current, const vector<bool> & neighbours){
	vector<size_t> intersection;
	for(size_t c : current){
		if(neighbours[c]){
			intersection.push_back(c);
		}
	}
	return intersection;
}

void find_clique(const vector<vector<bool>> & adj_matrix, const vector<size_t> &  intersection_vertices, size_t k, vector<size_t> & acc, 
					vector<vector<size_t>> & cliques){
	if(k == acc.size()){
		cliques.push_back(acc);
		return;
	}
	size_t last = acc[acc.size()-1];
	for(size_t v : intersection_vertices){
		if(last < v && neighbors_all(adj_matrix, v, acc)){
			acc.push_back(v);
			vector<size_t> new_intersection = intersect(intersection_vertices, adj_matrix[v]);
			find_clique(adj_matrix, new_intersection, k, acc, cliques);
			acc.pop_back();
		}
	}
}

void print_cliques(const vector<vector<size_t>> & cliques, const vector<string> & word_list){
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
	vector<vector<size_t>> edges = construct_edges(word_list);
	cerr << "constructed graph" << endl;
	vector<vector<bool>> adj_matrix = get_adj_matrix(edges);
	cerr << "adj matrix" << endl;
	vector<vector<size_t>> cliques;
	for(size_t v = 0; v < edges.size(); ++v){
		vector<size_t> acc{v};
		cerr << v << endl;
		find_clique(adj_matrix, edges[v], 5, acc, cliques);
	}
	print_cliques(cliques, word_list);
	return 0;	
}
