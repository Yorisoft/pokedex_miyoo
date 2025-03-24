// SQLStatementStore.h
#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

class SQLStatementStore
{
  private:
    static SQLStatementStore instance;

    const std::string SQL_QUERIES_PATH = "core/utils/queries/";

    std::unordered_map<std::string, std::string> queries;

  private:
    SQLStatementStore();
    SQLStatementStore(const std::filesystem::path &path);

    const std::string getQuery(const std::string &q) const;
    std::string readFile(const std::filesystem::path &path);

  public:
    static SQLStatementStore *getInstance();
};
