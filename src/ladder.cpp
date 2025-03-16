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

// Prints an error message including the two words involved.
void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

// Computes the edit distance between str1 and str2 and returns true if it is within d.
// This uses a dynamic programming approach with an early exit if the minimum edit distance exceeds d.
bool edit_distance_within(const string& str1, const string& str2, int d) {
    int m = str1.size();
    int n = str2.size();

    // If the difference in lengths is greater than d, then the edit distance cannot be within d.
    if (abs(m - n) > d) return false;

    // Create a DP table with dimensions (m+1) x (n+1)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({ dp[i - 1][j],     // Deletion
                                     dp[i][j - 1],     // Insertion
                                     dp[i - 1][j - 1]  // Substitution
                                   });
            }
        }
        // Early exit: if the minimum edit distance in this row is already greater than d, no need to continue.
        int row_min = *min_element(dp[i].begin(), dp[i].end());
        if (row_min > d) return false;
    }

    return dp[m][n] <= d;
}

// Returns true if word1 and word2 differ by exactly one edit (insertion, deletion, or substitution),
// and are not identical.
bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) return false;
    return edit_distance_within(word1, word2, 1);
}

// Generates a word ladder from begin_word to end_word using a breadth-first search (BFS) approach.
// Each intermediate word (except the begin_word) must be in word_list and no word is reused.
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return vector<string>();
    }

    queue<vector<string>> ladder_queue;
    set<string> visited;

    // Start the ladder with the begin_word.
    vector<string> start;
    start.push_back(begin_word);
    ladder_queue.push(start);
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();

        // Check every word in the dictionary.
        for (const auto& word : word_list) {
            // Only consider words not yet visited and that are adjacent.
            if (visited.find(word) == visited.end() && is_adjacent(last_word, word)) {
                visited.insert(word);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                if (word == end_word) {
                    return new_ladder;
                }
                ladder_queue.push(new_ladder);
            }
        }
    }

    error(begin_word, end_word, "No word ladder found");
    return vector<string>();
}

// Loads words from the given file into word_list.
// All words are converted to lowercase to ensure case-insensitivity.
void load_words(set<string>& word_list, const string& file_name) {
    ifstream infile(file_name);
    if (!infile) {
        cerr << "Unable to open file: " << file_name << endl;
        return;
    }
    string word;
    while (infile >> word) {
        // Convert each word to lowercase.
        for (auto &ch : word) {
            ch = tolower(ch);
        }
        word_list.insert(word);
    }
    infile.close();
}

// Prints a word ladder in a human-readable format.
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); ++i) {
        cout << ladder[i];
        if (i < ladder.size() - 1)
            cout << " -> ";
    }
    cout << endl;
}

// Runs a series of tests on the word ladder functionality.
// Expected ladder lengths are indicated in the assignment writeup.
void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    vector<string> ladder;

    // Test 1: "cat" to "dog" (expected ladder length is 4).
    ladder = generate_word_ladder("cat", "dog", word_list);
    cout << "Test 1 (cat -> dog), ladder length: " << ladder.size() << endl;
    print_word_ladder(ladder);

    // Test 2: "marty" to "curls" (expected ladder length is 6).
    ladder = generate_word_ladder("marty", "curls", word_list);
    cout << "Test 2 (marty -> curls), ladder length: " << ladder.size() << endl;
    print_word_ladder(ladder);

    // Test 3: "code" to "data" (expected ladder length is 6).
    ladder = generate_word_ladder("code", "data", word_list);
    cout << "Test 3 (code -> data), ladder length: " << ladder.size() << endl;
    print_word_ladder(ladder);

    // Test 4: "work" to "play" (expected ladder length is 6).
    ladder = generate_word_ladder("work", "play", word_list);
    cout << "Test 4 (work -> play), ladder length: " << ladder.size() << endl;
    print_word_ladder(ladder);

    // Test 5: "sleep" to "awake" (expected ladder length is 8).
    ladder = generate_word_ladder("sleep", "awake", word_list);
    cout << "Test 5 (sleep -> awake), ladder length: " << ladder.size() << endl;
    print_word_ladder(ladder);

    // Test 6: "car" to "cheat" (expected ladder length is 4).
    ladder = generate_word_ladder("car", "cheat", word_list);
    cout << "Test 6 (car -> cheat), ladder length: " << ladder.size() << endl;
    print_word_ladder(ladder);
}
