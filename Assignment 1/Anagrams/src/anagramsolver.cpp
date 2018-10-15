//This page implements the findAnagrams function, which prints out all the anagrams associated
//with a word and returns the amount of anagram groups printed.
//
//Name: Noah Bailyn
//Section leader: Gianna Chien

#include <string>
#include "letterinventory.h"
#include "set.h"
#include "strlib.h"
#include "hashset.h"
#include <iterator>
#include <set>
using namespace std;

/* This is the helper function for findAnagrams. This function recursively finds all viable sets of outcomes for
 * anagrams of a word, given the anagrams. After it is done, it prints all the vectors with anagrams that contain
 * MAX or fewer anagrams, unless MAX is 0, in which case it prints all the anagrams.
*/
int findAnagramsHelper(LetterInventory& inventory, Vector<string> chosen, int max, Set<string>& dictionary, Vector<string>& words, int& n) {
    if((inventory.isEmpty() && chosen.size() <= max) || (inventory.isEmpty() && max == 0)) {
        //base case
        cout << chosen << endl;
        n++;
        return n;
    } else {
        for(int i = 0; i < words.size(); i++) {
            string newWord = words.get(i);
            if(inventory.contains(newWord)) {
                //Choosing the next anagram in the vector
                inventory.subtract(newWord);
                chosen.add(newWord);
                //Exploring that anagram
                findAnagramsHelper(inventory, chosen, max, dictionary, words, n);
                //Unchoosing that anagram
                inventory.add(newWord);
                chosen.removeBack();
            }
        }
        return n;
    }
}

/* Most of the work is done by the helper function, but the findAnagrams function does pass it important
 * parameters. These include the letter inventory for the phrase, which keeps track of the letters used
 * so far and helps search for anagrams in the dictionary, the vector of possible anagrams, the vector
 * of words chosen to be anagrams, the number of anagrams found, the dictionary, and the max amount
 * of anagrams per phrase to be found. Importantly, this function also uses an iterator to find
 * all the anagrams in the dictionary that correspond to the phrase passed in.
*/
int findAnagrams(string phrase, int max, Set<string>& dictionary) {
    LetterInventory li(phrase);
    Vector<string> words;
    Vector<string> chosen;
    Set<string>::iterator itr = dictionary.begin();
    int m = 0;
    //Reading the anagrams into a vector.
    while(itr != dictionary.end()) {
        if(li.contains(*itr)) {
            words.add(*itr);
        }
        itr++;
    }
    //Calling the recursive function.
    int n = findAnagramsHelper(li, chosen, max, dictionary, words, m);
    return n;
}
