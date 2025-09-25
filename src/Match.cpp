#ifdef USE_MODULES
import std.core;
#else
#include <string>
#include <sstream>
#include <iomanip>
#endif

#include "../include/Match.hpp"

std::string Match::ToString() const {
    std::string outcome_string;
    switch(outcome) {
      case Outcome::H:
        outcome_string = "H";
        break;
      case Outcome::D:
        outcome_string = "D";
        break;
      case Outcome::A:
        outcome_string = "A";
        break;
      default:
        outcome_string = "?";
    }
    std::ostringstream oss;
    oss << std::right
      << std::setw(10) << date
      << std::setw(25) << home
      << std::setw(25) << away
      << std::setw(3) << std::to_string(home_goals)
      << std::setw(3) << std::to_string(away_goals)
      << std::setw(3) << outcome_string;
    return oss.str();
  }
