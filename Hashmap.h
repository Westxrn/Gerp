/* Hashmap.h
* Written by: Weston Starbird and Elki Laranas
* Date: 4/29/24
* Purpose: templated hashmap ADT with added functionality
*/
#ifndef HASHMAP_H
#define HASHMAP_H


#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <utility>
#include "wordStruct.h" 


using namespace std;

class Hashmap {
public:
    Hashmap();
    ~Hashmap();
    
    vector<wordStruct>& at(const string& key);
    bool contains(const string& key);
    void insert(const string& key, const wordStruct& value);

private:
    struct Node {
        string key;
        vector<wordStruct> value;
        bool isOccupied;
        Node() : isOccupied(false) {}
        Node(const string& k, const wordStruct& v) : key(k), value{v}, isOccupied(true) {}
    };

    vector<Node> buckets;
    size_t size;
    const float maxLoadFactor = 0.45; // 0.45 is best so far

    size_t hash(const string& key);
    void rehash();
    size_t getNextIndex(size_t index);
};

#endif // HASHMAP_H
