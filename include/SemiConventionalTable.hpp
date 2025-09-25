#ifndef SEMI_CONVENTIONAL_TABLE_HPP
#define SEMI_CONVENTIONAL_TABLE_HPP

#include "LinearTable.hpp"

class SemiConventionalTable : public LinearTable {
public:
  explicit SemiConventionalTable(const std::string& league): LinearTable(league) {}

  explicit SemiConventionalTable(const std::string& league,
                                 const std::string& start_date,
                                 const std::string& end_date)
    : LinearTable(league, start_date, end_date, 3.0f, 1.0f, 1.0f, 3.0f) {}

  virtual ~SemiConventionalTable() = default; // the "virtual" keyword redundant here because transitive?
};

#endif
