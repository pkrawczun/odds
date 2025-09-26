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
    } catch (const std::out_of_range& e) {
      std::cerr << "Season " << season << " does not exist for league " << league << "." << std::endl;
      start_date = "";
      end_date = "";
    }
  }


};

#endif
