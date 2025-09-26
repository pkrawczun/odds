#ifndef PUBLIC_API_HPP
#define PUBLIC_API_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <string>
#include <utility>
#include <memory>
#include <map>
#include <array>
#include <iomanip>
#include <ios> // probably already included?
#endif

#include <pqxx/pqxx>

#include "ConventionalTable.hpp"
#include "SemiConventionalTable.hpp"
#include "LinearTable.hpp"
#include "Table.hpp"

std::unique_ptr<pqxx::connection> SeasonTotalGoals(
                     const std::string& league,
                     const std::string& season,
                     int* total_goals_ptr,
                     std::unique_ptr<pqxx::connection> borrowed_connection) {
  ConventionalTable conv{league, season};
  borrowed_connection = conv.ReadFixtures(std::move(borrowed_connection));
  conv.CompileTable();
  //conv.Renorm();
  //conv.Sort();
  //conv.CalculateStandings();
  //conv.Print();
  *total_goals_ptr = conv.GetTotalGoals();
  return std::move(borrowed_connection);
}


std::unique_ptr<pqxx::connection> SeasonTotalMatches(
                     const std::string& league,
                     const std::string& season,
                     int* total_matches_ptr,
                     std::unique_ptr<pqxx::connection> borrowed_connection) {
  ConventionalTable conv{league, season};
  borrowed_connection = conv.ReadFixtures(std::move(borrowed_connection));
  conv.CompileTable();
  //conv.Renorm();
  //conv.Sort();
  //conv.CalculateStandings();
  //conv.Print();
  *total_matches_ptr = conv.GetTotalMatches();
  return std::move(borrowed_connection);
}

void GenerateTable(Table& tbl) {
  tbl.CompileTable();
  tbl.Renorm();
  tbl.Sort();
  tbl.CalculateStandings();
  tbl.Print();
}

std::unique_ptr<pqxx::connection> GenerateConventionalTable(
                               const std::string& league,
                               const std::string& season,
                               std::unique_ptr<pqxx::connection> borrowed_connection)
{
  ConventionalTable tbl{league, season};
  borrowed_connection = tbl.ReadFixtures(std::move(borrowed_connection));
  GenerateTable(tbl);
  return borrowed_connection;
}

std::unique_ptr<pqxx::connection> GenerateSemiConventionalTable(
                               const std::string& league,
                               const std::string& start_date,
                               const std::string& end_date,
                               std::unique_ptr<pqxx::connection> borrowed_connection)
{
  SemiConventionalTable tbl{league, start_date, end_date};
  borrowed_connection = tbl.ReadFixtures(std::move(borrowed_connection));
  GenerateTable(tbl);
  return borrowed_connection;
}

std::unique_ptr<pqxx::connection> GenerateLinearTable(
                               const std::string& league,
                               const std::string& start_date,
                               const std::string& end_date,
                               float hw, float hd, float ad, float aw,
                               std::unique_ptr<pqxx::connection> borrowed_connection)
{
  LinearTable tbl{league, start_date, end_date, hw, hd, ad, aw};
  borrowed_connection = tbl.ReadFixtures(std::move(borrowed_connection));
  GenerateTable(tbl);
  return borrowed_connection;
}

