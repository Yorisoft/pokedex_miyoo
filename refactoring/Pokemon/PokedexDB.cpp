#include <iostream>
#include <vector>
#include <string>
#include "sqlite/sqlite3.h"
#include "PokedexDB.h"

bool PokedexDB::isTestMode = false;
const std::string PokedexDB::MAIN_DB_PATH = "res/db/pokedex.sqlite";
const std::string PokedexDB::TEST_DB_PATH = "../res/db/pokedex.sqlite";
std::string PokedexDB::gameVersion = "red";
int PokedexDB::languageVersion = 9;
std::vector<std::vector<std::string>>* PokedexDB::results = new std::vector<std::vector<std::string>>();
sqlite3* PokedexDB::db;
int PokedexDB::rc;   
char* PokedexDB::zErrMsg;


PokedexDB::PokedexDB() {
 }

int PokedexDB::openDB() {
    rc = sqlite3_open(getDBpath().c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cout << "SQLite could not open file! SQLite Error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }
    return rc;
}

void PokedexDB::closeDB(sqlite3* db) {
    sqlite3_close(db);
}

const std::string PokedexDB::getDBpath() {
    return isTestMode ? TEST_DB_PATH : MAIN_DB_PATH;
}

void PokedexDB::enableTestMode(bool isTest) { // Used for changing directory paths between testing and live environments
    isTestMode = isTest;
}

std::vector<std::vector<std::string>>* PokedexDB::executeSQL(const std::string* SQL_STATEMENT) {
    // change sql statement to be language specific
    // find position of string to replace with languageVersion
    std::string SQLstatement = *SQL_STATEMENT;
    size_t pos;
    pos = SQLstatement.find("${language_id}");
    while (pos != std::string::npos) {
        SQLstatement.replace(pos, std::string("${language_id}").length(), std::to_string(languageVersion));
        pos = SQLstatement.find("${language_id}");
    }
    
    // change sql statement to be game version specific
    // find position of string to replace with gameVersion
    pos = SQLstatement.find("${game_version}");
    while (pos != std::string::npos) {
        SQLstatement.replace(pos, std::string("${game_version}").length(), gameVersion);
        pos = SQLstatement.find("${game_version}");
    }

    // clear any previous results
    results->clear();

    // Open SQLite database
    rc = openDB();

    // execute sql querry
	if (sqlite3_exec(db, SQLstatement.c_str(), callback, results, &zErrMsg) != SQLITE_OK || rc != SQLITE_OK) {
		std::cout << "SQL error: " << zErrMsg << std::endl;
		sqlite3_close(db);
        return {};
	}
    closeDB(db);
	return results;
}

//  copy and pasted from the internet. Not super sure what the heck its doing. 
int PokedexDB::callback(void* results, int argc, char** argv, char** azColName) {
    // using void* to handle results, 
    // cast results into a vector of vector of strings pointer.
    auto* res = static_cast<std::vector<std::vector<std::string>>*>(results);

    //handle any null values by replacing them with strings before adding to result
    std::vector<std::string> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    res->emplace_back(row); 
    
    return 0;
 }

void PokedexDB::printSQLresults(std::vector<std::vector<std::string>>* results) {
    for (const auto& row : (*results)) {
        for (const auto& col : row) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexDB::cleanup() {
    if (results) {
        delete results;
        results = nullptr;
    }

    if (zErrMsg) {
        sqlite3_free(zErrMsg); 
        zErrMsg = nullptr; 
    }

    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
    
}


