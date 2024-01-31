#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>
#include <string>
#include <algorithm>

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

unordered_set<string> readDictionary(string location, unordered_set<string> dictionary)
{
  ifstream dictionaryFile(location);
  if (!dictionaryFile)
  {
    cout << "dictionary not found, please enter a filename. ";
    location = getDictionaryFile();
    readDictionary(location, dictionary);
  }
  string word;
  while (dictionaryFile >> word)
  {
    dictionary.insert(word);
  }
  dictionaryFile.close();
  return dictionary;
}

void compareWords()
{
  return;
}

void findLargestLength()
{
  return;
}



int main()
{
  unordered_set<string> dictionary;
  set<int> wordLengths;
  dictionary = readDictionary("words_alpha.txt", dictionary);
  string letters = getLetters();
  unordered_set<string> permutations = generateAllPermutations(letters);
  cout << "Number of permutations; " << permutations.size() << endl;

  set<string> allWords;
  for (auto permutation : permutations)
  {
    for (auto word : dictionary)
    {
      if (permutation == word)
      {
        allWords.insert(permutation);
      } 
    }
  }

  set<string> oneLetterWords;
  set<string> twoLetterWords;
  set<string> threeLetterWords;
  set<string> fourLetterWords;
  set<string> fiveLetterWords;
  set<string> sixLetterWords;
  set<string> allOthers;

  for (auto word : allWords)
  {
    switch (word.length())
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
