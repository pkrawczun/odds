#ifndef MISCELLANEOUS_HPP
#define MISCELLANEOUS_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>
#endif

#include "PqxxConnection.hpp"

enum class Outcome {H=0, D=1, A=2};


// a temp function // fails for leap years
std::string SubtractOneYear(const std::string& date); 

// a temp function
std::string SubtractOneDay(const std::string& today);

// a temp function
std::string AddOneDay(const std::string& today);

#endif
