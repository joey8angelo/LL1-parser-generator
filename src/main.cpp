#include "headers.h"

int main() { 
    unordered_set<string> terminals;
    unordered_set<string> nonterminals;
    vector<vector<string>> productions;
    getGrammar(terminals, nonterminals, productions);
    
    cout << "terminals" << endl;
    prettyPrint(terminals);
    cout << endl;
    cout << "nonterminals" << endl;
    prettyPrint(nonterminals);
    cout << endl;

    unordered_map<string, unordered_set<string>> FIRST;
    FIRST["%"] = unordered_set<string>({"%"});
    makeFIRST(terminals, nonterminals, productions, FIRST);

    cout << "FIRST set" << endl;
    prettyPrint(FIRST);
    cout << endl;

    unordered_map<string, unordered_set<string>> FOLLOW;
    makeFOLLOW(nonterminals, productions, FIRST, FOLLOW);

    cout << "FOLLOW set" << endl;
    prettyPrint(FOLLOW);
    cout << endl;

    unordered_map<string, int> idTerm;
    unordered_map<string, int> idNonTerm;

    vector<vector<string>> table;
    makeTable(FIRST, FOLLOW, productions, terminals, nonterminals, table, idTerm, idNonTerm);
    terminals.clear();
    nonterminals.clear();
    FIRST.clear();
    FOLLOW.clear();
    productions.clear();

    cout << "Table" << endl;
    prettyPrint(table);
    cout << endl;

    parse(table, idTerm, idNonTerm);

}
// the problem is need to detect eof, >> will just keep spitting out the last thing in file
void parse(vector<vector<string>>& table, unordered_map<string, int>& idTerm, unordered_map<string, int>& idNonTerm){
    std::ifstream file;
    file.open("input.txt");

    if (!file.is_open())
        return;

    string input;

    file >> input;

    while (file) {
        vector<string> stack = {"$", "S"};
        while (stack[stack.size()-1] != "$") {

            if (stack[stack.size()-1] == input){
                stack.pop_back();
                if (!(file >> input))
                    input = "$";

            }
            else{
                string t = table[idNonTerm[stack[stack.size()-1]]][idTerm[input]];
                if (t == "-"){
                    cout << "SYNTAX ERROR on table[" << stack[stack.size()-1] << "][" << input << "]" << endl;
                    return;
                }
                stack.pop_back();
                vector<string> temp;
                string curr = "";
                for (int i = 0; i < t.size(); i++){
                    if (t[i] == ' '){
                        if (curr != "%")
                            temp.push_back(curr);
                        curr = "";
                    }
                    else
                        curr += t[i];
                }
                if (!curr.empty() && curr != "%")
                    temp.push_back(curr);

                for (int j = temp.size()-1; j >= 0; j--)
                    stack.push_back(temp[j]);
            }

            cout << "stack: ";
            for (int i = 0; i < stack.size(); i++){
                cout << " " << stack[i];
            }
            cout << endl;
            cout << " input: " << input << endl << endl;
        }
    }
}

