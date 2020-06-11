#pragma once
#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <string>
#include "Date.hpp"
#include "Dividend.hpp"

class MutualFund
{
private:
	std::string ticker;
	Date oldestDate;
	Date newestDate;
	float newestDat;
	float newestNav;
	std::map<Date, float> navHistory;
	std::set<Dividend> divHistory;
public:
	MutualFund(std::string t) : ticker(t) {}
	void report();
};

void MutualFund::report()
{
	std::ifstream NAV_hist_fin(ticker + "shuffled.csv");
	std::ifstream NAV_div_fin(ticker + "div.csv");
	std::ofstream NAV_hist_fout(ticker + "nav.txt");
	std::ofstream NAV_div_fout(ticker + "div.txt");
	std::string buffer;
	std::map <Date, float>::iterator nav;
	std::set<Dividend>::iterator div;

	if (!NAV_hist_fin || !NAV_div_fin || !NAV_hist_fout || !NAV_div_fout)
	{
		std::cerr << ticker << " file cannot be opened" << std::endl;
        exit(1);
	}
	std::cout << "Reading file " << ticker << "shuffled.csv into map<Date,float> ..." << std::endl;
	while (getline(NAV_hist_fin, buffer, ','))
	{
		Date date_NAV_hist(buffer);

		for (auto i = 0; i < 3; i++) { getline(NAV_hist_fin, buffer, ',');}
		navHistory.insert(std::pair<Date, float>(date_NAV_hist, std::stof(buffer)));
		getline(NAV_hist_fin, buffer);
	}
	std::cout << navHistory.size() << " records read\n" << std::endl;

	std::cout << "Reading file " << ticker << "div.csv into set<Dividend> ..." << std::endl;
	getline(NAV_div_fin, buffer); // Skip over header names
	while (getline(NAV_div_fin, buffer, ','))
	{
		Date date_div(buffer);
		getline(NAV_div_fin, buffer);
		divHistory.insert(Dividend(date_div, std::stof(buffer)));
	}
	std::cout << divHistory.size() << " records read\n" << std::endl;

	NAV_hist_fout << std::fixed;
	NAV_hist_fout << std::setprecision(2);
	NAV_div_fout << std::fixed;
	NAV_div_fout << std::setprecision(3);
	std::cout << "Writing NAV history data into file " << ticker << "nav.txt ... ";
	for (nav = navHistory.begin(); nav != navHistory.end(); ++nav)
		NAV_hist_fout << nav->first << std::setw(10) << nav->second << std::endl;
	std::cout << "done" << std::endl;
	std::cout << "Writing dividend history data into file " << ticker << "div.txt ... ";
	for (div = divHistory.begin(); div != divHistory.end(); ++div)
		NAV_div_fout << *div << std::endl;
	std::cout << "done\n" << std::endl;

	std::cout << "Fund: " << ticker << std::endl;
	std::cout << "Analysis Date: " << navHistory.end()->first << std::endl;
	std::cout << "Latest NAV: ";
	std::cout << "Minimum for the last year = $";
	std::cout << "Maximum for the last year = $";
	std::cout << "50 day moving average = $";
	std::cout << "200 day moving averaege= $";
	std::cout << "% gain over the last 200 Day Moving Avg: ";
	std::cout << "% return over the last 3 years: ";
	
	NAV_hist_fin.close();
	NAV_div_fin.close();
	NAV_hist_fout.close();
	NAV_div_fout.close();
}

std::string convert_to_string(std::time_t print_date)
{
	std::tm *ptm = localtime(&print_date);
	char buffer[32];

	std::strftime(buffer, 32, "%D", ptm);
	return buffer;
}
