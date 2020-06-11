#pragma once
#include <iostream>
#include <fstream>
#include "Date.hpp"

class Dividend
{
private:
	Date date;
	float div;
public:
	Dividend(Date d, float di) : date(d), div(di) {}

	Date getDateObj() const
	{ return date;}
	friend std::ostream& operator<<(std::ostream &out, const Dividend& date);
};

bool operator<(const Dividend& left, const Dividend& right)
{
	return (left.getDateObj() < right.getDateObj());
}

std::ostream& operator<<(std::ostream &out, const Dividend& divObj)
{
	out << divObj.date << " " << divObj.div;
	return out;
}