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

/*
// a temp function // fails for leap years
std::string SubtractOneYear(const std::string& date) { // date = "YYYY-MM-DD"
  if (date.length() != 10) {
    std::cout << "Invalid date: " << date << std::endl;
    return date;
  }
  std::string str_year = date.substr(0,4);
  std::string the_rest = date.substr(4);
  // if 29 Feb, subtract one day first
  if (the_rest == "-02-29") the_rest = "-02-28"; // still not checking if the date is valid, e.g. 2022-02-29 would be turned into 2021-02-29 which is still invalid and SQL will reject it.
  try{
    int year = std::stoi(str_year);
    year--;
    return std::to_string(year) + the_rest;
  } catch (const std::invalid_argument& e) {
    std::cout << "Invalid input: " << e.what() << std::endl;
    return date;
  } catch (const std::out_of_range& e) {
    std::cout << "Number out of range: " << e.what() << std::endl;
    return date;
  }
  return date;
}
*/

std::string SubtractOneDay(const std::string& some_day) { // date = "YYYY-MM-DD"
  return AddDays(some_day, -1);
}

/*
// a temp function
std::string SubtractOneDay(const std::string& today) { // date = "YYYY-MM-DD"
  std::string yesterday;
  try {
    auto conn = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    // Create a non-transactional query
    pqxx::nontransaction ntx(*conn);
    pqxx::result r = ntx.exec_params( R"(
      SELECT (CAST($1  AS DATE) - 1)::DATE AS previous_day;
    )", today);

    for (auto row : r) {
      yesterday = row["previous_day"].as<std::string>();
    }
  }
  catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return today;
  }
  return yesterday;
}
*/

std::string AddOneDay(const std::string& some_day) { // date = "YYYY-MM-DD"
  return AddDays(some_day, 1);
}

/*
// a temp function
std::string AddOneDay(const std::string& today) { // date = "YYYY-MM-DD"
  std::string tomorrow;
  try {
    auto conn = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    if (conn->is_open()) {
      //std::cout << "Connected to database: " << conn.dbname() << std::endl;
    } else {
      std::cerr << "Failed to connect" << std::endl;
      //return 1; // what then? void cannot return 1
    }
    // Create a non-transactional query
    pqxx::nontransaction ntx(*conn);
    pqxx::result r = ntx.exec_params( R"(
      SELECT (CAST($1  AS DATE) + 1)::DATE AS next_day;
    )", today);

    for (auto row : r) {
      tomorrow = row["next_day"].as<std::string>();
    }
  }
  catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return today;
  }
  return tomorrow;
}
*/
