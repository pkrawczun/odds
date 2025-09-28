#ifndef LINEAR_TABLE_HPP
#define LINEAR_TABLE_HPP

#include "Table.hpp"

class LinearTable : public Table {
protected:
  // points for: home win, home draw, away draw, away win.
  // For ConventionalTable and SemiConventionalTable these are (3.0, 1.0, 1.0, 3.0)
  std::array<float, 4> scoring_system{};
public:
  explicit LinearTable(const std::string& league): Table(league) {}

  explicit LinearTable(const std::string& league,
                       const std::string& start_date,
                       const std::string& end_date)
    : Table(league, start_date, end_date) {}
  
  explicit LinearTable(const std::string& league,
                       const std::string& start_date,
                       const std::string& end_date,
                       float hw, float hd, float ad, float aw)
    : Table(league, start_date, end_date), scoring_system{hw, hd, ad, aw} {}
  
  explicit LinearTable(const std::string& league,
                       const std::string& start_date,
                       const std::string& end_date,
                       const std::array<float, 4>& scoring)
    : Table(league, start_date, end_date), scoring_system(scoring) {}
  
  virtual ~LinearTable() = default; // the "virtual" keyword redundant here because transitive?

  virtual void CompileTable() override {
    for (const auto& match : matches) {
      teams.insert(match.GetHome());
      teams.insert(match.GetAway());
    } // teams now contains all team names (distinct)
    for (const std::string& team: teams) { // is & needed here?
      rows.push_back(team);
    } // our table is now populated with all relevant team names with all statistics set to 0

    // why for (const auto&row: rows) fails?
    for (auto& row: rows) { // NB: & is needed to avoid dangling pointers?
      m[row.GetTeamName()] = &row; // this key is a copy of std::string team_name from the Row class, but it won't change anyway
    }

    for (const auto& match: matches) {
      std::string h = match.GetHome();
      std::string a = match.GetAway();
      m[h]->IncrementPld();
      m[a]->IncrementPld();
      if (match.GetOutcome() == Outcome::H) { // home won
        m[h]->IncrementWon();
        m[a]->IncrementLost();
        m[h]->UpdatePts(scoring_system[0]);
      } else if (match.GetOutcome() == Outcome::A) { // away won
        m[h]->IncrementLost();
        m[a]->IncrementWon();
        m[a]->UpdatePts(scoring_system[3]);
      } else { // the outcome is Drawn
        m[h]->IncrementDrawn();
        m[a]->IncrementDrawn();
        m[h]->UpdatePts(scoring_system[1]);
        m[a]->UpdatePts(scoring_system[2]);
      }
      m[h]->UpdateGoalsFor(match.GetHomeGoals());
      m[h]->UpdateGoalsAgainst(match.GetAwayGoals());
      m[h]->UpdateGoalDifference( match.GetHomeGoals() - match.GetAwayGoals() );
      m[a]->UpdateGoalsFor(match.GetAwayGoals());
      m[a]->UpdateGoalsAgainst(match.GetHomeGoals());
      m[a]->UpdateGoalDifference( match.GetAwayGoals() - match.GetHomeGoals() );
    }
  }
  
  void SetScoring(const std::array<float, 4>& s) {
    scoring_system = s;
  }
  
  void SetScoring(float hw, float hd, float ad, float aw) {
    scoring_system = {hw, hd, ad, aw};
  }

  std::array<float, 4> GetScoring() const {
    return scoring_system;
  }

};

#endif
