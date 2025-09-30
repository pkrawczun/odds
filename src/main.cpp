#include <iostream>
#include <string>
#include <exception>

#include "PublicAPI.hpp"
#include "PqxxConnection.hpp"
#include "Season.hpp"
#include "OptionsParser.hpp"

int main (int argc, char** argv) {

  if (argc > 1) {
    auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
    auto the_shared_connection = GetSharedPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");

    auto options = OptionsParser::parse(argc, argv);

    if (options.count(Option::Help) && std::get<bool>(options.at(Option::Help))) {
      std::cout << "Usage: myapp [options]\n";
      return 0;
    }

    if (options.count(Option::League)) {
      std::cout << "League: " << std::get<std::string>(options.at(Option::League)) << "\n";
    }

    if (options.count(Option::Season)) {
      std::cout << "Season: " << std::get<std::string>(options.at(Option::Season)) << "\n";
    }

    if (options.count(Option::StartDate)) {
      std::cout << "Start Date: " << std::get<std::string>(options.at(Option::StartDate)) << "\n";
    }

    if (options.count(Option::EndDate)) {
      std::cout << "End Date: " << std::get<std::string>(options.at(Option::EndDate)) << "\n";
    }

    if (options.count(Option::Scoring)) {
      auto scores = std::get<std::vector<float>>(options.at(Option::Scoring));
      for (auto v : scores) std::cout << "Score param: " << v << "\n";
    }
    
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
        } else {
          std::cout << "Some error.";
        }
      }
    
    return 0;
  }

/*  if (argc>1) {
    try {
      auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
      auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
      auto the_shared_connection = GetSharedPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
      cxxopts::Options options("Odds", "A sports betting odds calculator using statistics.");
      
      options.add_options()
        ("l,league", "League", cxxopts::value<std::string>()) // string option
        ("s,season", "Season in YYY1-YYY2 format", cxxopts::value<std::string>()) // string option
        ("ed", "End date in YYYY-MM-DD format", cxxopts::value<std::string>()) // string option
        ("sd", "Start date in YYYY-MM-DD format", cxxopts::value<std::string>()) // string option
        ("conv", "Calculate a conventional table")   // flag
        ("semi", "Calculate a semi-conventional table")   // flag
        ("line", "Calculate a linear table")   // flag
        ("sc", "Scoring parameters", cxxopts::value<std::vector<float>>())
        ("h,help", "Print help");

      auto result = options.parse(argc, argv);
      
      if (result.count("help")) {
        std::cout << options.help() << std::endl;
          return 0;
        }
      
      if (result.count("sc")) {
        auto sc = result["sc"].as<std::vector<float>>();
        std::cout
         << "The number of args given: " << sc.size() << std::endl
         << "Values: ";
         for (auto x:sc) std::cout << x << " ";
         std::cout << std::endl;
        return 0;
      }

    } catch (const std::exception& e) {
      std::cerr << "Error parsing options: " << e.what() << "\n";
      return 1;
    }
    return 0;
    
  }
*/

  auto the_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  auto the_outer_connection = GetPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");
  auto the_shared_connection = GetSharedPqxxConnection("dbname=odds user=postgres password=mypassword host=localhost");

  //the_connection = GenerateConventionalTable("bundesliga", "2010-2011", std::move(the_connection));
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
  SimpleLinearAnnualAlgorithm("primera_division", "2009-08-01", "2025-09-25", 2.22f, 1.78f, 2.22f, 3.33f);

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
