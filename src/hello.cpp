#include <iostream>
#include "hello.h"
using namespace std;

/**
 * @brief prompt_wl
 * I/O for getting the word length from user
 * @return
 */
int prompt_wl() {
    int wl = 0;
    bool valid = false;
    int num_tries = 0;
    cout << "Enter desired word length: ";

    while (!valid) {
        if (num_tries > 0) cout << "Please enter a valid integer. Try again:" << endl;
        string input;
        getline(cin, input);
        istringstream stream(input);
        stream >> wl;
        if (wl > 0) valid = true;
        num_tries ++;
    }
    return wl;
}

/**
 * @brief prompt_ng
 * I/O for getting the number of guesses from user
 * @return
 */
int prompt_ng() {
    int ng = 0;
    bool valid = false;
    int num_tries = 0;
    cout << "Enter desired number of guesses: ";

    while (!valid) {
        if (num_tries > 0) cout << "Please enter a valid integer. Try again:" << endl;
        string input;
        getline(cin, input);
        istringstream stream(input);
        stream >> ng;
        if (ng > 0) valid = true;
        num_tries ++;
    }
    return ng;
}

/**
 * @brief playAgain
 * I/O for figuring out if user wants to play again
 * @return
 */
bool playAgain() {
    bool again;
    bool valid = false;
    int numTries = 0;
    string line;
    cout << "Do you want to play another game?" << endl;
    while (!valid) {
        line = "";
        if (numTries > 0) cout << "Invalid response, please enter yes or no: " << endl;
        getline(cin, line);
        if (line[0] == 'y' || line[0] == 'Y') {
            valid = true;
            again = true;
        } else if (line[0] == 'n' || line[0] == 'N') {
            valid = true;
            again = false;
        }
        numTries ++;
    }
    return again;
}

/**
 * @brief prompt_resist
 * I/O for figuring out whether the user wants to see the internal machinations of the program
 * (ie, if she wants to "cheat the cheater")
 * @return
 */
bool prompt_resist() {
    bool resist;
    bool valid = false;
    int numTries = 0;
    string line;
    cout << "Do you want to see a running count of how many words remain? " << endl;
    while (!valid) {
        line = "";
        if (numTries > 0) cout << "Invalid response, please enter yes or no: " << endl;
        getline(cin, line);
        if (line[0] == 'y' || line[0] == 'Y') {
            valid = true;
            resist = true;
        } else if (line[0] == 'n' || line[0] == 'N') {
            valid = true;
            resist = false;
        }
        numTries ++;
    }
    return resist;
}

/**
 * @brief Hangman::Hangman
 * Basic constructor for Hangman class
 * (basically just initializes instance variables)
 * @param numLetters
 * @param numGuesses
 * @param resisting
 */
Hangman::Hangman(const int numLetters, const int numGuesses, const bool resisting) {
    wordLength = numLetters;
    numGuess = numGuesses;
    resist = resisting;
    string s = "";
    for (int i=0; i<wordLength; i++) {
        s += "-";
    }
    reported = s;
}

void Hangman::find_words() {
    ifstream fs("dictionary.txt");
    string line;
    if (fs.is_open()) {
        while (getline(fs, line)) {
            if ((int)line.size() == wordLength) {
                words.push_back(line);
            }
        }
    }
}

void Hangman::print_words() {
    int len = words.size();
    cout << "num_words == " << len << endl;
    for (int i=0; i<len; i++) {
        cout << words[i] << endl;
    }
}

void Hangman::print_info() {
    cout << "resist bool = " << resist << endl;
    cout << "you picked wl of " << wordLength << endl;
    cout << "you picked ng of " << numGuess << endl;
}

ostream& operator<<(ostream& out, vector<string> vs) {
    for (unsigned int i=0; i<vs.size(); i++) {
        out << vs[i] << " ";
    }
    out << endl;
    return out;
}

ostream& operator<<(ostream& out, vector<int> vs) {
    for (unsigned int i=0; i<vs.size(); i++) {
        out << vs[i] << " ";
    }
    out << endl;
    return out;
}

void Hangman::reset() {
    words.clear();
    guessed.clear();
    family.clear();
    families.clear();
}

bool Hangman::has_guessed(const char ch) {

    int len = guessed.size();
    if (len == 0) return false;
    for (unsigned int i=0; i<guessed.size(); i++) {
        if (guessed[i] == ch) return true;
    }
    return false;
}

char Hangman::getGuess() {
    bool valid = false;
    int num_tries = 0;
    cout << "enter your guess: ";
    string line;
    char ch = '\0';
    while (!valid) {
        if (num_tries > 0) cout << "Invalid, please enter a single alphabetical character that you haven't already guessed: ";
        getline(cin, line);
        if (line.size() == 1 && isalpha(line[0])) {
            ch = tolower(line[0]);
            if (!Hangman::has_guessed(ch)) valid = true;
        }
        num_tries ++;
    }
    return ch;
}

void Hangman::print_guessed() {
    cout << "Letters Guessed: ";
    for (unsigned int i=0; i<guessed.size(); i++) {
        cout << guessed[i] << " ";
    }
    cout << endl;
}

/**
 * @brief find_num
 * How many times is a character found in a word?
 * @param str
 * @param ch
 * @return
 */
int find_num(string str, char ch) {
    int n=0;
    for (unsigned int i=0; i<str.size(); i++) {
        if (str[i] == ch) n ++;
    }
    return n;
}

