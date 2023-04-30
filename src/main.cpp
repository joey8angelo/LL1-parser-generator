#include "headers.h"

int main() {
    unordered_set<string> terminals;
    unordered_set<string> nonterminals;
    vector<vector<string>> productions;
    getGrammar(terminals, nonterminals, productions);
    
    cout << "terminals ";
    prettyPrint(terminals);
    cout << "nonterminals ";
    prettyPrint(nonterminals);
    cout << "productions ";
    prettyPrint(productions);

    unordered_map<string, unordered_set<string>> FIRST;
    FIRST["%"] = unordered_set<string>({"%"});
    makeFIRST(terminals, nonterminals, productions, FIRST);

    unordered_map<string, unordered_set<string>> FOLLOW;
    makeFOLLOW(nonterminals, productions, FIRST, FOLLOW);

    unordered_map<string, unordered_set<string>> FIRSTPLUS;
    makeFIRSTPLUS(FIRST, FOLLOW, FIRSTPLUS);

    cout << "FIRST set ";
    prettyPrint(FIRST);

    cout << "FOLLOW set ";
    prettyPrint(FOLLOW);

    cout << "FIRSTPLUS set ";
    prettyPrint(FIRSTPLUS);
}

void makeFIRST(unordered_set<string>& terminals, unordered_set<string>& nonterminals, vector<vector<string>>& productions, unordered_map<string, unordered_set<string>>& FIRST){

    for (auto a = terminals.begin(); a != terminals.end(); a++){
        if(FIRST.find(*a) == FIRST.end()){
            FIRST[*a] = unordered_set<string>({*a});
        }
    }
    for (auto A = nonterminals.begin(); A != nonterminals.end(); A++){
        FIRST[*A] = unordered_set<string>();
    }

    int count = 0;
    int prev = -1;

    while (count != prev) {
        prev = count;

        for (int i = 0; i < productions.size(); i++){
            bool cont = true;
            unordered_set<string> rhs;
            for (int j = 1; j < productions[i].size() && cont; j++){
                string B = productions[i][j];
                for (auto n = FIRST[B].begin(); n != FIRST[B].end(); n++){
                    if (*n != "%" || j == productions[i].size()-1)
                        rhs.insert(*n);
                }
                if (FIRST[B].find("%") == FIRST[B].end())
                    cont = false;
            }
            for (auto r = rhs.begin(); r != rhs.end(); r++){
                if (FIRST[productions[i][0]].find(*r) == FIRST[productions[i][0]].end())
                    count++;
                FIRST[productions[i][0]].insert(*r);
            }
            rhs.clear();
        }
    }
}

void makeFOLLOW(unordered_set<string>& nonterminals, vector<vector<string>>& productions, unordered_map<string, unordered_set<string>>& FIRST, unordered_map<string, unordered_set<string>>& FOLLOW) {
    for (auto A = nonterminals.begin(); A != nonterminals.end(); A++){
        FOLLOW[*A] = unordered_set<string>();
        if (*A == "S")
            FOLLOW[*A].insert("$");
    }
    int count = 0;
    int prev = -1;

    while (count != prev) {
        prev = count;

        for (int i = 0; i < productions.size(); i++) {
            auto trailer = FOLLOW[productions[i][0]];
            for (int j = productions[i].size()-1; j >= 1; j--) {
                string B = productions[i][j];
                if (nonterminals.find(B) != nonterminals.end()){
                    for (auto t = trailer.begin(); t != trailer.end(); t++){ // FOLLOW(B) = FOLLOW(B) U trailer
                        if (FOLLOW[B].find(*t) == FOLLOW[B].end())
                            count++;
                        FOLLOW[B].insert(*t);
                    }
                    if (FIRST[B].find("%") != FIRST[B].end()){ // if epsilon is in FIRST[B]
                        for (auto b = FIRST[B].begin(); b != FIRST[B].end(); b++) { // trailer = trailer U (FIRST[B] - {%})
                            if (*b != "%")
                                trailer.insert(*b);
                        }
                    }
                    else
                        trailer = FIRST[B];
                }
                else 
                    trailer = FIRST[B];
            }
        }
    }
}

void makeFIRSTPLUS(unordered_map<string, unordered_set<string>>& FIRST, unordered_map<string, unordered_set<string>>& FOLLOW, unordered_map<string, unordered_set<string>>& FIRSTPLUS) {
    for (auto i = FIRST.begin(); i != FIRST.end(); i++){
        if (i->first == "%")
            continue;
        FIRSTPLUS[i->first] = i->second;
        if (i->second.find("%") != i->second.end()){
            FIRSTPLUS[i->first].erase("%");
            for (auto j = FOLLOW[i->first].begin(); j != FOLLOW[i->first].end(); j++)
                FIRSTPLUS[i->first].insert(*j);
        }
    }
}

template <typename T>
void prettyPrint(unordered_set<T>& set) {
    cout << "{ ";
    for (auto i = set.begin(); i != set.end(); i++) {
        cout << *i << ", ";
    }
    cout << "}" << endl;
}

void prettyPrint(unordered_map<string, unordered_set<string>>& map){
    cout << "{ ";

    for (auto i = map.begin(); i != map.end(); i++){
        cout << i->first << ": ";
        prettyPrint(i->second);
    }
    cout << "}" << endl;
}

void prettyPrint(vector<vector<string>>& ls ) {
    cout << "[ ";

    for (int i = 0; i < ls.size(); i++){
        cout << "[ ";
        for (int j = 0; j < ls[i].size(); j++) {
            cout << ls[i][j] << ", ";
        }
        cout << "], " << endl;
    }

    cout << "]" << endl;

}; 

void getGrammar(unordered_set<string>& terminals, unordered_set<string>& nonterminals, vector<vector<string>>& productions) {
    cout << "Input grammar in form NONTERMINAL > (terminal |NONTERMINAL )* (space/case sensitive)" << endl;
    cout << "Start symbol must be S" << endl;
    cout << "Type \"end\" to exit input, enter though input.txt" << endl;
 
    string input = "";
    string lhs = "";
    string rhs = "";
    
    std::ifstream file;

    file.open("input.txt");

    if (!file.is_open())
        return;

    std::getline(file, input);

    while (input != "end" && file) {
        string curr = "";
        for (int i = 0; i < input.size(); i++) {
            if (input[i] != '>' && input[i] != ' ')
                curr += input[i];
            if ((input[i] == ' ' || i == input.size()-1) && curr != "") {
                if (lhs == "") {
                    lhs = curr;
                    nonterminals.insert(curr);
                    productions.push_back({lhs});
                }
                else if (strlow(curr)) {
                    terminals.insert(curr);
                    productions[productions.size()-1].push_back(curr);
                }
                else {
                    nonterminals.insert(curr);
                    productions[productions.size()-1].push_back(curr);
                }

                curr = "";
            }
        }
        lhs = "";
        curr = "";

        std::getline(file, input);
    }

    file.close();
}

bool strlow(string& input){
    for (int i = 0; i < input.size(); i++) {
        if (!isalpha(input[i]))
            continue;
        if (!islower(input[i])){
            return false;
        }
    }
    return true;
}