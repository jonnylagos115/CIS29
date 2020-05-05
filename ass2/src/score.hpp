#ifndef SCORE_HPP
#define SCORE_HPP
#include "date.hpp"

class Score
{
    private:
        char name[16];
        int score;
        Date date;
    public:
        // Constructors
        Score();
        Score(const char *n, int s);
        Score(const char *n, int s, time_t d);
        Score(const char *n, int s, const char *d);

        ~Score() {};
        // Accessor functions
        char* getName();
        int getScore() { return score;}
        Date getDateObj() { return date;}

        // Overloaded operators
        bool operator<(const Score& right);
        friend std::ostream& operator<<(std::ostream &out, Score& score);
};

#endif