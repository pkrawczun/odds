#ifndef SEASON_HPP
#define SEASON_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <utility>
#include <string>
#endif

/*
std::pair<std::string, std::string> GetSeasonDates(
                    const std::string& league,
                    const std::string& season,
                    std::shared_ptr<pqxx::connection> shared_connection) {
  return {"", ""};
}

class Season {
  std::string league;
  std::string season;
  std::string start, end;
public:
  Season() = default;
  std::unique_ptr<pqxx::connection> GetDates();
};
*/
#endif