void Hangman::print_families() {
    for (map<string, vector<string> >::iterator it = families.begin(); it != families.end(); ++it) {
        string i = it->first;
        vector<string> vs = it->second;
        cout << i << "=>" << vs.size() << endl;
    }
}

void find_overlap(const string str, const char ch, int &lap, vector<int> &inds) {
    for (unsigned int i=0; i<str.size(); i++) {
        char c = str[i];
        if (c == ch) {
            lap++;
            inds.push_back(i);
        }
    }
}

string make_string(const int numOverlap, const vector<int> indices) {
    string str = "";
    int len = indices.size();
    for (int i=0; i<len; i++) {
        str += to_string(indices[i]);
        if (i < len-1) str += ",";
    }
    string s2 = to_string(numOverlap);
    return s2+":"+str;

}

//borrowed from http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring
void parse_name(const string str, int &numOverlap, vector<int> &indices) {
    numOverlap = stoi(str, 0);
    indices.clear();
    if (str.size() > 2) {
        string s2 = str.substr(2, str.size()-2);
        stringstream ss(s2);
         int i;
         while (ss >> i) {
             indices.push_back(i);
             if (ss.peek() == ',')
                 ss.ignore();
         }
    }
}

void test_parser(){
    int numOverlap = 0;
    vector<int> inds;
    string s1 = "0:";
    parse_name(s1, numOverlap, inds);
    cout << s1 << endl << "num = " << numOverlap << "; " << inds << endl;

    string s2 = "1:2";
    parse_name(s2, numOverlap, inds);
    cout << s2 << endl << "num = " << numOverlap << "; " << inds << endl;

    string s3 = "2:3,4";
    parse_name(s3, numOverlap, inds);
    cout << s3 << endl << "num = " << numOverlap << "; " << inds << endl;

    string s4 = "3:1,3,5";
    parse_name(s4, numOverlap, inds);
    cout << s4 << endl << "num = " << numOverlap << "; " << inds << endl;

    string s5 = "4: 3,1,2,5";
    parse_name(s5, numOverlap, inds);
    cout << s5 << endl << "num = " << numOverlap << "; " << inds << endl;
}

void test_stringer() {
    cout << "testing" << endl;
    int i = 5;
    vector<int> inds;
    inds.push_back(1);
    inds.push_back(2);
    inds.push_back(3);
    inds.push_back(5);
    string s = make_string(i, inds);
    cout << "s len = " << s.size() << endl;
    cout << s << endl;
}

bool vContain(const vector<int> v, const int val) {
    for (unsigned int i=0; i<v.size(); i++) {
        if (v[i] == val) return true;
    }
    return false;
}

void Hangman::find_families(const char ch) {
    families.clear();
    for (string s: words) {
        int numOverlap = 0;
        vector<int> indices;
        indices.clear();
        find_overlap(s, ch, numOverlap, indices);
        string fam_key = make_string(numOverlap, indices);
        vector<string> vs = families[fam_key];
        vs.push_back(s);
        families[fam_key] = vs;
    }
}

void Hangman::find_family() {
    string max_key;
    int max_len = 0;
    for (map<string, vector<string> >::iterator it = families.begin(); it != families.end(); ++it) {
        string i = it->first;
        vector<string> vs = it->second;
        int this_len = vs.size();
        if (this_len > max_len) {
            max_key = i;
            max_len = this_len;
        }
    }
    words = families[max_key];
    familyName = max_key;
    if (resist) cout << "Words Remaining: " << max_len << endl;
}

vector<string> Hangman::getWords() {
    return words;
}

bool Hangman::report(char ch) {
    string curr_report = reported;
    int num;
    vector<int> inds;
    parse_name(familyName, num, inds);
    for (int i=0; i<wordLength; i++) {
        if (vContain(inds, i)) {
            curr_report[i] = ch;
        }
    }
    int cmp = strcmp(curr_report.c_str(), reported.c_str());
    reported = curr_report;
    if (cmp == 0) {
        return false;
    } else {
        return true;
    }
}

bool Hangman::userWon() {
    for (int i=0; i<wordLength; i++) {
        if (reported[i] == '-') return false;
    }
    return true;
}

void Hangman::play_game() {
    int guesses_remaining = numGuess;
    while (guesses_remaining > 0) {
        cout << reported << endl;
        cout << "Mistakes Remaining: " << guesses_remaining << endl;
        char ch = Hangman::getGuess();
        guessed.push_back(ch);
        Hangman::print_guessed();
        Hangman::find_families(ch);
        Hangman::find_family();
        bool b = Hangman::report(ch);
        if (!b) guesses_remaining --;
        if (userWon()) {
            cout << words[0] << endl << "Congratulations, you won!" << endl;
            break;
        }
    }
    if (!userWon()) {
        string randWord = words[0];
        cout << "You lost. The word was " << randWord << endl;
    }
}

int Hangman::num_words() {
    return words.size();
}



int main() {
    int num_games = 0;
    while(true) {
        int num_guess = prompt_ng();
        int word_length = prompt_wl();
        bool resist = prompt_resist();
        Hangman h = Hangman(word_length, num_guess, resist);
        if (num_games > 0) h.reset();
        h.find_words();
        if (h.getWords().size()==0) {
            cout << "oops, there are no words of that length." << endl;
            break;
        }
        if (resist) cout << "Words Remaining: " << h.num_words() << endl;
        h.play_game();
        num_games ++;
        if (!playAgain()) break;
    }
    return 0;
}
