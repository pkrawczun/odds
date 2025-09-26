#include <chrono> // chrono isn't very good at leap years, catch exceptions when querying the database
#include <iostream>
#include <string>
#include <sstream>

#include "Miscellaneous.hpp"

//std::chrono::year_month_day StrToYmd(const std::string&)
//std::string YmdToStr(const std::chrono::year_month_day&)

std::string AddDays(const std::string& date_str, int n) { // date = "YYYY-MM-DD"
  std::istringstream iss(date_str);
  std::chrono::year_month_day ymd;
  iss >> std::chrono::parse("%F", ymd);
  if (iss.fail()) {
    std::cerr << "Invalid date format: " << date_str << std::endl;
      return date_str; // in the unlikely event of an invalid date, just return back the argument
  }
  std::chrono::sys_days sd = std::chrono::sys_days(ymd);
  sd += std::chrono::days{n};
  std::chrono::year_month_day new_ymd{sd};
  std::ostringstream oss;
    oss << int(new_ymd.year()) << '-'
        << std::setw(2) << std::setfill('0') << unsigned(new_ymd.month()) << '-'
        << std::setw(2) << std::setfill('0') << unsigned(new_ymd.day());
    return oss.str();
}

std::string AddYears(const std::string& date_str, int n) { // date = "YYYY-MM-DD"
    std::istringstream iss(date_str);
  std::chrono::year_month_day ymd;
  iss >> std::chrono::parse("%F", ymd);
  if (iss.fail()) {
    std::cerr << "Invalid date format: " << date_str << std::endl;
      return date_str; // in the unlikely event of an invalid date, just return back the argument
  }
  ymd += std::chrono::years{n};
  
  // if the resulting date is invalid e.g., Feb 29 -> Feb 28)
  if (!ymd.ok()) {
    std::chrono::year_month_day_last ymdl{ymd.year(), std::chrono::month_day_last{ymd.month()}};
    ymd = std::chrono::year_month_day{ymdl};
  }
  
  std::ostringstream oss;
    oss << int(ymd.year()) << '-'
        << std::setw(2) << std::setfill('0') << unsigned(ymd.month()) << '-'
        << std::setw(2) << std::setfill('0') << unsigned(ymd.day());
    return oss.str();
}

std::string SubtractOneYear(const std::string& date) { // date = "YYYY-MM-DD"
  return AddYears(date, -1);
}

std::string SubtractOneDay(const std::string& some_day) { // date = "YYYY-MM-DD"
  return AddDays(some_day, -1);
}

std::string AddOneDay(const std::string& some_day) { // date = "YYYY-MM-DD"
  return AddDays(some_day, 1);
}
