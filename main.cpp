/*
for each a in (T U $ U epsilon):
    FIRST(a) = a

for each A in NT:
    FIRST(A) = (empty set)

while any first set changes:
    for each p in P where p is form A->B:
        if B is B_1 B_2...B_k where B_i in T U NT:
            rhs = FIRST(B_i) - {epsilon}
            i += 1
    if i == k and epsilon in FIRST(B_k):
        rhs = rhs U {epsilon}
    FIRST(A) = FIRST(A) U rhs
*/

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using std::cout, std::endl, std::cin, std::string;

template <typename T>
void prettyPrint(std::unordered_set<T>&);
void prettyPrint(std::unordered_map<string, std::vector<string>>&);

void getGrammar(std::unordered_set<string>&, std::unordered_set<char>&, std::unordered_map<string, std::vector<string>>&);

int main() {
    std::unordered_set<string> terminals;
    std::unordered_set<char> nonterminals;
    std::unordered_map<string, std::vector<string>> productions;
    getGrammar(terminals, nonterminals, productions);
    prettyPrint(terminals);
    prettyPrint(nonterminals);
    prettyPrint(productions);
}
template <typename T>
void prettyPrint(std::unordered_set<T>& set) {
    cout << "[ ";
    for (auto i = set.begin(); i != set.end(); i++) {
        cout << *i << ", ";
    }
    cout << "]" << endl;
}

void prettyPrint(std::unordered_map<string, std::vector<string>>& map ) {
    cout << "{ ";

    for (auto i = map.begin(); i != map.end(); i++){
        cout << i->first << ": [ ";
        for (int j = 0; j < i->second.size(); j++){
            cout << i->second[j] << ", ";
        }
        cout << "]" << endl;
    }

    cout << "}" << endl;

}

void getGrammar(std::unordered_set<string>& terminals, std::unordered_set<char>& nonterminals, std::unordered_map<string, std::vector<string>>& productions) {
    cout << "Input grammar in form TERMINAL > (TERMINAL |nonterminal )* (space/case sensitive)" << endl;
    cout << "type \"end\" to exit input" << endl;
 
    std::string input = "";
    std::getline(cin, input);

    while (input != "end") {
        string curr = "";
        string terminal = "";

        for (int i = 0; i < input.size(); i++) {
            if (input[i] == '>') {
                terminal = curr;
                terminals.insert(terminal);
                curr = "";
                i += 1;
            }
            if (input[i] != ' ' && input[i] != '>') {
                if (!isalpha(input[i]) || islower(input[i])){
                    nonterminals.insert(input[i]);
                }
            }
            if (terminal == ""){
                if(input[i] != ' '){
                    curr += input[i];
                }
            }
            else {
                curr += input[i];
            }
        }
        productions[terminal].push_back(curr);

        std::getline(cin, input);
    }
}

/*
EXPR > EXPR + TERM 
EXPR > EXPR - TERM 
TERM > TERM * FACTOR
TERM > TERM / FACTOR
FACTOR > ( EXPR )
FACTOR > %
FACTOR > num
FACTOR > ident

productions
{"EXPR": ["EXPR + TERM", "EXPR - TERM"], 
 "TERM": ["TERM * FACTOR", "TERM / FACTOR"], 
 "FACTOR": ["( EXPR )", "%"]}

terminals
{"EXPR", "TERM, "FACTOR"}

nonterminals
{+, -, (, ), %}
*/