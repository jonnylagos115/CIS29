#pragma once
#include <stdexcept>
#include <iostream>
#include "Mystring.hpp"
#include "Myhash.hpp"

template <typename T>
class DuplicateError : public std::logic_error
{
public:
    DuplicateError(T message) : std::logic_error(message)
    {
        std::cout << "Duplicate Mystring: ";
    }
};

