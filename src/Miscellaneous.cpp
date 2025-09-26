#include "Miscellaneous.hpp"

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
