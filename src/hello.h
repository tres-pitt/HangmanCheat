
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Hangman{
public:
    Hangman(const int numLetters, const int numGuesses, const bool resisting);
    void find_words();
    void print_words();
    void play_game();
    void reset();
    void print_info();
    int num_words();
    void print_guessed();
    bool has_guessed(const char ch);
    char getGuess();
    void find_families(const char ch);
    void print_families();
    void find_family();
    bool report(char ch);
    vector<string> getWords();
    bool userWon();

private:
    int wordLength;
    int numGuess;
    bool resist;
    vector<string> words;
    vector<char> guessed;
    map<string, vector<string> > families;
    vector<string> family;
    string familyName;
    string reported;
};
