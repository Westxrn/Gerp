/* Gerp.cpp
* Written by: Weston Starbird and Elki Laranas
* Date: 4/29/24
* Purpose: The main class of the Gerp program. Handles
*          the input and output of the program through
*          the command line.
*/

#include "Gerp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype> // For isalnum, tolower

using namespace std;

Gerp::Gerp(string inputDir, string outputFile) {
    traverseDirectory(inputDir);
    output.open(outputFile);
    // output already declared as ofstrem object
    if (not output.is_open()) {
        throw runtime_error("Unable to open file " + outputFile);
    }
}

Gerp::~Gerp() { output.close(); }


void Gerp::runQueryLoop() {
    string query;
    cout << "Query? ";
    while (cin >> query and query != "@q" and query != "@quit") {
        if (query == "@i" or query == "@insensitive") {
            cin >> query;
            printToOutput(stripNonAlphaNum(query), false);
        } else if (query == "@f") {
            cin >> query;
            output.close();
            output.open(query);      
        } else {
            printToOutput(stripNonAlphaNum(query), true);
        }
        cout << "Query? ";
    }
    cout << "Goodbye! Thank you and have a nice day." << endl;
}


/*
takes in query and bool, query representing word to search for, bool being caseSensitivity
*/
void Gerp::printToOutput(string query, bool caseSensitive) {
    vector<string> results = indexer.search(query, caseSensitive);
    // cout << results.size() << endl;
    for (string result : results) {
        output << result << endl;
    }
}

/*
buildIndex should be called on each file in the directory, 
fills in both insensMap and sensMap given file paths
*/
void Gerp::buildIndex(const string& filePath) {
    ifstream file(filePath); 
    // open file as ifstream object
    if(!file) {
        cerr << "Error opening file: " << filePath << endl;
        return; 
        // error message if a file can't be opened for some reason
    }

    int filePathIndex = indexer.filePaths.size();
    // if nothing in it, index = 0 as it should, shouldn't be buggy
    indexer.filePaths.push_back(filePath); 
    // push back the current filePath into filePaths
    indexer.fileLines.push_back(vector<string>());
    // put empty vector of strings in filelines


    string line; // string to hold each line
    int lineNum = 0; // counter to track lineNum, will increment
    while(getline(file, line)) {
        istringstream iss(line);
        string word; 

        // int lineNumIndex = indexer.fileLines[filePathIndex].size();
        // line number index aligns with file lines vector at the
        // corresponding filePath index
        indexer.fileLines[filePathIndex].push_back(line); 

        // push_back current line onto fileLines at filePathIndex

        // duplicate words cannot be read from the same line!!
        while(iss >> word) {
            word = stripNonAlphaNum(word); 

            // strip non alpha num characters from given word
            wordStruct wordOccurrence = indexer.createWordStruct(filePathIndex, lineNum);

            // create word struct via indexer method 
            indexer.addToSensMap(word, wordOccurrence);
            // add to map via indexer
            indexer.addToInsensMap(word, wordOccurrence); 
            // add to map via indexer
        }
        lineNum++;
        // increment line num after reading each line
    }
    file.close(); 
    // close after getline fails (implying eof) 
}





/*
method to traverse fsTree and collect files
*/
void Gerp::traverseDirectory(string directory) {
    FSTree tree(directory);
    DirNode *root = tree.getRoot();
    if (root == nullptr) {
        cout << "Could not build index, exiting." << endl;
        return;
    }
    traverseDirectoryHelper(root, directory);
}

void Gerp::traverseDirectoryHelper(DirNode *root, string path) {
    if (root->isEmpty()) return;

    if (root->hasSubDir()) {
        for (int i = 0; i < root->numSubDirs(); i++) {
            traverseDirectoryHelper(root->getSubDir(i), path + "/" +
            root->getSubDir(i)->getName());
        }
    }

    if (root->hasFiles()) {
        for (int i = 0; i < root->numFiles(); i++) {
            string filepath = path + "/" + root->getFile(i);
            buildIndex(filepath); 
        }
    }
}

/*
strip non alpha numeric characters from query and file words
*/
string Gerp::stripNonAlphaNum(string input) {
    int first = 0;
    while (first < input.size() and(not isalnum(input[first]))) {
        first++; 
    }
    if(first == input.size()) {
        return "";
    }
    int last = input.size()-1; 
    while(last >= 0 and (not isalnum(input[last]))) {
        last--; 
    }
    return input.substr(first, last-first +1);
}
