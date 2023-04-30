#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <fstream>

using std::cout, std::endl, std::cin, std::string, std::vector, std::unordered_map, std::unordered_set;

template <typename T>
void prettyPrint(unordered_set<T>&);
void prettyPrint(vector<vector<string>>&);
void prettyPrint(unordered_map<string, unordered_set<string>>&);
void getGrammar(unordered_set<string>&, unordered_set<string>&, vector<vector<string>>&);
void makeFIRST(unordered_set<string>&, unordered_set<string>&, vector<vector<string>>&, unordered_map<string, unordered_set<string>>&);
void makeFOLLOW(unordered_set<string>&, vector<vector<string>>&, unordered_map<string, unordered_set<string>>&, unordered_map<string, unordered_set<string>>&);
void makeFIRSTPLUS(unordered_map<string, unordered_set<string>>& FIRST, unordered_map<string, unordered_set<string>>& FOLLOW, std::unordered_map<string, unordered_set<string>>& FIRSTPLUS);
bool strlow(string&);