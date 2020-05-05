#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "date.hpp"
#include "score.hpp"
using namespace std;

const int MaxNumberOfScores = 10;

string trim(string str, string whitespace)
{
    const int strBegin = 0;

    const int strEnd = str.find_last_not_of(whitespace);
    const int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void eraseScoresFile(string scoresfile)
{
	ofstream sf;

	sf.open(scoresfile, ofstream::out | ofstream::trunc);
	sf.close();
}

bool processInputFile(ifstream& fin, Score* &newScore)
{
    string name, str_date, line;
    int score;
    time_t date;

    if (fin.eof())
        return false;
    if (getline(fin, line))
    {
     	name = trim(line.substr(0, 16), " "); //Get name then trim any trailing spaces
		line = line.substr(16);
		str_date = trim(line.substr(0, 13), " ");
		line = line.substr(13);
		score = stoi(line);
    }
	if (str_date.length() == 10) //This means that date format is in time_h type
    {
		date = stoi(str_date);
		newScore = new Score(name.c_str(), score, date);
	}
	else if (str_date.length())
		newScore = new Score(name.c_str(), score, str_date.c_str());
	else
		newScore = new Score(name.c_str(), score);
    return true;
}

unsigned int getScoresFromFile(string ScoresFile)
{
	unsigned int numScores = 0;
	string line;
	ifstream scores_file(ScoresFile, ios::binary);

	if (!scores_file)
	{
	   cerr << "Unable to open input file Scoresfile ifstream" << endl;
	   exit(1);
	}
	while (getline(scores_file, line) && numScores < 10)
		if (line.at(0) == '-')
			numScores++;
	scores_file.close();
	return numScores;
}

void sort(Score *scores, unsigned int numScores)
{
    Score temp;
    unsigned int curr;
    int walk;

    for (curr = 1; curr < numScores; curr++)
    {
        // make a copy of the current element
        temp = scores[curr];

        // shift elements in the sorted part of the list to make room
        walk = curr - 1;
        while( walk >= 0 && scores[walk] < temp)
        {
            scores[walk + 1] = scores[walk];
            walk--;
        }
        // put the copy of the current element back into the list
        scores[walk + 1] = temp;
    }// end of for: move to the next element to be inserted
}

void writeScoresToFile(string ScoresFile, Score *scores, unsigned int newScores)
{
    Date write_date;
    ofstream scores_file(ScoresFile, ios::binary | ios::app); //ios::app is the open mode "append" meaning new data will be written at end of file
    string name;

    if (!scores_file)
    {
        cerr << "Unable to open input file Scoresfile ostream" << endl;
        exit(1);
    }
    for (unsigned int i = 0; i < newScores; i++)
    {
		write_date = scores[i].getDateObj();
		name = scores[i].getName();
    	scores_file << left << setw(3) << i + 1
    	<< setw(19) << name << setw(4) << scores[i].getScore();
    	scores_file << write_date << endl;
    }
	scores_file << "----------------------------------" << endl;
    scores_file.close();
}

int main()
{
    Score scores[MaxNumberOfScores];
    string ScoresFile = "scoresfile";
    ifstream fin("ass2data.txt");
    if (!fin)
    {
        cerr << "Unable to open input file ass2data.txt" << endl;
        exit(1);
    }
    Score *newScore = nullptr;
    unsigned int numScores;
    bool updateScores = false;
    eraseScoresFile(ScoresFile); // erases contents in binary file

    while (processInputFile(fin, newScore)) // read a record from input file
    {
        // read binary Scores file
        numScores = getScoresFromFile(ScoresFile);
        if (numScores < MaxNumberOfScores)
        {
            scores[numScores++] = *newScore;
            updateScores = true;
        }
        else if (scores[numScores - 1] < *newScore) // newScore > lowest of Top Scores
        {
            scores[numScores - 1] = *newScore;
            updateScores = true;
        }
        else
            updateScores = false;
        if (updateScores)
        {
            sort(scores, numScores);
            for (auto i = 0U; i < numScores; ++i)
                cout << left << setw(3) << i+1 << scores[i] << endl;
            cout << "----------------------------------\n";
			writeScoresToFile(ScoresFile, scores, numScores);
        }
		delete newScore;
		newScore = nullptr;
    }
    fin.close();
    return 0;
}