void SimpleSemiConventionalAnnualAlgorithm(const std::string& league,
                                           const std::string& start_date,
                                           const std::string& end_date) {
  // prints the odds
  std::map<int, std::array<float, 3>> computed_odds;
  std::map<int, std::array<int, 4>> counter;
  SemiConventionalTable all_games{league, start_date, end_date};
  auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  the_outer_connection = all_games.ReadFixtures(std::move(the_outer_connection)); // may add another parameter to Table::ReadFixtures, bool sort=false;
  
  std::string match_date;
  int home_standing{}, away_standing{};
  Outcome outc{};
  std::string home_name, away_name;
  SemiConventionalTable tmp_table(league, "", "");
  for (auto it = all_games.begin(); it != all_games.end(); ++it) {
    std::cout << (*it).ToString() << "\n";
    if (match_date != (*it).GetDate()) { // do not recalculate the table if the match date is the same as the previous one
                                         // NB: all_games is not sorted so will have to sort it
      tmp_table.ResetTable();                                   
      match_date = (*it).GetDate();
      tmp_table.SetDates(SubtractOneYear(match_date), SubtractOneDay(match_date));
      the_connection = tmp_table.ReadFixtures(std::move(the_connection));
      tmp_table.CompileTable();
      tmp_table.Renorm();
      tmp_table.Sort();
      tmp_table.CalculateStandings();
      tmp_table.Print();
    }
    home_name = (*it).GetHome();
    away_name = (*it).GetAway();
    outc = (*it).GetOutcome();
    home_standing = tmp_table.GetStanding(home_name);
    away_standing = tmp_table.GetStanding(away_name);
    
    int diff = home_standing - away_standing;
    
    std::cout
      << "Home team " << home_name << " ranks " << home_standing << " "
      << "Away team " << away_name << " ranks " << away_standing << "\n\n";
    
    (counter[diff])[3]++;
    if (outc == Outcome::H) (counter[diff])[0]++;
    if (outc == Outcome::D) (counter[diff])[1]++;
    if (outc == Outcome::A) (counter[diff])[2]++;
  }
  for (const auto& [key, arr] : counter) {
    computed_odds[key] = {
      static_cast<float>(arr[0]) / static_cast<float>(arr[3]),
      static_cast<float>(arr[1]) / static_cast<float>(arr[3]),
      static_cast<float>(arr[2]) / static_cast<float>(arr[3])
    };
  }
  
  for (const auto& [key, arr] : computed_odds) {
    std::cout 
      << std::setw(3) << key << " "
      << std::setw(5) << std::fixed << std::setprecision(2) << 1.0/arr[0] << " "
      << std::setw(5) << std::fixed << std::setprecision(2) << 1.0/arr[1] << " "
      << std::setw(5) << std::fixed << std::setprecision(2) << 1.0/arr[2] << " "
      << std::setw(5) << counter.at(key)[3] << "\n";
  }
}

void SimpleLinearAnnualAlgorithm(const std::string& league,
                                           const std::string& start_date,
                                           const std::string& end_date,
                                           float hw, float hd, float ad, float aw) {
  // prints the odds
  std::map<int, std::array<float, 3>> computed_odds;
  std::map<int, std::array<int, 4>> counter;
  LinearTable all_games{league, start_date, end_date, hw, hd, ad, aw};
  auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  the_outer_connection = all_games.ReadFixtures(std::move(the_outer_connection)); // may add another parameter to Table::ReadFixtures, bool sort=false;
  
  std::string match_date;
  int home_standing{}, away_standing{};
  Outcome outc{};
  std::string home_name, away_name;
  LinearTable tmp_table(league, "", "", hw, hd, ad, aw);
  for (auto it = all_games.begin(); it != all_games.end(); ++it) {
    std::cout << (*it).ToString() << "\n";
    if (match_date != (*it).GetDate()) { // do not recalculate the table if the match date is the same as the previous one
                                         // NB: all_games is not sorted so will have to sort it
      tmp_table.ResetTable();                                   
      match_date = (*it).GetDate();
      tmp_table.SetDates(SubtractOneYear(match_date), SubtractOneDay(match_date));
      the_connection = tmp_table.ReadFixtures(std::move(the_connection));
      tmp_table.CompileTable();
      tmp_table.Renorm();
      tmp_table.Sort();
      tmp_table.CalculateStandings();
      tmp_table.Print();
    }
    home_name = (*it).GetHome();
    away_name = (*it).GetAway();
    outc = (*it).GetOutcome();
    home_standing = tmp_table.GetStanding(home_name);
    away_standing = tmp_table.GetStanding(away_name);
    
    int diff = home_standing - away_standing;
    
    std::cout
      << "Home team " << home_name << " ranks " << home_standing << " "
      << "Away team " << away_name << " ranks " << away_standing << "\n\n";
    
    (counter[diff])[3]++;
    if (outc == Outcome::H) (counter[diff])[0]++;
    if (outc == Outcome::D) (counter[diff])[1]++;
    if (outc == Outcome::A) (counter[diff])[2]++;
  }
  for (const auto& [key, arr] : counter) {
    computed_odds[key] = {
      static_cast<float>(arr[0]) / static_cast<float>(arr[3]),
      static_cast<float>(arr[1]) / static_cast<float>(arr[3]),
      static_cast<float>(arr[2]) / static_cast<float>(arr[3])
    };
  }
  
  for (const auto& [key, arr] : computed_odds) {
    std::cout 
      << std::setw(3) << key << " "
      << std::setw(5) << std::fixed << std::setprecision(2) << 1.0/arr[0] << " "
      << std::setw(5) << std::fixed << std::setprecision(2) << 1.0/arr[1] << " "
      << std::setw(5) << std::fixed << std::setprecision(2) << 1.0/arr[2] << " "
      << std::setw(5) << counter.at(key)[3] << "\n";
  }
}

#endif
