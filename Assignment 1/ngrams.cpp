/*This program makes an N-gram out of a series of words given in a text file.
 * An N-gram is a series of words that are randomly predicted based on the
 * N words preceding it, and how often those words lead to another in the
 * original file.
 *
 * Name: Noah Bailyn
 * Section leader: Gianna Chien
*/

#include <iostream>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "hashmap.h"
#include "tokenscanner.h"
#include "vector.h"
#include "queue.h"
#include "random.h"
using namespace std;

/*This method creates and prints the text for the final N-gram. Based on
 * the last window of text used, it randomly generates a new word, adds
 * that word to the result string, and moves the window forward by one.
*/
void printWords(HashMap<Queue<string>, Vector<string>> &map, int &numWords) {
    Vector<Queue<string>> windows = map.keys();
    string result = "... ";
    int keyNum = randomInteger(0, windows.size() - 1);
    Queue<string> queue = windows.get(keyNum);
    for(int i = 0; i < numWords; i++) {
        int resultNum = randomInteger(0, map.get(queue).size() - 1);
        string newWord = map.get(queue).get(resultNum);
        queue.enqueue(newWord);
        queue.dequeue();
        result += newWord;
        result += " ";
    }
    result += "...";
    cout << result << endl;
}

/*This method maps all the words in a give text file very specifically. Each
 * group or window of n words is mapped to the word directly after it. If
 * a window repeats at any point in the text, the result word is added to
 * the existing vector of result words. This map is later used to randomize
 * the N-gram.
*/
HashMap<Queue<string>, Vector<string>> mapWords(Vector<string> &wordList, int &n) {
    Queue<string> window;
    Vector<string> targets;
    HashMap<Queue<string>, Vector<string>> map;
    for(int i = 0; i < (n - 1); i++) {
        window.enqueue(wordList.get(i));
    }
    targets.add(wordList.get(n - 1));
    map.put(window, targets);
    targets.clear();
    for(int i = (n - 1); i < (wordList.size() - 2); i++) {
        window.dequeue();
        window.enqueue(wordList.get(i));
        if(map.containsKey(window)) {
            targets = map.get(window);
            targets.add(wordList.get(i + 1));
            map.put(window, targets);
            targets.clear();
        } else {
            targets.add(wordList.get(i + 1));
            map.put(window, targets);
            targets.clear();
        }
    }
    return map;
}

/*This method opens up a file and reads all the words into
 * it into a vector containing strings, then returns that
 * vector. The string it is given is pre-approved by the
 * getFile method.
*/
Vector<string> readWords(string &file, int &n) {
    ifstream stream;
    stream.open(file.c_str());
    string singleWord;
    Vector<string> wordList;
    while(!stream.fail()) {
        stream >> singleWord;
        if(stream.fail()) break;
        wordList.add(singleWord);
    }
    for(int i = 0; i < n; i++) {
        wordList.add(wordList.get(i));
    }
    return wordList;
}

/*This method has the user enter a string that corresponds
 * to a file existing on the computer. If the file does not
 * exist, the user is prompted to retry.
*/
string getFile() {
    cout << "" << endl;
    string str = getLine("Input file name: ");
    while(fileExists(str) == false) {
        cout << "Unable to open that file. Try again." << endl;
        str = getLine("Input file name: ");
    }
    return str;
}

/*This method prompts the user to enter a value for N. Values
 * under 2 are not accepted, and non-integers are also not
 * accepted.
*/
int getN() {
    int n = getInteger("Value of N: ");
    while(n < 2) {
        cout << "N must be 2 or greater." << endl;
        n = getInteger("Value of N: ");
    }
    return n;
}

//This method prints the text at the beginning of the program.
void printStartText() {
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups of" << endl;
    cout << "words, and I will generate random text for you." << endl;
}

/*This method prompts the user to enter the number of words they
 * would like like to generate in their N-gram. If the user enters
 * a 0, a 0 is returned and the program quits.
*/
int getNumWords(int &n) {
    cout << "" << endl;
    int numWords = getInteger("# of random words to generate (0 to quit): ");
    while(numWords < n) {
        if(numWords == 0) break;
        cout << "Must be at least " << n << " words." << endl;
        numWords = getInteger("# of random words to generate (0 to quit): ");
    }
    return numWords;
}

/*The main method prints the starting text, asks the user for the file
 * name and n, then loops over asking the user for how many words they
 * would like. If a 0 is ever entered, the loop ends and "Exiting." is
 * displayed.
*/
int main() {
    printStartText();
    string file = getFile();
    int n = getN();
    int numWords = getNumWords(n);
    while(numWords > 0) {
        Vector<string> wordList = readWords(file, n);
        HashMap<Queue<string>, Vector<string>> map = mapWords(wordList, n);
        printWords(map, numWords);
        numWords = getNumWords(n);
    }
    cout << "Exiting." << endl;
    return 0;
}
