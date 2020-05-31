#pragma once
#include "LinkedList.hpp"
#include "Mystring.hpp"
#include "DuplicateError.hpp"

template <typename T, int NumBuckets>
class Myhash
{
private:
	List<T>* hashtable[NumBuckets];
	int numOfWords;
	int bucketsUsed;
	unsigned hash(unsigned key);
public:
	Myhash();
	~Myhash();
	int size() const;
	double percentOfBucketsUsed();
	double averageNonEmptyBucketSize();
	int largestBucketSize();
	void insert(Mystring);
	bool find(Mystring);
};

template <typename T, int NumBuckets>
Myhash<T, NumBuckets>::Myhash()
{
	numOfWords = 0;
	bucketsUsed = 0;
	for (auto i = 0; i < NumBuckets; i++)
        hashtable[i] = nullptr;
}

template <typename T, int NumBuckets>
Myhash<T, NumBuckets>::~Myhash()
{
	for (auto i = 0; i < NumBuckets; i++)
		if (hashtable[i])
			delete hashtable[i];
}

template <typename T, int NumBuckets>
int Myhash<T, NumBuckets>::size() const
{
    return numOfWords;
}

template <typename T, int NumBuckets>
double Myhash<T, NumBuckets>::percentOfBucketsUsed()
{
	return static_cast<double>(bucketsUsed) / static_cast<double>(NumBuckets);
}

template <typename T, int NumBuckets>
int Myhash<T, NumBuckets>::largestBucketSize()
{
	int largestBS = 0;

	for (auto i = 0; i < NumBuckets; i++)
	{
		if (this->hashtable[i] && this->hashtable[i]->numOfNodes() > largestBS)
			largestBS = this->hashtable[i]->numOfNodes();
	}
	return largestBS;
}

template <typename T, int NumBuckets>
void Myhash<T, NumBuckets>::insert(Mystring buffer)
{
	auto index = hash(buffer);

	if (!hashtable[index])
	{
		hashtable[index] = new List<T>;
		bucketsUsed++;
	}
	else
	{
		if (hashtable[index]->find(buffer))
		{
			DuplicateError<Mystring> error(buffer);
			throw error;
		}
	}
	hashtable[index]->push(buffer);
	numOfWords++;
}

template <typename T, int NumBuckets>
double Myhash<T, NumBuckets>::averageNonEmptyBucketSize()
{
	int sum = 0;
	for (auto i = 0; i < NumBuckets; i++)
		if (hashtable[i])
			sum += hashtable[i]->numOfNodes();
	return static_cast<double>(sum) / static_cast<double>(NumBuckets);
}

template <typename T, int NumBuckets>
bool Myhash<T, NumBuckets>::find(Mystring buffer)
{
	for (auto i = 0; i < NumBuckets; i++)
		if (hashtable[i] && hashtable[i]->find(buffer))
			return true;
	return false;
}

template <typename T, int NumBuckets>
unsigned Myhash<T, NumBuckets>::hash(unsigned key)
{
	int c2 = 0x27d4eb2d; // a prime or an odd constant
	key = (key ^ 61) ^ (key >> 16);
	key = key + (key << 3);
	key = key ^ (key >> 4);
	key = key * c2;
	key = key ^ (key >> 15);
	return key % NumBuckets;
}