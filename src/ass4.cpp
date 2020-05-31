/*
** Jonathan Lagos
** CIS29
** Assignment #4
** GNU C++ Compiler on Linux
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Myhash.hpp"
#include "Mystring.hpp"
#include "LinkedList.hpp"
#include "DuplicateError.hpp"
using namespace std;

int main()
{
    Myhash<Mystring, 1500> Dictionary;
    Mystring buffer;

    const string DictionaryFileName = "ass4words";
    const string DocumentFileName = "roosevelt_first_inaugural.txt";

    ifstream fin(DictionaryFileName.c_str());
    if (!fin)
    {
        cerr << "Can't find " << DictionaryFileName << endl;
        exit(-1);
    }
    while (getline(fin, buffer))
    {
        // remove \r if present (this for Mac/Linux)
        if (buffer[buffer.size()-1] == '\r')
            buffer.resize(buffer.size() - 1);
        buffer.tolower();
        try
        {
            Dictionary.insert(buffer);
        }
        catch (const DuplicateError<Mystring>& error)
        {
            cout << error.what() << endl;
        }
    }
    cout << "Number of words in the dictionary = " << Dictionary.size() << endl;
    cout << "Percent of hash table buckets used = " << setprecision(2) << fixed << 100 * Dictionary.percentOfBucketsUsed() << '%' << endl;
    cout << "Average non-empty bucket size = " << Dictionary.averageNonEmptyBucketSize() << endl;
    cout << "Largest bucket size = " << Dictionary.largestBucketSize() << endl;
    fin.close();
    fin.clear();

    // Spellcheck
    unsigned misspelledWords = 0;

    fin.open(DocumentFileName.c_str());
    if (!fin)
    {
        cerr << "Can't find " << DocumentFileName << endl;
        exit(-1);
    }
    while (fin >> buffer)
    {
        buffer.tolower();
        buffer.removePunctuation();
        if (!buffer.size())
            continue;
        if (!Dictionary.find(buffer))
        {
            misspelledWords++;
            cout << "Not found in the dictionary: " << buffer << endl;
        }
    }
    cout << "Total mispelled words = " << misspelledWords << endl;
	return 0;
}