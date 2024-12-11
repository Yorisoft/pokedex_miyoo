#pragma once
#include <iostream> 
#include"sqlite/sqlite3.h"


class PokedexDB {
private:

	static std::vector<std::vector<std::string>>* results;
    static sqlite3* db;
	static int rc;   
	static int languageVersion;
    static char* zErrMsg;
	static bool isTestMode;
	static const std::string MAIN_DB_PATH;
	static const std::string TEST_DB_PATH;
	static std::string gameVersion;
	static int regionVersion;

	static int callback(void* results, int argc, char** argv, char** azColName);
	static const std::string getDBpath();
	static int openDB();
	static void closeDB(sqlite3* db);
	static void cleanup();


public:
	PokedexDB();
	static std::vector<std::vector<std::string>>* executeSQL(const std::string*);
	static void printSQLresults(std::vector<std::vector<std::string>>*);
	static void enableTestMode(bool);  // To turn on test mode
	static void setLanguageVersion(int);
	static void setGameVersion(std::string&);
	static void setRegionVersion(int);
	
	~PokedexDB();

};