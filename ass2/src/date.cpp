#include "date.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

Date::Date()
{
	auto curr = std::chrono::system_clock::now();
	date = std::chrono::system_clock::to_time_t(curr);
}

Date:: Date(const char* d)
{
	int yy, month, dd;
	date = 0;
	struct tm tm = *localtime(&date);

	if (strchr(d, '/'))
		sscanf(d, "%d/%d/%d", &month, &dd, &yy);
	else if (strchr(d, '-'))
		sscanf(d, "%d-%d-%d", &month, &dd, &yy);
	else
	{
		char cstr_month[8];
		sscanf(d, "%d%s", &dd, cstr_month);
		month = convertMonthToInt(cstr_month);
		yy = atoi(cstr_month+3) + 2000;
	}
	tm.tm_year = yy - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = dd;

	date = mktime(&tm);
}

int Date::convertMonthToInt(char *cstr_month)
{
	const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", 
	"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	int mm;
	char mon[3];

	strncpy(mon, cstr_month, 3);
	for (auto i = 0; i < 12; i++)
	{
		if (strcmp(mon, months[i]) == 0)
		{
			mm = i + 1;
			break ;
		}
	}
	return mm;
}

std::ostream& operator<<(std::ostream &out, Date& date)
{
	std::time_t print_date = date.getDate();
	std::tm *ptm = localtime(&print_date);
	char buffer[32];

	std::strftime(buffer, 32, "%D", ptm);
	out << buffer;
	return out;
}
