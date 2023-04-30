#include "headers.h"

int main() {
    std::unordered_set<string> terminals;
    std::unordered_set<string> nonterminals;
    vector<vector<string>> productions;
    getGrammar(terminals, nonterminals, productions);
    
    cout << "terminals ";
    prettyPrint(terminals);
    cout << "nonterminals ";
    prettyPrint(nonterminals);
    cout << "productions ";
    prettyPrint(productions);

    std::unordered_map<string, std::unordered_set<string>> FIRST;
    FIRST["%"] = std::unordered_set<string>({"%"});
    makeFIRST(terminals, nonterminals, productions, FIRST);

    std::unordered_map<string, std::unordered_set<string>> FOLLOW;
    makeFOLLOW(nonterminals, productions, FIRST, FOLLOW);

    cout << "FIRST set ";
    prettyPrint(FIRST);

    cout << "FOLLOW set ";
    prettyPrint(FOLLOW);
}

void makeFIRST(std::unordered_set<string>& terminals, std::unordered_set<string>& nonterminals, vector<vector<string>>& productions, std::unordered_map<string, std::unordered_set<string>>& FIRST){

    for (auto a = terminals.begin(); a != terminals.end(); a++){
        if(FIRST.find(*a) == FIRST.end()){
            FIRST[*a] = std::unordered_set<string>({*a});
        }
    }
    for (auto A = nonterminals.begin(); A != nonterminals.end(); A++){
        FIRST[*A] = std::unordered_set<string>();
    }

    int count = 0;
    int prev = -1;

    while (count != prev) {
        prev = count;

        for (int i = 0; i < productions.size(); i++){
            bool cont = true;
            std::unordered_set<string> rhs;
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

void makeFOLLOW(std::unordered_set<string>& nonterminals, vector<vector<string>>& productions, std::unordered_map<string, std::unordered_set<string>>& FIRST, std::unordered_map<string, std::unordered_set<string>>& FOLLOW) {
    for (auto A = nonterminals.begin(); A != nonterminals.end(); A++){
        FOLLOW[*A] = std::unordered_set<string>();
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

void makeFIRSTPLUS() {}

template <typename T>
void prettyPrint(std::unordered_set<T>& set) {
    cout << "{ ";
    for (auto i = set.begin(); i != set.end(); i++) {
        cout << *i << ", ";
    }
    cout << "}" << endl;
}

void prettyPrint(std::unordered_map<string, std::unordered_set<string>>& map){
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

void getGrammar(std::unordered_set<string>& terminals, std::unordered_set<string>& nonterminals, vector<vector<string>>& productions) {
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