#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

using std::cout, std::endl, std::cin, std::string, std::vector, std::unordered_map, std::unordered_set;

void prettyPrint(unordered_set<string>&);
void prettyPrint(vector<vector<string>>&);
void prettyPrint(unordered_map<string, unordered_set<string>>&);
void prettyPrint(unordered_map<string, int>&);
void getGrammar(unordered_set<string>&, unordered_set<string>&, vector<vector<string>>&);
void makeFIRST(unordered_set<string>&, unordered_set<string>&, vector<vector<string>>&, unordered_map<string, unordered_set<string>>&);
void makeFOLLOW(unordered_set<string>&, vector<vector<string>>&, unordered_map<string, unordered_set<string>>&, unordered_map<string, unordered_set<string>>&);
void makeTable(unordered_map<string, unordered_set<string>>&, unordered_map<string, unordered_set<string>>&, vector<vector<string>>&, unordered_set<string>&, unordered_set<string>&, vector<vector<string>>&, unordered_map<string, int>&, unordered_map<string, int>&);
bool strlow(string&);
void parse(vector<vector<string>>&, unordered_map<string, int>&, unordered_map<string, int>&);