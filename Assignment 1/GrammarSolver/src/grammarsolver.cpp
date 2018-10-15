/*These methods read a grammar program, save them to a HashMap,
 * and then create structures based on user input and based
 * on that HashMap.
 *
 * Name: Noah Bailyn
 * Section leader: Gianna Chien
*/
#include "grammarsolver.h"
#include "hashmap.h"
#include "random.h"
#include "tokenscanner.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include <iostream>
#include <fstream>
using namespace std;

/*At the end of the method, the vector of structures is
 * then converted back into a readable string that looks
 * more like a sentence. Each part of the vector is
 * concatenated onto the string and spaces are added.
*/
string vectorToString(Vector<string> &v) {
    string result;
    for(int i = 0; i < v.size(); i++) {
        result += v.get(i);
        result += " ";
    }
    return result;
}

/*This method reads the grammar into a HashMap that maps strings
 * (the structure names) to vectors of vectors of strings. The
 * reason there need to be vectors of vectors is that there are
 * multiple ways of separating structure: a '|', which denotes
 * the end of a possible choice, and a ' ', which denotes that
 * the previous and next choices are both part of the structure.
*/
HashMap<string, Vector<Vector<string>>> getMap(istream& input) {
    HashMap<string, Vector<Vector<string>>> h;
    while(true) {
        string str;
        getline(input, str);
        if(input.fail()) break;
        string term = str.substr(0, str.find(":"));
        string rest = str.substr(str.find("=") + 1, str.length());
        Vector<Vector<string>> outer;
        for(int i = 0; i < rest.size(); i++) {
            if(rest.at(i) == '|') {
                string first = rest.substr(0, i);
                Vector<string> inner;
                inner.add(first);
                rest = rest.substr(i + 1);
                i = 0;
                outer.add(inner);
            }
        }
        Vector<string> inner;
        inner.add(rest);
        outer.add(inner);
        for(int i = 0; i < outer.size(); i++) {
            Vector<string> newInner;
            string s = outer.get(i).get(0);
            for(int j = 0; j < s.size(); j++) {
                if(s.at(j) == ' ') {
                    string first = s.substr(0, j);
                    newInner.add(first);
                    s = s.substr(j + 1);
                    j = 0;
                }
            }
            newInner.add(s);
            outer.remove(i);
            outer.insert(i, newInner);
        }
        h.put(term, outer);
    }
    return h;
}

/*This is the recursive helper function for the generateStructure method.
 * This helper method uses recursion to generate a tree of structures based
 * on the one that the user entered. The base case is when the marker reaches
 * the last structure, and it is not a key in the HashMap.
*/
string generateStructureHelper(HashMap<string, Vector<Vector<string>>> &h, Vector<string> &structure, int &structureIndex) {
    if(structureIndex == structure.size()) {
        return vectorToString(structure);
    } else if(!h.containsKey(structure.get(structureIndex))) {
        structureIndex += 1;
        return generateStructureHelper(h, structure, structureIndex);
    } else {
        Vector<Vector<string>> v = h.get(structure.get(structureIndex));
        structure.remove(structureIndex);
        int random = randomInteger(0, v.size() - 1);
        for(int i = v.get(random).size() - 1; i >= 0; i--) {
            structure.insert(structureIndex, v.get(random).get(i));
        }
        return generateStructureHelper(h, structure, structureIndex);
    }
}

/*This method calls its helper method so it can add another parameter, the structureIndex.
*/
string generateStructure(HashMap<string, Vector<Vector<string>>> &h, string &startingSymbol) {
    Vector<string> v;
    v.add(startingSymbol);
    int i = 0;
    return generateStructureHelper(h, v, i);
}

/*This method generates a grammar structure based on the structure name the user passes in.
 * It does this by reading the grammar into a HashMap, and then creating a random tree
 * of structures randomly and recursively. The vector of structures is passed back.
*/
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    HashMap<string, Vector<Vector<string>>> h = getMap(input);
    Vector<string> v;
    for(int i = 0; i < times; i++) {
        v.add(generateStructure(h, symbol));
    }
    return v;
}
