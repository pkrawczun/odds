#ifndef CONVENTIONAL_TABLE_HPP
#define CONVENTIONAL_TABLE_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <iostream>
#include <string>
#endif

#include "SemiConventionalTable.hpp"
#include "Miscellaneous.hpp"
#include "Season.hpp"

class ConventionalTable final : public SemiConventionalTable {
public:
  explicit ConventionalTable(const std::string& league): SemiConventionalTable(league) {}

  explicit ConventionalTable(const std::string& league,
                             const std::string& season)
   :  SemiConventionalTable(league) {
    scoring_system={3.0f, 1.0f, 1.0f, 3.0f}; // WHY IS THIS NEEDED HERE?
    try {
      Season s(league, season);
      start_date = s.GetStartDate();
      end_date = s.GetEndDate();
      /*
      if (league == "premier_league") {
        // NB: using find() instead of at() might make it cleaner
        start_date = premier_league::seasons.at(season).first;
        end_date = premier_league::seasons.at(season).second;
      } else if (league == "bundesliga") {
        start_date = bundesliga::seasons.at(season).first;
        end_date = bundesliga::seasons.at(season).second;
        //auto dates GetSeasonDates("bundesliga", "2002-2003");
        //auto std::start_date = dates.first;
        //auto std::end_date = dates.second;
      } else if (league == "ligue_1") {
        start_date = ligue_1::seasons.at(season).first;
        end_date = ligue_1::seasons.at(season).second;
      } else if (league == "serie_a") {
        start_date = serie_a::seasons.at(season).first;
        end_date = serie_a::seasons.at(season).second;
      } else if (league == "primera_division") {
        start_date = primera_division::seasons.at(season).first;
        end_date = primera_division::seasons.at(season).second;
      } else {
        std::cerr << "League " << league << " does not exist." << std::endl;
        start_date = "";
        end_date = "";
      }
      */
    } catch (const std::out_of_range& e) {
      std::cerr << "Season " << season << " does not exist for league " << league << "." << std::endl;
      start_date = "";
      end_date = "";
    }
  }


};

#endif
