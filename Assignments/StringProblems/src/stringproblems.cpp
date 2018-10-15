/*These are the string recursion problems. There are four: contains, lettersOnly,
 * printBinary, and reverse. Each is explained in their separate comment, and these
 * are called with the stringproblemsmain.cpp program.
 *
 * Name: Noah Bailyn
 * Section leader: Gianna Chien
*/

#include "stringproblems.h"
#include <iostream>
#include <cctype>
using namespace std;

/*This method determines whether or not string 1 contains string 2. It does
 * this by taking one character off string 1 if the starting 2 characters of each
 * do not match, or taking 1 character off both strings if the starting 2 characters
 * do match. Then, if repeating this process leads to an empty string 2 or 1 character
 * in each string that matches, a true is returned, and, if string 1 does not contain
 * string 2, string 1 will end up shorter than string 2 and false is returned.
*/
bool contains(string s1, string s2) {
    if(s2.empty() || (tolower(s2[0]) == tolower(s1[0]) && s2.length() == 1)) {
        return true;
    } else if (s1.length() < s2.length()) {
        return false;
    } else {
        if(tolower(s1[0]) == tolower(s2[0]) && tolower(s1[1]) == tolower(s2[1])) {
            s2 = s2.substr(1);
        }
        s1 = s1.substr(1);
        return (s1.length() >= s2.length() && contains(s1, s2));
    }
}

/*This is the helper function for lettersOnly. It separates the letters from
 * a string of a combination of letters and other characters. It does this by
 * removing all the characters from the original string 1 by 1 and testing whether
 * they are or are not letters, and then adding the character to the letters string
 * only if they are letters. Then, the letters string is returned.
*/
string lettersOnlyHelper(string s, string letters) {
    if(!s.empty()) {
        if(isalpha(s[0])) {
            letters += s[0];
        }
        return lettersOnlyHelper(s.substr(1), letters);
    }else{
        return letters;
    }
}

/*The lettersOnly method calls the lettersOnlyHelper method so that a new
 * parameter may be used.
*/
string lettersOnly(string s) {
    return lettersOnlyHelper(s, "");
}

/*This is the helper function for printBinary. When called, the function
 * finds the highest power of 2 less than the given number, n, and sets
 * the indicator to true. Once that is done, that power of 2 is subtracted
 * from n and a 1 is printed, and twoPower is divided by 2. If twoPower is ever
 * more than n, it is halved and a 0 is printed. This process continues until
 * n is 0 and twoPower is 0.5.
*/
void printBinaryHelper(int n, int twoPower, bool indicator) {
    if(indicator && n == 0 && twoPower < 1) {
    } else if(!indicator){
        if(twoPower * 2 > n) {
            indicator = true;
            if(n != 0) {
                printBinaryHelper(n, twoPower, indicator);
            } else {
                cout << "0";
            }
        } else {
            printBinaryHelper(n, twoPower * 2, indicator);
        }
    } else {
        if(twoPower <= n) {
            n -= twoPower;
            cout << "1";
        } else {
            cout << "0";
        }
        twoPower = twoPower / 2;
        printBinaryHelper(n, twoPower, indicator);
    }
}

/*The printBinary method calls the printBinaryHelper method so that a new
 * parameter may be used.
*/
void printBinary(int n) {
    if(n < 0) {
        cout << "-";
        n = -n;
    }
    printBinaryHelper(n, 1, false);
}

/*This is the helper function for the reverse method. It reverses a string
 * by creating a new string called reverse, and then transferring all the chars
 * in order from the original string to the beginning of the new string. This
 * reverses the string, and then the string is returned once the original
 * string is empty.
*/
string reverseHelper(string s, string reverse) {
    if(!s.empty()) {
        reverse = s[0] + reverse;
        return reverseHelper(s.substr(1), reverse);
    } else {
        return reverse;
    }
}

/*The reverse method calls the reverseHelper method so that a new
 * parameter may be used.
*/
string reverse(string s) {
    return reverseHelper(s, "");
}
