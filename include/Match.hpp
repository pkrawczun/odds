#ifndef MATCH_HPP
#define MATCH_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <string>
#endif

#include "Miscellaneous.hpp"

class Match final {
  std::string date;
  std::string home, away;
  int home_goals, away_goals;
  Outcome outcome;
  
public:
  Match(const std::string& date,
        const std::string& home,
        const std::string& away,
        int home_goals, int away_goals, Outcome outcome)
      : date(date), // Avoid unnecessary copies vs 
        home(home), // pass by const std::string& to leave
        away(away), // the original variable passed to this constructor intact.
        home_goals(home_goals),
        away_goals(away_goals),
        outcome(static_cast<Outcome>(outcome)) {}

  std::string GetDate() const { return date; }
  int GetHomeGoals() const { return home_goals; }
  int GetAwayGoals() const { return away_goals; }
  std::string GetHome() const { return home; }
  std::string GetAway() const { return away; }
  Outcome GetOutcome() const { return outcome; }
  std::string ToString() const;
  
};

#endif
