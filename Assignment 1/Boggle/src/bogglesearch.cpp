// This program implements bogglesearch, which has the human player and computer player
//take turns searching for words. The human has to enter in words, while the computer
//can find all words instantly.

//Name: Noah Bailyn
//Section leader: Gianna Chien

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "bogglegui.h"
#include "hashmap.h"
using namespace std;

/*The humanWordSearchHelper for Boggle has the user enter in a word,
 * then returns whether or not that word is valid or can be found on
 * the board. Then, it scores the word based on its length. A recursive
 * backtracking process is used to do this by looking for letters corresponding
 * to each index of letters found in the word. If no letters are found in the
 * 8 surrounding tiles, the program backtracks an index and looks for other options.
*/
bool humanWordSearchHelper(Grid<char>& board, string word, string& chosen, int x, int y, HashMap<int, int> chosenXY, int &index) {
    if(chosen == toUpperCase(word)) {
        if(word.length() == 4) {
            BoggleGUI::scorePointsHuman(1);
        } else if(word.length() == 5) {
            BoggleGUI::scorePointsHuman(2);
        } else if(word.length() == 6) {
            BoggleGUI::scorePointsHuman(3);
        } else if(word.length() == 7) {
            BoggleGUI::scorePointsHuman(5);
        } else if(word.length() >= 8) {
            BoggleGUI::scorePointsHuman(11);
        }
        return true;
    } else if(index == 0 && (board.get(x, y) != word[0] || chosenXY.get(x) == y)) {
        if(x < board.numRows() - 1) {
            return humanWordSearchHelper(board, word, chosen, x + 1, y, chosenXY, index);
        } else if(y < board.numCols() - 1) {
            x = 0;
            return humanWordSearchHelper(board, word, chosen, x, y + 1, chosenXY, index);
        } else {
            return false;
        }
    } else if(index != 0 && (board.get(x, y) != word[index] || chosenXY.get(x) == y)) {
        return false;
    } else {
        //choose
        bool f = false;
        char ch = board.get(x, y);
        chosen += ch;
        chosenXY.put(x, y);
        BoggleGUI::setHighlighted(x, y);
        index++;
        //explore
        if(x > 0 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x - 1, y, chosenXY, index);
        }
        if(x < board.numRows() - 1 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x + 1, y, chosenXY, index);
        }
        if(y > 0 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x, y - 1, chosenXY, index);
        }
        if(y < board.numCols() - 1 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x, y + 1, chosenXY, index);
        }
        if(x > 0 && y > 0 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x - 1, y - 1, chosenXY, index);
        }
        if(x > 0 && y < board.numCols() - 1 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x - 1, y + 1, chosenXY, index);
        }
        if(x < board.numRows() - 1 && y < board.numRows() - 1 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x + 1, y + 1, chosenXY, index);
        }
        if(x < board.numRows() - 1 && y > 0 && !f) {
            f = humanWordSearchHelper(board, word, chosen, x + 1, y - 1, chosenXY, index);
        }
        //unchoose
        index--;
        chosen.erase(chosen.length() - 1);
        BoggleGUI::clearHighlighting();
        if(f) {
            return true;
        }
        return humanWordSearchHelper(board, word, chosen, x, y, chosenXY, index);
    }
}

/*The normal humanWordSearch function passes parameters into the humanWordSearchHelper
 * function, and makes sure that the word passed in is in the dictionary and
 * above 4 letters long.
*/
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    if(word.length() < 4) {
        return false;
    } else if(!dictionary.contains(word)) {
        return false;
    }
    HashMap<int, int> chosenXY;
    chosenXY.put(1, 10);
    chosenXY.put(2, 10);
    chosenXY.put(3, 10);
    chosenXY.put(0, 10);
    int index = 0;
    int x = 0;
    int y = 0;
    string chosen;
    return humanWordSearchHelper(board, word, chosen, x, y, chosenXY, index);
}

