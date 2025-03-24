#include "SQLStatementStore.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

SQLStatementStore SQLStatementStore::instance;

SQLStatementStore::SQLStatementStore()
{
    if (!std::filesystem::exists(SQL_QUERIES_PATH) ||
        !std::filesystem::is_directory(SQL_QUERIES_PATH))
    {
        std::cerr << "SQLStatementStore() - Invalid default query directory: " << SQL_QUERIES_PATH
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    for (const auto &query : std::filesystem::directory_iterator(SQL_QUERIES_PATH))
        if (std::filesystem::is_regular_file(query) && query.path().extension() == ".sql")
        { // Ensure it's a file
            std::string name = query.path().stem().string();

            const std::string querytext = readFile(query.path());
            queries[name]               = querytext;
        }

    if (queries.empty())
    {
        std::cerr << "SQLStatementStore() - Warning: No SQL "
                     "queries loaded from default directory: "
                  << SQL_QUERIES_PATH << std::endl;
        exit(EXIT_FAILURE);
    }

    // Uncomment when logging is implemented
    /* for (std::pair<std::string, std::string> query : queries) */
    /* { */
    /*     std::cout << query.first << std::endl; */
    /*     std::cout << query.second << std::endl; */
    /* } */
}

SQLStatementStore::SQLStatementStore(const std::filesystem::path &path)
{
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
    {
        std::cerr
            << "SQLStatementStore(const std::filesystem::path &path) - Invalid query directory: "
            << path.string() << std::endl;
        exit(EXIT_FAILURE);
    }

    for (const auto &query : std::filesystem::directory_iterator(path))
        if (std::filesystem::is_regular_file(query) && query.path().extension() == ".sql")
        { // Ensure it's a file
            std::string name = query.path().stem().string();

            const std::string querytext = readFile(query.path());
            queries[name]               = querytext;
        }

    if (queries.empty())
    {
        std::cerr << "SQLStatementStore(const std::filesystem::path &path) - Warning: No SQL "
                     "queries loaded from directory: "
                  << path.string() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Uncomment when logging is implemented
    /* for (std::pair<std::string, std::string> query : queries) */
    /* { */
    /*     std::cout << query.first << std::endl; */
    /*     std::cout << query.second << std::endl; */
    /* } */
}

SQLStatementStore::~SQLStatementStore() {}

std::string SQLStatementStore::readFile(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::in);
    if (!file)
    {
        std::cerr << "SQLStatementStore::readFile(const std::filesystem::path &path) - Could not "
                     "open file: "
                  << path.string() << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

const std::string SQLStatementStore::getQuery(const char *q) const
{
    auto it = queries.find(q);
    if (it != queries.end())
        return it->second;

    return "";
}

SQLStatementStore *SQLStatementStore::getInstance() { return &instance; }
