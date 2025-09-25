#ifndef TABLE_HPP
#define TABLE_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iterator>
#include <tuple>
#include <memory>
#include <string_view>
#endif

#include <pqxx/pqxx>

#include "Row.hpp"
#include "Match.hpp"

class Table {
protected:
  std::string league;
  std::list<Row> rows; // Maybe std::deque might be suitable?
  std::unordered_map<std::string, Row*> m; // A look-up map for that list. There is no dangling-pointers risk after sorting the std::list.
                                 // This std::unordered_map exists to help accessing the elements of that std::list.
  std::unordered_set<std::string> teams;
  std::vector<Match> matches;
  std::string start_date;
  std::string end_date;
  // std::string cannot be used here with "constexpr"
  static constexpr std::string_view header = " Lp          Team            Pld  W  D  L GF GA  GD       Pts Pts Renorm";
public:
  explicit Table(const std::string& league): league(league) {}

  explicit Table(const std::string& league,
                 const std::string& start_date,
                 const std::string& end_date)
    : league(league), start_date(start_date), end_date(end_date) {}
  
  virtual ~Table() = default;

  std::vector<Match>::const_iterator begin() const { return matches.begin(); }
  std::vector<Match>::const_iterator end() const { return matches.end(); }

  void ResetTable();

  void SetLeague(const std::string& s) {
    league = s;
  }
  
  std::string GetLeague() const {
    return league;
  }

  void SetDates(const std::string& d1, const std::string& d2) {
    start_date = d1;
    end_date = d2;
  }

  void CalculateStandings();

  std::tuple<float, float, float> HDAStat(bool print=false);

  void Print(bool print_header=true) const;

  std::unique_ptr<pqxx::connection>
    ReadFixtures(std::unique_ptr<pqxx::connection> borrowed_connection);
    
  // Bad idea. Work with either a shared or a unique pointer at a time
  //void
  //  ReadFixtures(std::shared_ptr<pqxx::connection> borrowed_connection);

  virtual void CompileTable() = 0;

  // this sorting is not deterministic in general
  void Sort() {
    rows.sort(std::greater<Row>()); // descending order
  }

  void Renorm(float f=38.0) {
    for (auto& row: rows) {
      row.Renorm(f);
    }
  }

  // return 0 if team not in table
  int GetStanding(const std::string& team_name) const;

  int GetTotalGoals() const {
    int tmp_count{};
    for (const auto& row :rows) {
      tmp_count += row.GetGoalsFor();
    }
    return tmp_count;
  }
  
  int GetTotalMatches() const {
    int tmp_count{};
    for (const auto& row: rows) {
      // summing the table pld column, therefore twice overcounted
      tmp_count += row.GetPld();
    }
    return tmp_count/2;
  }
  
};

#endif
