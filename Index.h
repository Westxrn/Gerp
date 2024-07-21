#ifndef INDEX_H
#define INDEX_H

#include <vector>
#include <string>
#include "Hashmap.h"


using namespace std;

class Index {

public:
vector<string> filePaths;
vector<vector<string>> fileLines;
vector<string> search(string query, bool caseSens);
wordStruct createWordStruct(int filePathIndex, int lineNumIndex);
void addToSensMap(const string& key, const wordStruct& value);
void addToInsensMap(const string& key, const wordStruct& value); 

private:
Hashmap sensWordMap;
Hashmap insensWordMap; 
bool caseSens; 



};

#endif // INDEX_H


