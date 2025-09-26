#include <iostream>
#include <string>

#include "cxxopts.hpp"
#include "PublicAPI.hpp"
#include "PqxxConnection.hpp"
#include "Season.hpp"

int main (int argc, char** argv) {
    auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    auto the_shared_connection = GetSharedPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");

  the_connection = GenerateConventionalTable("bundesliga", "2010-2011", std::move(the_connection));
  //the_connection = GenerateSemiConventionalTable("premier_league", "2025-01-01", "2025-09-30" , std::move(the_connection));
/*  the_connection = GenerateLinearTable("premier_league", "2024-09-24", "2025-09-25",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("bundesliga", "2024-09-24", "2025-09-25",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("ligue_1", "2024-09-24", "2025-09-25",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("serie_a", "2024-09-24", "2025-09-25",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("primera_division", "2024-09-24", "2025-09-25",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
 */ 
  //SimpleSemiConventionalAnnualAlgorithm("premier_league", "2009-08-01", "2025-09-09");
  //SimpleLinearAnnualAlgorithm("primera_division", "2009-08-01", "2025-09-25", 2.22f, 1.78f, 2.22f, 3.33f);

/*  
  const std::string& l ="bundesliga";
  const std::string& s = "2010-2011";
  int total = 0;
  the_connection = SeasonTotalGoals(l, s, &total, std::move(the_connection));
  std::cout
    << "The total goal count in " << l << " season " << s << " is "
    << total << std::endl;
  the_connection = SeasonTotalMatches(l, s, &total, std::move(the_connection));
  std::cout
    << "The total matches count in " << l << " season " << s << " is "
    << total << std::endl;
*/
  
//  Season season("premier_league", "2023-2024");
//  season.Print();
  
  return 0;
}
