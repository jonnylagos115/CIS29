#include "Mystring.hpp"

Mystring::operator unsigned()
{
    auto sum = 0u;

    for (auto i = 0u; i < this->length(); i++)
        sum += static_cast<unsigned>(this->at(i));
    return sum;
}

void Mystring::removePunctuation()
{
    int i, size = this->length();
    if (!isalpha(this->at(size - 1))) //Checks then removes any trailing punctuation
    {
        for (i = size - 1; !isalpha(this->at(i)) && i > 0; i--) {}
        if (i > 0)
			*this = this->substr(0, i+1);
        if (!i) //Not considered to be a word
            *this = this->substr(0,0);
    }
}

void Mystring::tolower()
{
	for (auto i = 0u; i < this->length(); i++)
		if (std::isupper(this->at(i)))
			this->at(i) = std::tolower(this->at(i));
}