#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>

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

// With this implementation, two identical words are considered adjacent 
// because 0 edit distance is within 1.
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

/*
  Helper function: get_neighbors
  Generates all candidate words that are one edit away (by substitution, insertion, or deletion)
  from the input word.
*/
vector<string> get_neighbors(const string &word) {
    vector<string> neighbors;
    string candidate;
    int len = word.size();
    
    // Substitution: for each position, try each letter from 'a' to 'z'
    for (int i = 0; i < len; i++) {
        candidate = word;
        for (char c = 'a'; c <= 'z'; c++) {
            if (candidate[i] == c) continue;
            candidate[i] = c;
            neighbors.push_back(candidate);
        }
    }
    
    // Insertion: for each position from 0 to len, insert a letter
    for (int i = 0; i <= len; i++) {
        for (char c = 'a'; c <= 'z'; c++) {
            candidate = word;
            candidate.insert(candidate.begin() + i, c);
            neighbors.push_back(candidate);
        }
    }
    
    // Deletion: for each position, remove that letter (if word is non-empty)
    if (len > 0) {
        for (int i = 0; i < len; i++) {
            candidate = word;
            candidate.erase(candidate.begin() + i);
            neighbors.push_back(candidate);
        }
    }
    
    return neighbors;
}

/*
  Revised generate_word_ladder:
  We partition the dictionary by word length, then use the get_neighbors helper to generate
  all candidate words one edit away from the current word. For each candidate, we check whether 
  it exists in our dictionary partition. If so, we enqueue a new ladder with that candidate and 
  immediately remove it from the dictionary.
*/
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& original_word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return vector<string>();
    }
    
    // Partition the dictionary by word length.
    map<int, set<string>> remaining_by_length;
    for (const auto &word : original_word_list) {
        remaining_by_length[word.size()].insert(word);
    }
    // Remove the begin word from the appropriate group if present.
    remaining_by_length[begin_word.size()].erase(begin_word);
    
    queue<vector<string>> ladder_queue;
    vector<string> start = {begin_word};
    ladder_queue.push(start);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        
        // Generate all candidate neighbors (one edit away).
        vector<string> candidates = get_neighbors(last_word);
        for (const string &candidate : candidates) {
            int candidate_len = candidate.size();
            // Check if candidate exists in our partition for its length.
            if (remaining_by_length.count(candidate_len) && remaining_by_length[candidate_len].find(candidate) != remaining_by_length[candidate_len].end()) {
                // Candidate is valid; remove it to prevent reuse.
                remaining_by_length[candidate_len].erase(candidate);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(candidate);
                if (candidate == end_word) {
                    return new_ladder;
                }
                ladder_queue.push(new_ladder);
            }
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

// Print with the prefix "Word ladder found: " and words separated by a space.
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found.\n";
        return;
    }
    cout << "Word ladder found: ";
    for (const string &word : ladder) {
        cout << word << " ";
    }
    cout << "\n";
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}
