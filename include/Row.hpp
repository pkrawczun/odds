#ifndef ROW_HPP
#define ROW_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <string>
#include <tuple>
#include <sstream>
#include <iomanip>
#endif

class Row {
  std::string team_name;
  int pld{}, won{}, drawn{}, lost{}, goals_for{}, goals_against{},
      goal_difference{};
  float pts{}, pts_renorm{};
  int standing{};
public:
  Row(std::string team_name): team_name(team_name) {}
  
  // The == operation on floats is fine here because if equal, they will be computed with the same formula
  bool operator==(const Row& other) const {
    return std::tie(pts_renorm, pts, goal_difference, goals_for) ==
             std::tie(other.pts_renorm, other.pts, other.goal_difference, other.goals_for);
  }

  // The > operation on floats is fine here because if equal, they will be computed with the same formula
  bool operator>(const Row& other) const {
      return std::tie(pts_renorm, pts, goal_difference, goals_for) >
             std::tie(other.pts_renorm, other.pts, other.goal_difference, other.goals_for);
  }

  
  // these operators aren't strict, so two objects may be distinct but "equal"
  // sorting won't be in general deterministic
  bool operator<(const Row& other) const {
    return !(*this>other || *this==other);
  }
  
  bool operator!=(const Row& other) const {
    return !(*this==other);
  }
  
  bool operator>=(const Row& other) const {
    return (*this>other || *this==other);
  }
  
  bool operator<=(const Row& other) const {
    return (*this<other || *this==other);
  }
  
  std::string ToString() const {
    std::ostringstream oss;
    oss << std::right
      << std::setw(3) << std::to_string(standing)
      << std::setw(25) << team_name
      << std::setw(4) << std::to_string(pld)
      << std::setw(3) << std::to_string(won)
      << std::setw(3) << std::to_string(drawn)
      << std::setw(3) << std::to_string(lost)
      << std::setw(3) << std::to_string(goals_for)
      << std::setw(3) << std::to_string(goals_against)
      << std::setw(4) << std::to_string(goal_difference)
      << std::setw(10) << std::to_string(pts)
      << std::setw(11) << std::to_string(pts_renorm);
    return oss.str();
  }

  std::string GetTeamName() const { return team_name; }

  /**
   * @brief Increments the number of games played by one.
   */
  void IncrementPld() { pld++;}

  /**
   * @brief Increments the number of games won by one.
   */
  void IncrementWon() { won++; }

  /**
   * @brief Increments the number of games drawn by one.
   */
  void IncrementDrawn() { drawn++; }

  /**
   * @brief Increments the number of games lost by one.
   */
  void IncrementLost() { lost++; }

  /**
   * @brief Changes the number of goals for by the value given.
   *
   * @param n The number of goals scored in the game.
   */
  void UpdateGoalsFor(int n) { goals_for += n; }

  /**
   * @brief Changes the number of goals lost by the value given.
   * 
   * @param n The number of lost goals in the game.
   */
  void UpdateGoalsAgainst(int n) { goals_against += n; }

  /**
   * @brief Changes the number of goal difference by the value given.
   *
   * @param n The number of goals scored minus goals lost in the game.
   */
  void UpdateGoalDifference(int n) { goal_difference += n; }

  /**
   * @brief Changes the number of points by the value given.
   *
   * @param n The number of points gained in the game.
   */
  void UpdatePts(int n) { pts += n; } // MAKE A TEMPLATE
  void UpdatePts(float n) { pts += n; }

  void SetStanding(int n) { standing = n; }

  int GetStanding() const { return standing; }

  /**
   * @brief Renormalizes the number of points to an arbitrary constant (38.0 by default). 38 is the number of games each team plays per season. If the end date and the start date do not match a specific season, not all teams will have played the same number of matches so the value of conventional points is not conclusive.
   */
  void Renorm(float f=38.0) {
    pts_renorm = static_cast<float>(pts)/static_cast<float>(pld)*f;
  }
  
  int GetGoalsFor() const {
    return goals_for;
  }
  
  int GetPld() const {
    return pld;
  }
};

#endif
