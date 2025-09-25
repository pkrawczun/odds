#ifdef USE_MODULES
import std.core;
#else
#include <memory>
#include <iostream>
#endif

#include <pqxx/pqxx>

#include "PqxxConnection.hpp"

std::unique_ptr<pqxx::connection> GetPqxxConnection(char const options[]) {
  std::unique_ptr<pqxx::connection> the_connection = std::make_unique<pqxx::connection>(options);
  if (the_connection->is_open()) {
    std::cout << "Connected to database from PqxxConnection.hpp: " << the_connection->dbname() << std::endl;
    return the_connection;
  } else {
    std::cout << "Failed to connect to database from PqxxConnection.hpp." << std::endl;
    return nullptr;
  }

}

std::shared_ptr<pqxx::connection> GetSharedPqxxConnection(char const options[]) {
  std::shared_ptr<pqxx::connection> the_connection = std::make_shared<pqxx::connection>(options);
  if (the_connection->is_open()) {
    std::cout << "Connected to database from PqxxConnection.hpp: " << the_connection->dbname() << std::endl;
    return the_connection;
  } else {
    std::cout << "Failed to connect to database from PqxxConnection.hpp." << std::endl;
    return nullptr;
  }

}
