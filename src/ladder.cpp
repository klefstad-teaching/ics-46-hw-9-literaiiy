#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    int m = str1.size();
    int n = str2.size();

    if (abs(m - n) > d) return false;

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
        }
        int row_min = *min_element(dp[i].begin(), dp[i].end());
        if (row_min > d) return false;
    }
    return dp[m][n] <= d;
}

// Updated: identical words are now considered adjacent (0 edit distance is within 1)
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

// Revised generate_word_ladder uses a modifiable copy of word_list to remove words as they are used.
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& original_word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return vector<string>();
    }

    // Make a copy of the dictionary so we can remove words as they’re used.
    set<string> remaining = original_word_list;
    // If the begin word is in the dictionary, remove it.
    remaining.erase(begin_word);

    queue<vector<string>> ladder_queue;
    vector<string> start = {begin_word};
    ladder_queue.push(start);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();

        // To avoid iterating over the entire set repeatedly, collect words to remove.
        vector<string> to_remove;
        for (const string &word : remaining) {
            if (is_adjacent(last_word, word)) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                if (word == end_word) {
                    return new_ladder;
                }
                ladder_queue.push(new_ladder);
                to_remove.push_back(word);
            }
        }
        // Remove all words that have been used in this iteration.
        for (const string &word : to_remove) {
            remaining.erase(word);
        }
    }
    error(begin_word, end_word, "No word ladder found");
    return vector<string>();
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream infile(file_name);
    if (!infile) {
        cerr << "Unable to open file: " << file_name << endl;
        return;
    }
    string word;
    while (infile >> word) {
        for (auto &ch : word) {
            ch = tolower(ch);
        }
        word_list.insert(word);
    }
    infile.close();
}

// Updated print_word_ladder: print with the prefix and words separated by a space.
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << "\n";
        return;
    }
    cout << "Word ladder found: ";
    for (const string &word : ladder) {
        cout << word << " ";
    }
    cout << "\n";
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    vector<string> ladder;

    // Test 1: "cat" to "dog"
    ladder = generate_word_ladder("cat", "dog", word_list);
    cout << "Test 1 (cat -> dog), ladder length: " << ladder.size() << "\n";
    print_word_ladder(ladder);

    // Test 2: "marty" to "curls"
    ladder = generate_word_ladder("marty", "curls", word_list);
    cout << "Test 2 (marty -> curls), ladder length: " << ladder.size() << "\n";
    print_word_ladder(ladder);

    // Test 3: "code" to "data"
    ladder = generate_word_ladder("code", "data", word_list);
    cout << "Test 3 (code -> data), ladder length: " << ladder.size() << "\n";
    print_word_ladder(ladder);

    // Test 4: "work" to "play"
    ladder = generate_word_ladder("work", "play", word_list);
    cout << "Test 4 (work -> play), ladder length: " << ladder.size() << "\n";
    print_word_ladder(ladder);

    // Test 5: "sleep" to "awake"
    ladder = generate_word_ladder("sleep", "awake", word_list);
    cout << "Test 5 (sleep -> awake), ladder length: " << ladder.size() << "\n";
    print_word_ladder(ladder);

    // Test 6: "car" to "cheat"
    ladder = generate_word_ladder("car", "cheat", word_list);
    cout << "Test 6 (car -> cheat), ladder length: " << ladder.size() << "\n";
    print_word_ladder(ladder);
}
