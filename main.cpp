#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <algorithm>

std::unordered_set<std::string> generate_permutations(const std::string& str) {
    std::unordered_set<std::string> perms;
    for (std::size_t i = 1; i <= str.length(); i++) {
        std::string substring = str.substr(0, i);
        std::sort(substring.begin(), substring.end());
        do {
            perms.insert(substring);
        } while (std::next_permutation(substring.begin(), substring.end()));
    }
    return perms;
}

int main() {
    std::unordered_set<std::string> dict;

    // Read in dictionary of words from file
    std::ifstream infile("words_alpha.txt");
    if (!infile) {
        std::cout << "Error: Could not open dictionary file.\n";
        return 1;
    }
    std::string word;
    while (infile >> word) {
        dict.insert(word);
    }
    infile.close();

    // Prompt user to input letters
    std::cout << "Enter up to 6 letters: ";
    std::string letters;
    std::cin >> letters;

    // Check for valid input
    if (letters.length() > 6) {
        std::cout << "Error: Too many letters entered.\n";
        return 1;
    }

    // Generate all possible permutations of letters
    std::unordered_set<std::string> perms = generate_permutations(letters);

    // Search for words in dictionary that match permutations of letters
    std::cout << "Matching words:\n";
    for (const auto& word : dict) {
        for (const auto& perm : perms) {
          if (word == perm) {
                std::cout << word << std::endl;
                break;
            }
        }
    }

    return 0;
}
