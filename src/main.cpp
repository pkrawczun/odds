#include <iostream>
#include <string>
#include <exception>

#include "PublicAPI.hpp"
#include "PqxxConnection.hpp"
#include "Season.hpp"
#include "OptionsParser.hpp"
#include "tests/Tests.hpp"

int main (int argc, char** argv) {

  if (argc > 1) {
    auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    auto the_shared_connection = GetSharedPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");

    auto options = OptionsParser::parse(argc, argv);
    if (
      (options.count(Option::Conv) && std::get<bool>(options.at(Option::Conv)))
      || (options.count(Option::Semi) && std::get<bool>(options.at(Option::Semi)))
      || (options.count(Option::Line) && std::get<bool>(options.at(Option::Line)))
    ) {
        
        Table* tbl;
        if (
          options.count(Option::Conv)
          && std::get<bool>(options.at(Option::Conv))
          && options.count(Option::League)
          && options.count(Option::Season)
          ) {
            std::cout
              << "Calculating a conventional table: league "
              << std::get<std::string>(options.at(Option::League))
              << ", season "
              << std::get<std::string>(options.at(Option::Season))
              << ".\n";
            the_connection = GenerateConventionalTable(
              std::get<std::string>(options.at(Option::League)),
              std::get<std::string>(options.at(Option::Season)),
              std::move(the_connection));
        } else if (
          options.count(Option::Semi)
          && std::get<bool>(options.at(Option::Semi))
          && options.count(Option::League)
          && options.count(Option::StartDate)
          && options.count(Option::EndDate)
        ) {
          std::cout
            << "Calculating a semi-conventional table: league "
            << std::get<std::string>(options.at(Option::League))
            << " from "
            << std::get<std::string>(options.at(Option::StartDate))
            << " to "
            << std::get<std::string>(options.at(Option::EndDate))
            << ".\n";
          the_connection = GenerateSemiConventionalTable(
              std::get<std::string>(options.at(Option::League)),
              std::get<std::string>(options.at(Option::StartDate)),
              std::get<std::string>(options.at(Option::EndDate)),
              std::move(the_connection));
        } else if(
          options.count(Option::Line)
          && std::get<bool>(options.at(Option::Line))
          && options.count(Option::League)
          && options.count(Option::StartDate)
          && options.count(Option::EndDate)
          && options.count(Option::Scoring)
          && std::get<std::vector<float>>(options.at(Option::Scoring)) .size() == 4
        ) {
          std::cout
            << "Calculating a linear table: league "
            << std::get<std::string>(options.at(Option::League))
            << " from "
            << std::get<std::string>(options.at(Option::StartDate))
            << " to "
            << std::get<std::string>(options.at(Option::EndDate))
            << " with scoring parameters ";
          for (const auto& f: std::get<std::vector<float>>(options.at(Option::Scoring)))
          {
            std::cout << f << ", ";
          }
          std::cout << std::endl;
          the_connection = GenerateLinearTable(
              std::get<std::string>(options.at(Option::League)),
              std::get<std::string>(options.at(Option::StartDate)),
              std::get<std::string>(options.at(Option::EndDate)),
              std::get<std::vector<float>>(options.at(Option::Scoring))[0],
              std::get<std::vector<float>>(options.at(Option::Scoring))[1],
              std::get<std::vector<float>>(options.at(Option::Scoring))[2],
              std::get<std::vector<float>>(options.at(Option::Scoring))[3],
              std::move(the_connection));
        } else {
          std::cout << "Some error.";
        }
      } else if (
        options.count(Option::Tests) && std::get<bool>(options.at(Option::Tests))
      ) {
        std::cout << "Running tests..." << std::endl;
        RunTests(false);
      }
    
    return 0;
  }

  auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  auto the_shared_connection = GetSharedPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");

  //the_connection = GenerateConventionalTable("bundesliga", "2010-2011", std::move(the_connection));
  //the_connection = GenerateSemiConventionalTable("premier_league", "2025-01-01", "2025-09-30" , std::move(the_connection));
/*  the_connection = GenerateLinearTable("premier_league", "2024-09-30", "2025-09-30",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("bundesliga", "2024-09-30", "2025-09-30",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("ligue_1", "2024-09-30", "2025-09-30",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("serie_a", "2024-09-30", "2025-09-30",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
  the_connection = GenerateLinearTable("primera_division", "2024-09-30", "2025-09-30",
                                       2.22f, 1.78f, 2.22f, 3.33f, std::move(the_connection));
*/  
  //SimpleSemiConventionalAnnualAlgorithm("bundesliga", "2009-08-01", "2025-09-09");
  SimpleLinearAnnualAlgorithm("primera_division", "2009-08-01", "2025-09-30", 2.22f, 1.78f, 2.22f, 3.33f);

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
  
  /*
  std::string today = "2020-02-29";
  std::cout << today << std::endl;
  std::cout << AddOneDay(today) << std::endl;
  std::cout << SubtractOneDay(today) << std::endl;
  std::cout << SubtractOneYear(today) <<std::endl;
  std::cout << SubtractOneDay(SubtractOneYear(today)) <<std::endl;
  */
  return 0;
}
