#include "score.hpp"
#include "date.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>

Score::Score()
{
    strcpy(name, "");
    score = -1;
}

Score::Score(const char *n, int s)
{
    Date temp;

    strcpy(name, n);
    score = s;
    date = temp;
}

Score::Score(const char *n, int s, time_t d)
{
    Date temp(d);

    strcpy(name, n);
    score = s;
    date = temp;
}

Score::Score(const char *n, int s, const char *d)
{
    Date temp(d);

    strcpy(name, n);
    score = s;
    date = temp;
}

char* Score::getName()
{
    char *buffer;
    
    strcpy(buffer, name);
    return buffer;
}

bool Score::operator<(const Score& right)
{
    return this->score < right.score;
}

std::ostream& operator<<(std::ostream &out, Score& score_obj)
{
    Date print_date = score_obj.getDateObj();
    std::string name = score_obj.getName();

	out << std::setw(19) << name << std::setw(4)
    << score_obj.getScore();
    out << print_date;
	return out;
}