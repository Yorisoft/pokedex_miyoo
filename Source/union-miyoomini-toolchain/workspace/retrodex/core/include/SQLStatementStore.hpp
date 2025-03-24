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
    ~SQLStatementStore();

    std::string readFile(const std::filesystem::path &path);

  public:
    const std::string getQuery(const char *q) const;

    static SQLStatementStore *getInstance();
};
