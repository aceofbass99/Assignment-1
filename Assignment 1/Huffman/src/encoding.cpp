// This program implements Huffman encoding, which works to decrease
// the size of a file passed in by changing its binary coding to occupy
// the minimum space. This is done through a combination of encoding and
// decoding the subject file, and is explained more in depth in the comments
// above each method.

//Name: Noah Bailyn
//Section leader: Gianna Chien

#include "encoding.h"
#include "vector.h"
#include "priorityqueue.h"
#include "map.h"
#include "set.h"
#include "filelib.h"
using namespace std;

//Clears the tree passed in from memory.
void freeTree(HuffmanNode* node) {
    if(node != nullptr) {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}

//Reads an input file with a bunch of characters in it. After it does
//so, this method then counts the number of occurences for each character
//and stores each different character in one vector and its respective amount
//of occurences in another vector. The occurence vector is mapped to the
// vector with the characters in it and returned.
Map<Vector<int>, Vector<int>> mapOccurrences(istream& input) {
    Vector<int> characters;
    Vector<int> occurences;
    while(true) {
        int nextChar = input.get();
        if(nextChar == EOF) break;
        if(!characters.contains(nextChar)) {
            characters.add(nextChar);
            occurences.add(1);
        } else {
            int index = characters.indexOf(nextChar);
            int currentOccurences = occurences.get(index);
            occurences.remove(index);
            occurences.insert(index, currentOccurences + 1);
        }
    }
    characters.add(PSEUDO_EOF);
    occurences.add(1);
    Map<Vector<int>, Vector<int>> map;
    map.put(characters, occurences);
    return map;
}

//Builds the binary tree, given a map like the one created above passed in as a
//parameter. This tree is built by combining the first two highest-priority characters
//into a node that points to each, and then putting that node back into the queue with the
//combined priority of both existing characters. The node that points to the entire tree
//is passed back.
HuffmanNode* buildEncodingTreeHelper(Map<Vector<int>, Vector<int>>& occurenceMap) {
    Vector<int> characters = occurenceMap.keys().get(0);
    Vector<int> occurences = occurenceMap.get(characters);
    PriorityQueue<HuffmanNode*> pqueue;
    while(characters.size() != 0) {
        HuffmanNode* nodePt = new HuffmanNode;
        nodePt->character = characters.get(0);
        nodePt->count = occurences.get(0);
        pqueue.enqueue(nodePt, nodePt->count);
        characters.removeFront();
        occurences.removeFront();
    }
    while(pqueue.size() > 1) {
        HuffmanNode* node1 = pqueue.dequeue();
        HuffmanNode* node2 = pqueue.dequeue();
        HuffmanNode* node3 = new HuffmanNode;
        node3->character = NOT_A_CHAR;
        node3->count = node1->count + node2->count;
        node3->zero = node1;
        node3->one = node2;
        pqueue.enqueue(node3, node3->count);
    }
    return pqueue.dequeue();
}

//Calls the helper, so that the helper has the ability to only take in a map
//in order to build the tree, which comes in handy with the compress and decompress
//methods.
HuffmanNode* buildEncodingTree(istream& input) {
    Map<Vector<int>, Vector<int>> occurenceMap = mapOccurrences(input);
    return buildEncodingTreeHelper(occurenceMap);
}

//Recursively maps the binary code for each character in the stream into a Map. This Map
//is passed by reference, so it is not returned. While it may seem heavy on "if"s, the fact
//that the base case has to point to the current node and not a null node in order to store
//the character in the map adds more restrictions to the fluidity of the recursion.
void findTreeCode(Map<int, string>& map, HuffmanNode* encodingTree, string& startingString) {
    if(encodingTree->zero != nullptr || encodingTree->one != nullptr) {
        //recursive case, focuses on next case so base case can retain the node's character
        if(encodingTree->zero != nullptr) {
            startingString += "0";
            HuffmanNode* save = encodingTree;
            encodingTree = encodingTree->zero;
            findTreeCode(map, encodingTree, startingString);
            encodingTree = save;
            save = nullptr;
            startingString.erase(startingString.length() - 1);
        }
        if(encodingTree->one != nullptr) {
            startingString += "1";
            HuffmanNode* save = encodingTree;
            encodingTree = encodingTree->one;
            findTreeCode(map, encodingTree, startingString);
            encodingTree = save;
            save = nullptr;
            startingString.erase(startingString.length() - 1);
        }
    } else {
        //base case
        map.put(encodingTree->character, startingString);
    }
}

//Using the map of characters to their binary code found above, this method
//encodes the whole message in binary and writes it to the obitstream in bits.
void encodeData(istream& input, HuffmanNode* encodingTree, obitstream& output) {
    Map<int, string> map;
    string startingString = "";
    findTreeCode(map, encodingTree, startingString);
    int currentChar = 0;
    while(true) {
        currentChar = input.get();
        if(currentChar == EOF) break;
        string binary = map.get(currentChar);
        while(binary.length() > 0) {
            int num = stringToInteger(binary.substr(0, 1));
            binary = binary.substr(1);
            if(num == 0) {
                output.writeBit(0);
            } else {
                output.writeBit(1);
            }
        }
    }
    string binary = map.get(PSEUDO_EOF);
    while(binary.length() > 0) {
        int num = stringToInteger(binary.substr(0, 1));
        binary = binary.substr(1);
        if(num == 0) {
            output.writeBit(0);
        } else {
            output.writeBit(1);
        }
    }
}

//Takes the data from an ibitstream (binary), and decodes it recursively
//using the existing tree. The tree is traveled down recursively, but is
//called repeatedly from its original function in order to reset at the
//top of the tree for each new character. Again, since the base case has
//to mention the node's character, it comes at the cost of the conciseness
//of the function.
int decode(ibitstream& input, HuffmanNode* encodingTree) {
    if(encodingTree->zero == nullptr && encodingTree->one == nullptr) {
        //base case
        return encodingTree->character;
    } else {
        //recursive case
        int num = input.readBit();
        if(num == 0) {
            if(encodingTree->zero != nullptr) {
                encodingTree = encodingTree->zero;
            }
        } else if(num == 1) {
            if(encodingTree->one != nullptr) {
                encodingTree = encodingTree->one;
            }
        }
        if(num != -1) {
            return decode(input, encodingTree);
        } else {
            return PSEUDO_EOF;
        }
    }
}

//Calls its recursive helper function (above) until it hits PSEUDO_EOF, in which
//case it stops calling the helper function. For each call, the character returned
//is put into the output stream.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(true) {
        int num = decode(input, encodingTree);
        if(num == PSEUDO_EOF) break;
        output.put(num);
    }
}

//Compresses a given file by adding a header consisting of the map of each character
//to its frequency. After the header, the file is compressed into binary normally.
//The tree created in order to compress is deleted after the file is completely compressed.
void compress(istream& input, obitstream& output) {
    Map<Vector<int>, Vector<int>> map = mapOccurrences(input);
    output << map;
    rewindStream(input);
    HuffmanNode* tree = buildEncodingTreeHelper(map);
    encodeData(input, tree, output);
    freeTree(tree);
}

//Uncompresses a compressed file by using the header provided in order to create
//a tree. This tree decodes the binary in the input stream so that it can be
//put into normal text for the output stream. The tree created in order to decode
//is deleted after this is done.
void uncompress(ibitstream& input, ostream& output) {
    Map<Vector<int>, Vector<int>> map;
    input >> map;
    HuffmanNode* tree = buildEncodingTreeHelper(map);
    decodeData(input, tree, output);
    freeTree(tree);
}


