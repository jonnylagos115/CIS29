#pragma once
#include <string>
#include <cctype>
#include <iostream>

class Mystring : public std::string
{
public:
	Mystring() {}
	Mystring(std::string d) : std::string(d) {}
	operator unsigned();
	void removePunctuation();
	void tolower();
};