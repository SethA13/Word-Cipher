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

using namespace std;

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

string getLetters()
{
  string letters;
  // Prompt for letters
  cout << "Enter letters: ";
  cin >> letters;
  return letters;
}

string getDictionaryFile()
{
  string dictionaryFile;
  cin >> dictionaryFile;
  return dictionaryFile;
}

unordered_set<string> readDictionary(string location) // No need to pass dictionary as argument
{
  unordered_set<string> dictionary; // Create a new dictionary here
  ifstream dictionaryFile(location);
  if (!dictionaryFile)
  {
    cout << "dictionary not found, please enter a filename. ";
    location = getDictionaryFile();
    return readDictionary(location); // Return here
  }
  string word;
  while (dictionaryFile >> word)
  {
    dictionary.insert(word);
  }
  dictionaryFile.close();
  return dictionary;
}

void compareWords(const unordered_set<string>& permutations, const unordered_set<string>& dictionary, mutex& mtx, map<int, vector<string>>& categorizedWords)
{
  for (auto permutation : permutations)
  {
    if (dictionary.find(permutation) != dictionary.end())
    {
      lock_guard<mutex> lock(mtx); // Lock the mutex to ensure thread-safe printing

      // Categorize the word by its length
      categorizedWords[permutation.length()].push_back(permutation);
    }
  }
}

int main()
{
  unordered_set<string> dictionary;
  dictionary = readDictionary("words_alpha.txt");
  string letters = getLetters();
  unordered_set<string> permutations = generateAllPermutations(letters);
  cout << "Number of permutations: " << permutations.size() << endl;

  mutex mtx; // Mutex for thread synchronization
  map<int, vector<string>> categorizedWords; // Categorized words by length

  // Split permutations into chunks for parallel processing
  const size_t numThreads = thread::hardware_concurrency();
  vector
  <thread> threads(numThreads);
  vector
  <unordered_set<string>> chunkedPermutations(numThreads);

  size_t chunkSize = permutations.size() / numThreads;
  auto it = permutations.begin();
  for (size_t i = 0; i < numThreads; ++i) {
    auto start = it;
    std::advance(it, chunkSize);
    chunkedPermutations[i] = unordered_set<string>(start, it);
  }

  // Launch threads to compare permutations against the dictionary
  for (size_t i = 0; i < numThreads; ++i) {
    threads[i] = thread(compareWords, ref(chunkedPermutations[i]), ref(dictionary), ref(mtx), ref(categorizedWords));
  }

  // Join threads
  for (size_t i = 0; i < numThreads; ++i) {
    threads[i].join();
  }
  set<string> oneLetterWords;
  set<string> twoLetterWords;
  set<string> threeLetterWords;
  set<string> fourLetterWords;
  set<string> fiveLetterWords;
  set<string> sixLetterWords;
  set<string> allOthers;

  for (auto entry : categorizedWords)
  {
    for (const auto& word : entry.second)
    {
      switch (entry.first)
      {
      case 1:
        oneLetterWords.insert(word);
        break;
      case 2:
        twoLetterWords.insert(word);
        break;
      case 3:
        threeLetterWords.insert(word);
        break;
      case 4:
        fourLetterWords.insert(word);
        break;
      case 5:
        fiveLetterWords.insert(word);
        break;
      case 6:
        sixLetterWords.insert(word);
        break;
      
      default:
        allOthers.insert(word);
        break;
      }
    }
  }
  
  cout << "One letter words: " << endl;
  for (auto word : oneLetterWords)
  {
    cout << word << endl;
  }
  cout << endl;

  cout << "Two letter words: " << endl;
  for (auto word : twoLetterWords)
  {
    cout << word << endl;
  }
  cout << endl;

  cout << "Three letter words: " << endl;
  for (auto word : threeLetterWords)
  {
    cout << word << endl;
  }
  cout << endl;

  cout << "Four letter words: " << endl;
  for (auto word : fourLetterWords)
  {
    cout << word << endl;
  }
  cout << endl;

  cout << "Five letter words: " << endl;
  for (auto word : fiveLetterWords)
  {
    cout << word << endl;
  }
  cout << endl;

  cout << "Six letter words: " << endl;
  for (auto word : sixLetterWords)
  {
    cout << word << endl;
  }
  cout << endl;

  cout << "All others: " << endl;
  for (auto word : allOthers)
  {
    cout << word << endl;
  }
  cout << endl;

  return 0;
}
