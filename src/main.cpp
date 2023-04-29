#include "headers.h"

int main() {
    std::unordered_set<string> terminals;
    std::unordered_set<string> nonterminals;
    std::unordered_map<string, vector<vector<string>>> productions; // FIX: PRODUCTIONS DOES NOT NEED TO BE A MAP : JUST A VECTOR<VECTOR<STRING>>
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

void makeFIRST(std::unordered_set<string>& terminals, std::unordered_set<string>& nonterminals, std::unordered_map<string, vector<vector<string>>>& productions, std::unordered_map<string, std::unordered_set<string>>& FIRST){

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
        for (auto p = productions.begin(); p != productions.end(); p++) {
            std::unordered_set<string> rhs;
            for (int i = 0; i < p->second.size(); i++){//iterate over all production rules for p
                bool cont = true;
                for (int j = 0; j < p->second[i].size() && cont; j++){// iterate over production rule
                    string B = p->second[i][j];
                    if (FIRST[B].find("%") == FIRST[B].end())
                        cont = false;
                    for (auto z = FIRST[B].begin(); z != FIRST[B].end(); z++){//add each FIRST[B] to rhs
                        if (*z != "%" || j == p->second[i].size()-1){
                            rhs.insert(*z);
                        }
                    }
                }
            }
            for (auto i = rhs.begin(); i != rhs.end(); i++){
                if (FIRST[p->first].find(*i) == FIRST[p->first].end())
                    count++;
                FIRST[p->first].insert(*i);
            }
            rhs.clear();
        }
    }
}

void makeFOLLOW(std::unordered_set<string>& nonterminals, std::unordered_map<string, vector<vector<string>>>& productions, std::unordered_map<string, std::unordered_set<string>>& FIRST, std::unordered_map<string, std::unordered_set<string>>& FOLLOW) {
    for (auto A = nonterminals.begin(); A != nonterminals.end(); A++){
        FOLLOW[*A] = std::unordered_set<string>();
        if (*A == "S")
            FOLLOW[*A].insert("$");
    }
    int count = 0;
    int prev = -1;

    while (count != prev) {
        prev = count;

        for (auto p = productions.begin(); p != productions.end(); p++) {
            for (int i = 0; i < p->second.size(); i++) {
                auto trailer = FOLLOW[p->first];
                for (int j = p->second[i].size()-1; j >= 0; j--) {
                    string B = p->second[i][j];
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

}

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

void prettyPrint(std::unordered_map<string, vector<vector<string>>>& map ) {
    cout << "{ ";

    for (auto i = map.begin(); i != map.end(); i++){
        cout << i->first << ": [";
        for (auto j = i->second.begin(); j != i->second.end(); j++){
            cout << "[ ";
            for (int z = 0; z < j->size(); z++) {
                cout << j->at(z) << ", ";
            }
            cout << "]";
        }
        cout << "]" << endl;
    }

    cout << "}" << endl;

}; 

void getGrammar(std::unordered_set<string>& terminals, std::unordered_set<string>& nonterminals, std::unordered_map<string, vector<vector<string>>>& productions) {
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
                    if (productions.find(lhs) == productions.end())
                        productions[lhs] = vector<vector<string>> {{}};
                    else
                        productions[lhs].push_back({});
                }
                else if (strlow(curr)) {
                    terminals.insert(curr);
                    productions[lhs][productions[lhs].size()-1].push_back(curr);
                }
                else {
                    nonterminals.insert(curr);
                    productions[lhs][productions[lhs].size()-1].push_back(curr);
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