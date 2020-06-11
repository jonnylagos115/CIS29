#include <iostream>
#include <fstream>
#include <unordered_set>
#include "MutualFund.hpp"
using namespace std; 

void shuffleNavHistoryFile(string NAV_filename);

int main() 
{
	// Part 1
	string ticker1 = "VDADX", ticker2 = "VBIAX";
	shuffleNavHistoryFile(ticker1);
	shuffleNavHistoryFile(ticker2);

	// Part 2
    MutualFund vdadx(ticker1);
    MutualFund vbiax(ticker2);
    vdadx.report();
    vbiax.report();

    // Part 3
    //analyze2Funds(vdadx, vbiax);

	return 0;
}

void shuffleNavHistoryFile(string NAV_filename)
{
	unordered_set <string> inputDataSet;
	unordered_set<string> :: iterator itr;
	ifstream fin(NAV_filename + ".csv");
	ofstream fout(NAV_filename + "shuffled.csv");
	string buffer;

	if (!fin || !fout)
	{
        cerr <<  NAV_filename << ".csv history file cannot be opened" << endl;
        exit(1);
    }
	cout << "Reading " << NAV_filename << ".csv into an unordered_set<string> ..." << endl;
	getline(fin, buffer); //Skip over header name row
	while (getline(fin, buffer))
		inputDataSet.insert(buffer);
	cout << "Input data stored in " << inputDataSet.bucket_count() << " buckets" << endl;
	cout << "Writing unordered_set<string> data into file " << NAV_filename << "shuffled.csv" << endl;
	for (itr = inputDataSet.begin(); itr != inputDataSet.end(); itr++)
		fout << *itr << endl;
	cout << endl;
	fin.close();
	fout.close();
}