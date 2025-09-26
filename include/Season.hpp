#ifndef SEASON_HPP
#define SEASON_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <utility>
#include <string>
#include <memory>
#endif

#include <pqxx/pqxx>

#include "PqxxConnection.hpp"

/*
std::pair<std::string, std::string> GetSeasonDates(
                    const std::string& league,
                    const std::string& season,
                    std::shared_ptr<pqxx::connection> shared_connection) {
  return {"", ""};
}
*/

class Season {
  std::string league; // maybe const?
  std::string season; // maybe const?
  std::string start_date, end_date;
public:
  Season(const std::string& league, const std::string& season):
    league(league), season(season) {
      auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
      try {
        if (the_connection && the_connection->is_open()) {
        std::cout << "Accessing the db with Season::Season: " << the_connection->dbname() << std::endl;
        } else {
          std::cerr
            << "Failed to access db with Season::Season for league "
            << league << ", season " << season << std::endl;
        }
        pqxx::nontransaction ntx(*the_connection);
        std::string query = 
          "SELECT start_date, end_date "
          "FROM " + league + ".seasons "
          "WHERE name = '" + season + "' ;";
        pqxx::result r = ntx.exec(query);
        for (const auto& row : r) {
          start_date = row["start_date"].as<std::string>();
          end_date = row["end_date"].as<std::string>();
        }
      }
      catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
  
  Season(const std::string& league, const std::string& season,  const std::string& start_date,  const std::string& end_date):
    league(league), season(season), start_date(start_date), end_date(end_date) {}
  
  std::string GetStartDate() const {
    return start_date;
  }
  
  std::string GetEndDate() const {
    return end_date;
  }
  
  void Print() {
    std::cout
      << "The " << league << " season " + season
      << " ran from " << start_date << " until " << end_date << std::endl;
  }
};

#endif
