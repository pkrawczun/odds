#ifndef EVENTS_DATES_HPP
#define EVENTS_DATES_HPP

class EventsDates {
  std::string start_date{}, end_date{};
  std::map<std::string, int> dates;
public:
  EventsDates(const std::string& start_date, const std::string& end_date):
    start_date(start_date), end_date(end_date) {}
  
  std::unique_ptr<pqxx::connection> GetDates(std::unique_ptr<pqxx::connection> borrowed_connection) {
    try {
      //pqxx::connection conn("dbname=odds user=postgres password=mypassword host=localhost");
      if (borrowed_connection && borrowed_connection->is_open()) {
        std::cout << "Accessing the database with EventDates::GetDates: " << borrowed_connection->dbname() << std::endl;
      } else {
        std::cerr << "Failed to access the database with EventDates::GetDates" << std::endl;
        //return 1; // what then? void cannot return 1
      }
      // Create a non-transactional query
      pqxx::nontransaction ntx(*borrowed_connection);
/*      pqxx::result r = ntx.exec_params( R"(
        SELECT fixture_date, EXTRACT(DOW FROM DATE(fixture_date)) as week_day_int -- 0 is Sunday
        FROM premier_league.fixtures WHERE fixture_date
        BETWEEN $1 AND $2;
      )", start_date, end_date);
*/
      std::string query =
         "SELECT fixture_date, "
         "EXTRACT(DOW FROM DATE(fixture_date)) AS week_day_int "  // 0 = Sunday
         "FROM " + league + ".fixtures "
         "WHERE fixture_date BETWEEN $1 AND $2";

      pqxx::result r = ntx.exec_params(query, start_date, end_date);

      for (const auto& row : r) {
          dates[row["fixture_date"].as<std::string>()] =
            row["week_day_int"].as<unsigned int>(); // 0 is Sunday
          
      }
      ntx.commit();
      return borrowed_connection;
    }
    catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
//      return 1; // what then? void cannot return 1
      return borrowed_connection;
    }
  }
  
  void Print(bool footer = false, bool print_weekday = true) {
    for (std::map<std::string, int>::const_iterator it =
      dates.cbegin(); it != dates.cend(); ++it) { // too verbose?
        std::cout << it->first;
        if (print_weekday) {
          // 0 is Sunday
          std::cout << " " << it->second;
        }
        std::cout << "\n";
    }
    if (footer) {
      std::cout << "The number of distinct dates is " << dates.size() << std::endl;
    }
  }

  std::size_t Size() { // std::set::size_type is a typedef inside std::set,
                                        // and it is defined as an unsigned integral type (usually std::size_t)?
    return dates.size();
  }

};

#endif