void makeTable(unordered_map<string, unordered_set<string>>& FIRST, unordered_map<string, unordered_set<string>>& FOLLOW, vector<vector<string>>& productions, unordered_set<string>& terminals, unordered_set<string>& nonterminals, vector<vector<string>>& table, unordered_map<string, int>& idTerm, unordered_map<string, int>& idNonTerm){
    terminals.erase("%");
    terminals.insert("$");

    int id = 1;
    table.push_back({"-"});
    for (auto i = nonterminals.begin(); i != nonterminals.end(); i++) {
        idNonTerm[*i] = id++;
        table.push_back({*i});
    }


    id = 1;
    for (auto i = terminals.begin(); i != terminals.end(); i++){
        idTerm[*i] = id++;
        table[0].push_back(*i);
    }

    prettyPrint(idTerm);
    prettyPrint(idNonTerm);

    for (int i = 1; i < nonterminals.size()+1; i++){
        for (int j = 1; j < terminals.size()+1; j++){
            table[i].push_back("-");
        }
    }

    string rule;
    for (int i = 0; i < productions.size(); i++){
        bool cont = true;
        rule = "";
        unordered_set<string> curr;
        for (int j = 1; j < productions[i].size(); j++){
            string B = productions[i][j];
            if (j == 1)
                rule = B;
            else
                rule += " " + B;
            for (auto z = FIRST[B].begin(); z != FIRST[B].end() && cont; z++){
                if (*z != "%" || j == productions[i].size()-1)
                    curr.insert(*z);
            }
            if (FIRST[B].find("%") == FIRST[B].end())
                cont = false;
        }

        if (curr.find("%") != curr.end()){
            curr.erase("%");
            for (auto f = FOLLOW[productions[i][0]].begin(); f != FOLLOW[productions[i][0]].end(); f++){
                curr.insert(*f);
            }
        }

        string A = productions[i][0];
        for (auto a = curr.begin(); a != curr.end(); a++){
            if (table[idNonTerm[A]][idTerm[*a]] != "-"){
                cout << "ERROR, CONFLICTS IN TABLE[" << A << "][" << *a << "]" << endl;
                cout << "attempting to put rule \"" << productions[i][0] + " > " + rule << "\" when \"" << productions[i][0] + " > " + table[idNonTerm[A]][idTerm[*a]] << "\" already in table" << endl;
            }
            table[idNonTerm[A]][idTerm[*a]] = rule;
        }
    }
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

void prettyPrint(unordered_set<string>& set) {
    if (set.empty())
        return;
    cout << "{";
    cout << *set.begin();
    for (auto i = ++set.begin(); i != set.end(); i++) {
        cout << ", " << *i;
    }
    cout << "}" << endl;
}

void prettyPrint(unordered_map<string, int>& map){
    if (map.empty())
        return;
    cout << map.begin()->first << " : " << map.begin()->second;
    for (auto i = ++map.begin(); i != map.end(); i++){
        cout << ", " << i->first << " : " << i->second;
    }
    cout << endl;
}

void prettyPrint(unordered_map<string, unordered_set<string>>& map){
    if (map.empty())
        return;
    for (auto i = map.begin(); i != map.end(); i++){
        cout << i->first << ": ";
        prettyPrint(i->second);
    }
}

void prettyPrint(vector<vector<string>>& ls ) {
    if (ls.empty())
        return;
    cout << "[";
    cout << "[";
    if (ls.size() && ls[0].size())
        cout << ls[0][0];
    for (int j = 1; j < ls[0].size(); j++) {
        cout << "," << ls[0][j];
    }
    cout << "]";

    for (int i = 1; i < ls.size(); i++){
        cout << ",\n[";
        if (ls[i].size())
            cout << ls[i][0];
        for (int j = 1; j < ls[i].size(); j++) {
            cout << ", " << ls[i][j];
        }
        cout << "]";
    }

    cout << "]" << endl;

}; 

void getGrammar(unordered_set<string>& terminals, unordered_set<string>& nonterminals, vector<vector<string>>& productions) {
    //Input Grammar in form NONTERMINAL > (terminal |NONTERMINAL )* (space/case sensitive)
        // terminals can be non alphabetic nonterminals must be upper case
        // all terms must be seperated by a space S > terminal_ NONTERMINAL
        // Start symbol must be S
 
    string input = "";
    string lhs = "";
    string rhs = "";
    
    std::ifstream file;

    file.open("grammar.txt");

    if (!file.is_open())
        return;

    std::getline(file, input);

    while (file) {
        string curr = "";
        for (int i = 0; i < input.size(); i++) {
            if (input[i] != ' ')
                curr += input[i];
            if ((input[i] == ' ' || i == input.size()-1) && curr != "") {
                if (lhs == "") {
                    i += 1;
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