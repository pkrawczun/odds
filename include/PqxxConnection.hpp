#ifndef PQXX_CONNECTION_HPP
#define PQXX_CONNECTION_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <memory>
#endif

#include <pqxx/pqxx>

std::unique_ptr<pqxx::connection> GetPqxxConnection(char const options[]);
std::shared_ptr<pqxx::connection> GetSharedPqxxConnection(char const options[]);

#endif
