# Clique words solver

Solver for finding set of words with unique letters.
The solver is inspired by Matt Parker's (https://www.youtube.com/watch?v=_-AfhLQfb6w)[video] about five five-letter words that have 25 unique letters. He said it took him about a month, which for Benjamin Paassen appeared as optimizable and he created faster (https://gitlab.com/bpaassen/five_clique)[algorithm] that runs about 20 minutes. This solver needs less than a minute and it also can do other lenghts of words other than 5.

## How to run
1. Get your favorite list of words e.g. `words_alpha.txt` from (https://github.com/dwyl/english-words)[this] repository.
2. Compile the programs using `make`.
3. Filter the words to the length you want `./bin/filter <words_length> < <file_with_all_words>`. The filtered words will be printed to `stdout`. To replicate Matt Parker's settings use `./bin/filter 5 < words_alpha.txt`.
4. Run the solver with wanted number of words in the solution `./bin/solver <number_of_words> < <filtered_words>`. The result will appear on `stdout` and progress information on `stderr`. Most likely, you would want to set the number of words as `floor(26/<words_length>)`.


## Algorithm
The problem is reformulated as a graph problem of finding all the cliques of wanted size. 
The clique is searched for in recursive way, by remembering vector of vertices that could be part of the clique. 
To avoid repetition, the vertices inside the vector are maintained in sorted order.
There is a second vector of vertices which is the intersection of neighbors of vertices in the partial clique.

A small time improvement were achieved by using dynamic programming. 
For current vertices in the clique, set of corresponding letters is calculated.
If for given partial clique no solution is found, then the letter "footprint" is stored along with the index of last added vertex.
So, when the footprint is seen for the second time with larger last vertex index, we know, that no solution can be found in this branch.

For the data structures, apart from adjacency list and adjacency matrix, fast hash table from (https://github.com/martinus/robin-hood-hashing)[martinus] is used for the DP table.

## Performance
The algorithm was tested on IntelCore i7-8550U with 16 GB RAM using `words_alpha.txt` filtered to strings of length 5 as a dataset.
With added compilation flag `-march=haswell` the algorithm managed to find all cliques of size 5 in `30,279` seconds.
Without it, it finished in `40,047` seconds.

