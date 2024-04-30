#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>
#include <string>
#include <algorithm>
#include <thread>
#include <vector>
#include <mutex>
#include <map>

std::unordered_set<std::string> generateAllPermutations(const std::string& str) 
{
  std::unordered_set<std::string> permutations;
  
  if (str.length() == 1) 
  {
    permutations.insert(str);
    return permutations;
  }
  
  for (size_t i = 0; i < str.length(); i++) 
  {
    std::string remaining = str;
    remaining.erase(i, 1);
    std::unordered_set<std::string> partialPermutations = generateAllPermutations(remaining);
    
    for (const std::string& partial : partialPermutations) 
    {
      permutations.insert(str[i] + partial);
      permutations.insert(partial); // Include subsets
    }
  }
  
  permutations.insert(str); // Include original string
  
  return permutations;
}

std::string getLetters()
{
  std::string letters;
  // Prompt for letters
  std::cout << "Enter letters: ";
  std::cin >> letters;
  return letters;
}

std::string getDictionaryFile()
{
  std::string dictionaryFile;
  std::cin >> dictionaryFile;
  return dictionaryFile;
}

std::unordered_set<std::string> readDictionary(std::string location) // No need to pass dictionary as argument
{
  std::unordered_set<std::string> dictionary; // Create a new dictionary here
  std::ifstream dictionaryFile(location);
  if (!dictionaryFile)
  {
    std::cout << "dictionary not found, please enter a filename. ";
    location = getDictionaryFile();
    return readDictionary(location); // Return here
  }
  std::string word;
  while (dictionaryFile >> word)
  {
    dictionary.insert(word);
  }
  dictionaryFile.close();
  return dictionary;
}

void compareWords(const std::unordered_set<std::string>& permutations, const std::unordered_set<std::string>& dictionary, std::mutex& mtx, std::map<int, std::set<std::string>>& categorizedWords)
{
  for (auto permutation : permutations)
  {
    if (dictionary.find(permutation) != dictionary.end())
    {
      std::lock_guard<std::mutex> lock(mtx); // Lock the mutex to ensure thread-safe printing

      // Categorize the word by its length
      categorizedWords[permutation.length()].insert(permutation);
    }
  }
}

int main()
{
  std::unordered_set<std::string> dictionary;
  dictionary = readDictionary("words_alpha.txt");
  std::string letters = getLetters();
  std::unordered_set<std::string> permutations = generateAllPermutations(letters);
  std::cout << "Number of permutations: " << permutations.size() << std::endl;

  std::mutex mtx; // Mutex for thread synchronization
  std::map<int, std::set<std::string>> categorizedWords; // Categorized words by length

  // Split permutations into chunks for parallel processing
  const size_t numThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(numThreads);
  std::vector<std::unordered_set<std::string>> chunkedPermutations(numThreads);

  size_t chunkSize = permutations.size() / numThreads;
  auto it = permutations.begin();
  for (size_t i = 0; i < numThreads; ++i) {
    auto start = it;
    std::advance(it, chunkSize);
    chunkedPermutations[i] = std::unordered_set<std::string>(start, it);
  }

  // Launch threads to compare permutations against the dictionary
  for (size_t i = 0; i < numThreads; ++i) {
    threads[i] = std::thread(compareWords, ref(chunkedPermutations[i]), ref(dictionary), ref(mtx), ref(categorizedWords));
  }

  // Join threads
  for (size_t i = 0; i < numThreads; ++i) {
    threads[i].join();
  }

  int totalWordsFound = 0;
  for (auto entry : categorizedWords)
  {
    int wordsFound = 0;
    std::cout << entry.first << " Letter words;" << std::endl;
    for (const auto& word : entry.second)
    {
      std::cout << word << std::endl;
      wordsFound++;
      totalWordsFound++;
    }
    std::cout << entry.first << " Letter words found; " << wordsFound << std::endl;
    std::cout << std::endl;
  }
  std::cout << "Total words found; " << totalWordsFound << std::endl;
  return 0;
}
