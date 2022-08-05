#include <bits/stdc++.h>
#include "fast_hash_table.hpp"

using namespace std;

using vertex_t = int;
using adj_list_t = vector<vector<vertex_t>>;
using adj_matrix_t = vector<vector<bool>>;

/*
 * Checks whether a and b share letters.
 */
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

adj_list_t construct_adj_list(const vector<string> & word_list ){
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

/*
 * Footprint is a set representation of letters by single integer. 
 * For the first 26 bits a letter in the english alphabet is assigned.
 * If the bit is set to 1 it means the letter is in the set.
 */
int create_footprint(const string & word){
	int footprint = 0;
	for(char c: word){
		footprint = footprint | (1 << (c-'a'));
	}
	return footprint;
}

vector<int> word_list_footprints(const vector<string> & word_list){
	vector<int> footprints;
	for(const string & word: word_list){
		footprints.push_back(create_footprint(word));
	}
	return footprints;
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

/*
 * Intersects set of current vertices that could be in the clique with the neighbors of the new vertex.
 */
vector<vertex_t> intersect(const vector<vertex_t> & current, const vector<bool> & neighbors){
	vector<vertex_t> intersection;
	for(vertex_t c : current){
		if(neighbors[c]){
			intersection.push_back(c);
		}
	}
	return intersection;
}

/*
 * Finds the cliques in recursive manner.
 * Current clique is in the @acc, which is maintained in ascending order.
 * The vertices in @acc are a clique, because @intersection_vertices is the intersection of neighbors of the clique.
 */
bool find_clique(const adj_matrix_t & adj_matrix, const vector<vertex_t> & intersection_vertices, 
									size_t k, vector<vertex_t> & acc,  vector<vector<vertex_t>> & cliques,
					 				const vector<int> & footprints, int current_footprint, fast_map<int, vertex_t> & DP){
	vertex_t last = acc[acc.size()-1];
	// in DP the index of smallest vertex that failed with given footprint is stored.
	// if we encounter vertex with higher label we know, that in this branch no solution can be found.
	if(DP.count(current_footprint) == 1 && DP[current_footprint] < last){
		return false;
	}
	if(k == acc.size()){
		cliques.push_back(acc);
		return true;
	}
	bool result = false;
	for(vertex_t v : intersection_vertices){
		if(last < v){
			acc.push_back(v);
			vector<vertex_t> new_intersection = intersect(intersection_vertices, adj_matrix[v]);
			int v_footprint = current_footprint | footprints[v];
			result = find_clique(adj_matrix, new_intersection, k, acc, cliques, footprints, v_footprint, DP) || result;
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


class ProgressBar{
	const string unit;
	string progress_bar = "[]";
	const int progress_bar_width = 50;
	int curr_progress_width = 0;
	int goal;

	public:
	ProgressBar(int goal, const string & unit ): unit(unit), goal(goal){
		for(int i = 0; i < progress_bar_width; ++i){
			progress_bar.insert(1, 1, '-');
		}
	}

	void update(int progress){
		double percentage = (progress * 100.0)/goal;
		if(percentage > (double)curr_progress_width*100.0/progress_bar_width){
			progress_bar[curr_progress_width + 1] = '#';
			++curr_progress_width;
		}

		cerr << "\r [" << progress << "/" << goal << " " << unit << "] [" << static_cast<int>(percentage) << '%' << "] " << progress_bar << std::flush;
	}

	void finish(){
		update(goal);
		cerr << endl;
	}
};

int main(int argc, char ** argv){
    if(argc <= 1){
        cerr << "Run this program like " << argv[0] << " <clique_size>" << endl;
        return 1;
    }
	int clique_size = atoi(argv[1]);
	ios_base::sync_with_stdio(0); cin.tie(0);

	cerr << "Reading input" << endl;
	vector<string> word_list = read_word_list();
	vector<int> footprints = word_list_footprints(word_list);
	cerr << "Constructing graph" << endl;
	adj_list_t edges = construct_adj_list(word_list);
	cerr << "Constructing adjacency matrix" << endl;
	adj_matrix_t adj_matrix = adj_list_to_matrix(edges);
	
	cerr << "Finding cliques" << endl;
	vector<vector<vertex_t>> cliques;
	fast_map<int, vertex_t> DP;
    size_t num_words = word_list.size();
	ProgressBar PB(num_words, "words");

	for(vertex_t v = 0; v < (vertex_t)num_words; ++v){
		vector<vertex_t> acc{v};
		PB.update(v);
		find_clique(adj_matrix, edges[v], clique_size, acc, cliques, footprints, footprints[v], DP);
	}
	PB.finish();

	print_cliques(cliques, word_list);
	return 0;	
}
