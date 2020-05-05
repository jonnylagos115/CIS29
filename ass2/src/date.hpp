#ifndef DATE_HPP
#define DATE_HPP
#include <ctime>
#include <fstream>
#include <chrono>

class Date
{
    private:
        std::time_t date;
    public:
        // Constructors
        Date();
        Date(const Date&) = default;
        Date(std::time_t t) {date = t;}
        Date(const char* d);

        void setDate(std::time_t d)
        { date = d;}
        std::time_t getDate()
        { return date;}

        // Other functions
        int convertMonthToInt(char *cstr_month);

        // Overloaded operators
        friend std::ostream& operator<<(std::ostream &out, Date& date);
};
#endif