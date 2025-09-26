#ifndef PQXX_CONNECTION_HPP
#define PQXX_CONNECTION_HPP

#ifdef USE_MODULES
import std.core;
#else
#include <memory>
#include <iostream>
#include <concepts>
#endif

#include <pqxx/pqxx>

// this concept only allows unique_ptr<T> or shared_ptr<T>
template <typename P>
concept AllowedPtr =
    std::same_as<P, std::unique_ptr<typename P::element_type>> ||
    std::same_as<P, std::shared_ptr<typename P::element_type>>;

template <AllowedPtr Ptr>
Ptr GetPqxxConnectionImpl(char const options[]) {
    try {
        Ptr the_connection(new pqxx::connection(options));

        if (the_connection && the_connection->is_open()) {
            std::cout << "Connected to database: "
                      << the_connection->dbname() << std::endl;
            return the_connection;
        } else {
            std::cout << "Failed to connect to database." << std::endl;
            return nullptr;
        }
    } catch (const pqxx::broken_connection &e) {
        std::cerr << "Database connection failed: " << e.what() << std::endl;
        return nullptr;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return nullptr;
    }
}

inline std::unique_ptr<pqxx::connection> GetPqxxConnection(char const options[]) {
    return GetPqxxConnectionImpl<std::unique_ptr<pqxx::connection>>(options);
}

inline std::shared_ptr<pqxx::connection> GetSharedPqxxConnection(char const options[]) {
    return GetPqxxConnectionImpl<std::shared_ptr<pqxx::connection>>(options);
}

#endif
