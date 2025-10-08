#ifndef OPTIONS_PARSER_HPP
#define OPTIONS_PARSER_HPP

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <iostream>
#include <sstream>

enum class Option {
  Help,
  League,
  Season,
  StartDate,
  EndDate,
  Conv,
  Semi,
  Line,
  Scoring,
  Tests,
};

using OptionValue = std::variant<std::monostate, bool, std::string, std::vector<float>>;
using OptionsMap = std::unordered_map<Option, OptionValue>;

class OptionsParser {
public:
  static OptionsMap parse(int argc, char** argv) {
    OptionsMap opts;

    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];

      if (arg == "--help" || arg == "-h") {
        opts[Option::Help] = true;
      }
      else if (arg == "--league" || arg == "-l") {
        if (i + 1 < argc) opts[Option::League] = std::string(argv[++i]);
      }
      else if (arg == "--season" || arg == "-s") {
        if (i + 1 < argc) opts[Option::Season] = std::string(argv[++i]);
      }
      else if (arg == "--sd") {
        if (i + 1 < argc) opts[Option::StartDate] = std::string(argv[++i]);
      }
      else if (arg == "--ed") {
        if (i + 1 < argc) opts[Option::EndDate] = std::string(argv[++i]);
      }
      else if (arg == "--conv") {
        opts[Option::Conv] = true;
      }
      else if (arg == "--semi") {
        opts[Option::Semi] = true;
      }
      else if (arg == "--line") {
        opts[Option::Line] = true;
      }
      else if (arg == "--run-tests") {
        opts[Option::Tests] = true;
      }
      else if (arg == "--sc") {
        std::vector<float> vals;
        while (i + 1 < argc && argv[i+1][0] != '-') {
          vals.push_back(std::stof(argv[++i]));
        }
        opts[Option::Scoring] = vals;
      }
      else {
        std::cerr << "Unknown option: " << arg << "\n";
      }
    }

    return opts;
  }
};

#endif
