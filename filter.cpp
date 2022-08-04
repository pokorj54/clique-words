#include <bits/stdc++.h>

using namespace std;

bool multiple_occurrences(const string & s){
    vector<bool> letters(26);
    for(char c : s){
        if (letters[c]){
            return true;
        }   
        letters[c] = true;
    }
    return false;
}

unordered_set<string> seen;
bool permutation_of_seen(string s){
    sort(s.begin(), s.end() );
    bool answer = seen.count(s);
    seen.insert(s);
    return answer; 
}

template <typename F>
void filter(F f){
    while(true){
        string word;
        cin >> word;
        if(!cin.eof()){
            if(f(word)){
                cout << word << endl;
            }
        }else{
            break;
        }
    }
}

int main(int argc, char ** argv){
    if(argc <= 1){
        cerr << "Run this program like " << argv[0] << " <words_lengths>" << endl;
        return 1;
    }
    size_t length = atoi(argv[1]);
	ios_base::sync_with_stdio(0); cin.tie(0);
    filter([&length](const string & s){return s.size() == length && !multiple_occurrences(s) && !permutation_of_seen(s); });
	return 0;	
}
