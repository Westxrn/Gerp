/* Index.cpp
* Written by: Weston Starbird and Elki Laranas
* Date: 4/29/24
* Purpose: Class definitions for the file Indexer class
*/

#include "Index.h"
#include <fstream>
#include <sstream>
#include <cctype> // For tolower
#include <set>
#include <unordered_set> 

using namespace std;


vector<string> Index::search(string query, bool caseSens) {
    unordered_set<string> uniqueResults;
    vector<string> results;
    // Handle case sensitivity
    if (caseSens) {
        if (sensWordMap.contains(query)) {
            // If the map contains the query
            vector<wordStruct> wordOccurrences = sensWordMap.at(query);

            for (const auto& occurrence : wordOccurrences) { 
                string filePath = filePaths[occurrence.filePathIndex];
                string line = fileLines[occurrence.filePathIndex][occurrence.lineNumIndex];
                string result = filePath + ":" + to_string(occurrence.lineNumIndex + 1) + ": " + line;
                if (uniqueResults.insert(result).second) {
                    results.push_back(result);
                }
            }
        } else {
            // If map doesn't contain query
            string result = query + " Not Found. Try with @insensitive or @i";
            results.push_back(result);
        }
    } else {
        string lowercaseQuery = query;
        for (char& c : lowercaseQuery) {
            c = tolower(c);
        }

        // Turn query lower and search for it in insens wordMap
        if (insensWordMap.contains(lowercaseQuery)) {
            vector<wordStruct> wordOccurrences = insensWordMap.at(lowercaseQuery);
            for (const auto& occurrence : wordOccurrences) {
                // Collect line in result string
                string filePath = filePaths[occurrence.filePathIndex];
                string line = fileLines[occurrence.filePathIndex][occurrence.lineNumIndex];
                string result = filePath + ":" + to_string(occurrence.lineNumIndex + 1) + ": " + line;
                // Insert the result into the set
                if (uniqueResults.insert(result).second) {
                    results.push_back(result);
                }
            }
        } else {
            string result = query + " Not Found.";
            results.push_back(result);
        }
    }
    return results;
}

wordStruct Index::createWordStruct(int filePathIndex, int lineNumIndex) {
    wordStruct newStruct; 
    // newStruct.word = word; 
    newStruct.filePathIndex = filePathIndex;
    newStruct.lineNumIndex = lineNumIndex; 
    return newStruct; 
}

void Index::addToSensMap(const string& key, const wordStruct& value) {
    if(sensWordMap.contains(key)) {
        sensWordMap.at(key).push_back(value);
    } else {
        sensWordMap.insert(key, value);
    }
}

void Index::addToInsensMap(const string& key, const wordStruct& value) {
    string lowercaseKey = key; 
    for(char& c : lowercaseKey) {
        c = tolower(c);
    }
    // convert key tolower inside the function
    // key must be lowercase word b/c this is insensitive map
    if(insensWordMap.contains(lowercaseKey)) {
        insensWordMap.at(lowercaseKey).push_back(value);
    } else {
        insensWordMap.insert(lowercaseKey, value); 
    }
}





