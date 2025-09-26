#ifndef MISCELLANEOUS_HPP
#define MISCELLANEOUS_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>
#include <chrono>
#endif

#include "PqxxConnection.hpp"

enum class Outcome {H=0, D=1, A=2};

//std::chrono::year_month_day StrToYmd(const std::string&);
//std::string YmdToStr(const std::chrono::year_month_day&);
std::string AddDays(const std::string&, int);
std::string AddYears(const std::string&, int);
std::string SubtractOneYear(const std::string&); 
std::string SubtractOneDay(const std::string&);
std::string AddOneDay(const std::string&);

#endif
