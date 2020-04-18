#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const int MaxWordSize = 45;

class Word
{
   private:
      char *word_;
   public:
      Word(const char* text = 0)
      {
         word_ = new char[strlen(text) + 1];
         strcpy(word_, text);
         if (isupper(word_[0]))
            word_[0] += 32; //Converts dictionary word from upper-case to lower-case
      }
      ~Word() //Destructor is invoked when 'delete' keyword is used explicitly to free dynamically allocated object
      { delete[] word_; } //Free allocated char * array memory within this context
      const char* getWord() const
      { return word_; }
};

class Dictionary
{
   private:
      Word **words_;
      unsigned int capacity_; // max number of words Dictionary can hold
      unsigned int numberOfWordsInDictionary_;
      void resize();
      void addWordToDictionary(char *word);
   public:
      Dictionary(const char* filename);
      ~Dictionary();
      bool find(const char* word);
};

Dictionary::Dictionary(const char* filename)
{
   ifstream fin(filename);
   char buffer[MaxWordSize];
   char *line;

   if (!fin)
   {
      cerr << "Error occurred in attempt to open input file\n";
      exit(1);
   }
   capacity_ = 8;
   numberOfWordsInDictionary_ = 0;
   words_ = new Word*[capacity_];
   while (fin.getline(buffer, MaxWordSize))
   {
      line = buffer;
      line = strtok(line, "\r");
      addWordToDictionary(line);
   }
   fin.close();
}

Dictionary::~Dictionary()
{
   for (unsigned int i = 0; i < numberOfWordsInDictionary_; i++)
      delete words_[i]; //Free each dynamically allocated 'Word' object
   delete[] words_; //Free array of pointer to objects
}

void Dictionary::addWordToDictionary(char *word)
{
   int index = numberOfWordsInDictionary_;

   if (numberOfWordsInDictionary_ > capacity_)
      resize();
   words_[index] = new Word(word);
   numberOfWordsInDictionary_++;
}

void Dictionary::resize()
{
   Word **temp = new Word*[capacity_];

   for (unsigned int i = 0; i <= capacity_; i++)
      temp[i] = words_[i];
   delete[] words_; //delete the array of pointers
   capacity_ *= 2; //double the array size
   words_ = new Word*[capacity_];
   for (unsigned int i = 0; i <= capacity_ / 2; i++)
      words_[i] = temp[i];
   delete[] temp;
   cout << "Dictionary resized to capacity: " << capacity_ << endl;
}

bool Dictionary::find(const char *word)
{
   int first = 0, last = numberOfWordsInDictionary_, middle;
   bool flag = false;

   while (!flag && first <= last)
   {
      middle = (first + last) / 2;
      if (!strcmp(words_[middle]->getWord(), word))
         flag = true;
      else if (strcmp(words_[middle]->getWord(), word) > 0)
         last = middle - 1;
      else
         first = middle + 1;
   }
   return flag;
}

bool cleanupWord(char *buffer)
{
   bool flag = true;
   int i, buf_size = strlen(buffer);

   if (isupper(buffer[0])) //Converts any leading letter of word from upper-case to lower-case
      buffer[0] += 32;
   if (!isalpha(buffer[buf_size - 1])) //Checks then removes any trailing punctuation
   {
      for (i = buf_size; !isalpha(buffer[i]) && i > 0; i--) {}
      if (i > 0)
         buffer[i + 1] = '\0';
      else
         flag = false;
   }
   return flag; //Returns false if buffer contains no alphabets in which no attempt to search is made
}

int main()
{
   char buffer[MaxWordSize];
   Dictionary Websters("words");
   ifstream fin("gettysburg.txt");
   if (!fin)
   {
      cerr << "Error occurred in attempt to open input file\n";
      exit(1);
   }
   cout << "\nSpell checking gettysburg.txt " << "\n\n";
   while (fin >> buffer) {
      // remove leading/trailing punctuation, change to lowercase
      if (cleanupWord(buffer)) {
         if (!Websters.find(buffer)) {
            cout << buffer << " not found in the Dictionary\n";
         }
      }
   }
   return 0;
}