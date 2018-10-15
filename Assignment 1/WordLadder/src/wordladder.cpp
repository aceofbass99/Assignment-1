/* This is the file for the Word Ladders program. This program
 * prompts the user for two words, and forms a "bridge" or
 * ladder of words between them using only words found in the
 * provided dictionary.
 *
 * Name: Noah Bailyn
 * Section leader: Gianna Chien
*/

#include <iostream>
#include "console.h"
#include "vector.h"
#include "filelib.h"
#include "simpio.h"
#include "stack.h"
#include "queue.h"
#include "strlib.h"
#include "hashset.h"
using namespace std;

/*This method returns all the "neighbor" words for a particular given word
 * at the front of the corresponding Stack passed in. While it is searching
 * for words, it adds all the neighbors to a "words" HashSet, which details
 * all the words that have already been found and should be avoided.
*/
Vector<string> findNeighbors(Stack<string> s, HashSet<string> &words, HashSet<string> &dictionary) {
    string word = s.peek();
    for(int i = 0; i < s.size(); i++) {
        words.add(s.pop());
    }
    string neighbor = word;
    Vector<string> neighbors;
    for(int i = 0; i < neighbor.length(); i++) {
        for(char j = 'a'; j <= 'z'; j++) {
            neighbor = neighbor.substr(0, i) + j + neighbor.substr(i + 1);
            if(dictionary.contains(neighbor) && words.contains(neighbor) == false) {
                neighbors.add(neighbor);
                words.add(neighbor);
            }
            neighbor = word;
        }
    }
    return neighbors;
}

/*This method checks whether the words passed in are appropriate for use in the word ladder.
 * If they are, it passes back true, and if they are not, it passes back false.
*/
bool check(string &word1, string &word2, HashSet<string> &dictionary) {
    if(dictionary.contains(word1) && dictionary.contains(word2) && word1 != word2 && word1.length() == word2.length()) {
        return true;
    }else {
        return false;
    }
}

/*This method asks the user to input a word. If the user does not enter a word, the
 * game then quits, which is denoted by "playingGame" being set to false. The text
 * changes based on whether the user has already entered a word or not.
*/
string readWord(int &numWords, bool &playingGame) {
    string word;
    if(numWords == 0) {
        word = getLine("Word 1 (or Enter to quit):");
        numWords++;
        if(word == ""){
            playingGame = false;
        }
    } else {
        word = getLine("Word 2 (or Enter to quit):");
        numWords = 0;
        if(word == ""){
            playingGame = false;
        }
    }
    if(word != "") {
        word = toLowerCase(word);
    }
    return word;
}

/*This method asks the user for a file to be used for a dictionary, and, if
 * it is a legitimate file, reads the dictionary into a HashSet, which is
 * passed back and stored for later use.
*/
HashSet<string> readDictionary() {
    cout << "" << endl;
    HashSet<string> dictionary;
    ifstream fileStream;
    string fileName = getLine("Dictionary file name: ");
    while(fileExists(fileName) == false){
        cout << "Unable to open that file. Try again." << endl;
        fileName = getLine("Dictionary file name: ");
    }
    fileStream.open(fileName.c_str());
    string line;
    while(fileStream.fail() == false) {
        getline(fileStream, line);
        if(fileStream.fail() == false) {
            dictionary.add(line);
        }
    }
    fileStream.close();
    return dictionary;
}

/*This method prints the three lines of text seen at the beginning of
 * the program.
*/
void printIntroText() {
    cout << "Welcome to CS106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl;
}

/*This method prints the result of a word ladder once a stack containing
 * the word ladder has been passed in. This is called every time a
 * successful word ladder is made.
*/
void printStack(Stack<string> &s, string word1, string word2) {
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    string result = "";
    Stack<string> s2 = s;
    for(int i = 0; i < s.size(); i++) {
        result += s2.pop();
        result += " ";
    }
    cout << result << endl;
}

/*This method creates the word ladder by making a queue of stacks,
 * and then checking each new stack to see if the goal word is in it.
 * If it is, then the method stops and calls printStack. If there is
 * no possible word ladder, the console prints a message saying so.
*/
void ladder(string &word1, string &word2, HashSet<string> &dictionary) {
    Queue<Stack<string>> queue;
    HashSet<string> lines;
    lines.add(word1);
    Stack<string> stack1;
    stack1.push(word1);
    queue.enqueue(stack1);
    bool containsWord = false;
    while(containsWord == false && queue.isEmpty() == false) {
        Stack<string> s = queue.dequeue();
        Vector<string> neighbors = findNeighbors(s, lines, dictionary);
        for(int i = 0; i < neighbors.size(); i++) {
            Stack<string> s2 = s;
            s2.push(neighbors.get(i));
            queue.enqueue(s2);
            if(neighbors.get(i) == word2) {
                printStack(s2, word1, word2);
                containsWord = true;
                break;
            }
        }
    }
    if(queue.isEmpty()) {
        cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
    }
}

/*This method gives error messages if the words passed in are invalid.
 * Additionally, it loops due to the main method, and calls the ladder
 * method once appropriate words have been entered.
*/
bool playGame(HashSet <string> &dictionary, bool &playingGame) {
    int numWords = 0;
    cout << "" << endl;
    string word1 = readWord(numWords, playingGame);
    string word2;
    if(playingGame) {
        word2 = readWord(numWords, playingGame);
    }
    while(check(word1, word2, dictionary) == false && playingGame) {
        if(dictionary.contains(word1) == false || dictionary.contains(word2) == false) {
            cout << "The two words must be found in the dictionary." << endl;
        } else if(word1 == word2) {
            cout << "The two words must be different." << endl;
        } else {
            cout << "The two words must be the same length." << endl;
        }
        cout << "" << endl;
        word1 = readWord(numWords, playingGame);
        if(playingGame) {
            word2 = readWord(numWords, playingGame);
        }
    }
    if(playingGame) {
        ladder(word1, word2, dictionary);
    }
    return playingGame;
}

/* The main method calls each of the other methods, and ends the game
 * once the user has chosen not to enter in a word.
*/
int main() {
    printIntroText();
    HashSet<string> dictionary = readDictionary();
    bool playingGame = true;
    while(playingGame) {
        playGame(dictionary, playingGame);
    }
    cout << "Have a nice day." << endl;
    return 0;
}
