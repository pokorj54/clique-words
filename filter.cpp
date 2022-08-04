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

int main(void){
	ios_base::sync_with_stdio(0); cin.tie(0);
    filter([](const string & s){return s.size() == 5 && !multiple_occurrences(s);});
	return 0;	
}
