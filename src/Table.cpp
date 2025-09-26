#include "../include/Table.hpp"
#include "Miscellaneous.hpp"

#ifdef USE_MODULES
import std.core;
#else
#include <tuple>
#include <iostream>
#include <string>
#include <memory>
#include <exception>
#endif

#include <pqxx/pqxx>

void Table::ResetTable() {
    rows.clear();
    m.clear();
    teams.clear();
    matches.clear();
    start_date="";
    end_date="";
  }

// ex-aequo not considered because they aren't statistically relevant
void Table::CalculateStandings() {
    int tmp{};
    for (auto& row: rows) {
      tmp++;
      row.SetStanding(tmp);
    }
  }

  std::tuple<float, float, float> Table::HDAStat(bool print) {
    int h{}, d{}, a{};
    for (const auto& match: matches) {
      switch(match.GetOutcome()) {
        case Outcome::H: {
          h++;
          break;
        }
        case Outcome::D: {
          d++;
          break;
        }
        case Outcome::A: {
          a++;
          break;
        }
        default: {
          std::cout << "Invalid Outcome." << std::endl;
        }
      }
    }
    int total = matches.size();
    // divisio-by-zero risk. Prints "inf" with g++ which is fine for now
    float h_prob = static_cast<float>(h)/static_cast<float>(total);
    float d_prob = static_cast<float>(d)/static_cast<float>(total);
    float a_prob = static_cast<float>(a)/static_cast<float>(total);
    if (print) {
      std::cout << h_prob << " " << d_prob << " " << a_prob << "\n";
    }
    return std::make_tuple(h_prob, d_prob, a_prob);
  }
  
  
  void Table::Print(bool print_header) const {
    std::cout << "The number of rows is " << rows.size() << std::endl;
    if (print_header) {
      std::cout << Table::header << std::endl;
    }
    for (const auto& row: rows) std::cout << row.ToString() << "\n";
    return;
  }

  std::unique_ptr<pqxx::connection>
    Table::ReadFixtures(std::unique_ptr<pqxx::connection> borrowed_connection) {
    try {
      // the connection should be "dbname=odds user=postgres password=mypassword host=localhost"
      if (borrowed_connection && borrowed_connection->is_open()) {
        std::cout << "Accessing the db with Table::ReadFixtures: " << borrowed_connection->dbname() << std::endl;
      } else {
        std::cerr << "Failed to access db with Table::ReadFixtures" << std::endl;
        return nullptr;
      }
      pqxx::nontransaction ntx(*borrowed_connection);
/*      pqxx::result r = ntx.exec_params( R"(
        SELECT fixture_date, home, away, home_score, away_score, outcome
        FROM premier_league.fixtures WHERE fixture_date
        BETWEEN $1 AND $2;
      )", start_date, end_date);
*/


      std::string query =
		"SELECT "
		"    f.fixture_date, "
		"    ht.name AS home, "
		"    at.name AS away, "
		"    f.home_score, "
		"    f.away_score, "
		"    f.outcome "
		"FROM " + league + ".fixtures AS f "
		"JOIN " + league + ".teams AS ht ON f.home_id = ht.team_id "
		"JOIN " + league + ".teams AS at ON f.away_id = at.team_id "
		"WHERE f.fixture_date BETWEEN $1 AND $2; ";


/*      std::string query = 
            "SELECT fixture_date, home, away, home_score, away_score, outcome "
            "FROM " + league + ".fixtures "
            "WHERE fixture_date BETWEEN $1 AND $2";
*/
      pqxx::result r = ntx.exec_params(query, start_date, end_date);



      for (const auto& row : r) {
          matches.push_back({
            row["fixture_date"].as<std::string>(),
            row["home"].as<std::string>(),
            row["away"].as<std::string>(),
            row["home_score"].as<int>(),
            row["away_score"].as<int>(),
            static_cast<Outcome>(row["outcome"].as<int>())
          });
      }
      //ntx.commit();  // nontransactions don't need commit
      return borrowed_connection;
    }
    catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      return nullptr;
    }
    // Print the matches on the screen
    //for (auto match : matches) {
    //  std::cout << match.ToString() << "\n";
    //}
    std::cout << "Total matches: " << std::to_string(matches.size()) << std::endl;
    std::cout << "Time period: " << start_date << " to " << end_date << std::endl;
  }
  
// return 0 if team not in table
  int Table::GetStanding(const std::string& team_name) const {
    if (m.find(team_name) == m.end()) {
      return 0;
    } else {
      // no not use the [] operator here
      return m.at(team_name) -> GetStanding();
    }
  }

