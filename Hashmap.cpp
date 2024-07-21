/* Hashmap.cpp
* Written by: Weston Starbird and Elki Laranas
* Date: 4/29/24
* Purpose: Implementation of the Hashmap class
*/

#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <utility>
#include "Hashmap.h"
#include "Index.h"

using namespace std;



Hashmap::Hashmap() {
    buckets.resize(10);
    size = 0;
}

Hashmap::~Hashmap(){}

vector<wordStruct>& Hashmap::at(const string& key) {
    size_t index = hash(key);
    size_t originalIndex = index;
    while (buckets[index].isOccupied) {
        if (buckets[index].key == key) {
            return buckets[index].value;
        }
        index = getNextIndex(index);
        if (index == originalIndex) {
            break;
        }
    }
    throw std::out_of_range("Key not found");
}

bool Hashmap::contains(const string& key) {
    size_t index = hash(key);
    size_t originalIndex = index;
    while (buckets[index].isOccupied) {
        if (buckets[index].key == key) {
            return true;
        }
        index = getNextIndex(index);
        if (index == originalIndex) {
            break;
        }
    }
    return false;
}

void Hashmap::insert(const string& key, const wordStruct& value) {
    if (static_cast<float>(size + 1) / buckets.size() > maxLoadFactor) {
        rehash();
    }

    size_t index = hash(key);
    while (buckets[index].isOccupied) {
        if (buckets[index].key == key) {
            buckets[index].value.push_back(value);
            return;
        }
        index = getNextIndex(index);
    }

    buckets[index] = Node(key, value);
    ++size;
}

size_t Hashmap::hash(const string& key) {
    return std::hash<string>{}(key) % buckets.size();
}

void Hashmap::rehash() {
    vector<Node> oldBuckets = std::move(buckets);
    buckets.resize(oldBuckets.size() * 2);
    size = 0;
    for (const auto& node : oldBuckets) {
        if (node.isOccupied) {
            for (const auto& value : node.value) {
                insert(node.key, value);
            }
        }
    }
}

size_t Hashmap::getNextIndex(size_t index) {
    return (index + 1) % buckets.size();
}