/*The computerWordSearchHelper function looks for every possible word that can be
 * formed on the current Boggle board, tallies up their total score, and compares
 * it to the human's score. This is done through a recursive backtracking process
 * that looks at prefixes of words in the dictionary and compares them to the
 * surrounding tiles on the board. If no prefixes are found corresponding to that
 * series of tiles, the program backtracks and tries another tile.
*/
void computerWordSearchHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords, Set<string>& computerWords, int tilesTested, int x, int y, string& word, Set<Vector<int>> chosenXY) {
    Vector<int> a;
    a.add(x);
    a.add(y);
    Vector<int> b;
    b.add(x - 1);
    b.add(y);
    Vector<int> c;
    c.add(x + 1);
    c.add(y);
    Vector<int> d;
    d.add(x);
    d.add(y + 1);
    Vector<int> e;
    e.add(x);
    e.add(y - 1);
    Vector<int> f;
    f.add(x + 1);
    f.add(y + 1);
    Vector<int> g;
    g.add(x + 1);
    g.add(y - 1);
    Vector<int> h;
    h.add(x - 1);
    h.add(y - 1);
    Vector<int> i;
    i.add(x - 1);
    i.add(y + 1);
    bool t = true;
    if(tilesTested < 16 && !chosenXY.contains(a) && !humanWords.contains(word) && dictionary.containsPrefix(word + board.get(x, y))) {
        //choose
        char ch = board.get(x, y);
        word += ch;
        if(dictionary.contains(word) && !computerWords.contains(word) && word.length() >= 4) {
            computerWords.add(word);
            if(word.length() == 4) {
                BoggleGUI::scorePointsComputer(1);
            } else if(word.length() == 5) {
                BoggleGUI::scorePointsComputer(2);
            } else if(word.length() == 6) {
                BoggleGUI::scorePointsComputer(3);
            } else if(word.length() == 7) {
                BoggleGUI::scorePointsComputer(5);
            } else if(word.length() >= 8) {
                BoggleGUI::scorePointsComputer(11);
            }
        }
        chosenXY.add(a);
        //explore
        computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x, y, word, chosenXY);
        //unchoose
        word.erase(word.length() - 1);
        x = a.get(0);
        y = a.get(1);
        chosenXY.remove(a);
        t = false;
        //once all possible combinations have been exhausted, moves on to the next line and increments tilesTested
        if(word.length() == 0) {
            if(x < board.numRows() - 1) {
                tilesTested++;
                x = tilesTested % 4;
                computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x, y, word, chosenXY);
            } else if(y < board.numCols() - 1) {
                y += 1;
                x = 0;
                tilesTested++;
                computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x, y, word, chosenXY);
            }
        }

    }
    //exploring the 8 squares around the chosen square
    /*The reason for the concentric if statements is that the function will call
     * tiles out of place if it recurs at all, so it must be kept from recurring.
    */
    if(!chosenXY.contains(b) && x > 0 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x - 1, y))) {
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x - 1, y, word, chosenXY);
        }
    }
    if(!chosenXY.contains(c) && x < board.numRows() - 1 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x + 1, y))) {
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x + 1, y, word, chosenXY);
        }
    }
    if(!chosenXY.contains(d) && y < board.numCols() - 1 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x, y + 1))){
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x, y + 1, word, chosenXY);
        }
    }
    if(!chosenXY.contains(e) && y > 0 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x, y - 1))) {
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x, y - 1, word, chosenXY);
        }
    }
    if(!chosenXY.contains(f) && x < board.numRows() - 1 && y < board.numCols() - 1 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x + 1, y + 1))) {
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x + 1, y + 1, word, chosenXY);
        }
    }
    if(!chosenXY.contains(g) && x < board.numRows() - 1 && y > 0 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x + 1, y - 1))) {
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x + 1, y - 1, word, chosenXY);
        }
    }
    if(!chosenXY.contains(h) && x > 0 && y > 0 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x - 1, y - 1))) {
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x - 1, y - 1, word, chosenXY);
        }
    }
    if(!chosenXY.contains(i) && x > 0 && y < board.numCols() - 1 && tilesTested < 16 && t) {
        if(dictionary.containsPrefix(word + board.get(x - 1, y + 1))) {
            computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x - 1, y + 1, word, chosenXY);
        }
    }
}

/*The computerWordSearch function passes parameters into its helper function and returns
 * the set of words the computer found.
*/
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> computerWords;
    int tilesTested = 0;
    int x = 0;
    int y = 0;
    Set<Vector<int>> chosenXY;
    string word;
    computerWordSearchHelper(board, dictionary, humanWords, computerWords, tilesTested, x, y, word, chosenXY);
    return computerWords;
}
