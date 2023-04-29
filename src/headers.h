#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using std::cout, std::endl, std::cin, std::string, std::vector;

template <typename T>
void prettyPrint(std::unordered_set<T>&);
void prettyPrint(std::unordered_map<string, vector<vector<string>>>&);
void prettyPrint(std::unordered_map<string, std::unordered_set<string>>&);
void getGrammar(std::unordered_set<string>&, std::unordered_set<string>&, std::unordered_map<string, vector<vector<string>>>&);
void makeFIRST(std::unordered_set<string>&, std::unordered_set<string>&, std::unordered_map<string, vector<vector<string>>>&, std::unordered_map<string, std::unordered_set<string>>&);
bool strlow(string&);