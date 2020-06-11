#pragma once
#include <iomanip>
#include <ctime>
#include <sstream>

class Date
{
private:
	std::time_t date_t;
public:
	Date() : date_t(0){}
	Date(std::string d) : date_t(convert_to_time_t(d)) {}

	//conversion function from string to time_t
	std::time_t convert_to_time_t(std::string d);

	//Setter
	void setDate(std::string d)
	{ date_t = convert_to_time_t(d);}
	//Getter
	std::time_t getDate() const
	{ return date_t;}

	friend std::ostream& operator<<(std::ostream &out, Date& date);
};

std::time_t Date::convert_to_time_t(std::string d)
{
	int yy, month, dd;
	std::time_t t = 0;
	struct tm *tm = localtime(&t);

	sscanf(d.c_str(), "%d-%d-%d", &yy, &month, &dd);
	tm->tm_year = yy - 1900;
	tm->tm_mon = month - 1;
	tm->tm_mday = dd;
	t = mktime(tm);
	return t;
}

bool operator<(const Date& left, const Date& right)
{
	return (left.getDate() < right.getDate());
}

std::ostream& operator<<(std::ostream &out, const Date& date)
{
	std::time_t print_date = date.getDate();
	std::tm *ptm = localtime(&print_date);
	char buffer[32];

	std::strftime(buffer, 32, "%m/%d/%Y", ptm);
	out << buffer;
	return out;

}