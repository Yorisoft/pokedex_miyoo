#include <iostream>
#include <vector>
#include <string>
#include "sqlite/sqlite3.h"
#include "Pokedex.h"

bool Pokedex::isTestMode = false;
const std::string Pokedex::MAIN_DB_PATH = "res/db/pokedex.sqlite";
const std::string Pokedex::TEST_DB_PATH = "../res/db/pokedex.sqlite";
std::string Pokedex::gameVersion = "red";
int Pokedex::languageVersion = 9;
std::vector<std::vector<std::string>>* Pokedex::results = new std::vector<std::vector<std::string>>();
sqlite3* Pokedex::db;
int Pokedex::rc;   
char* Pokedex::zErrMsg;


Pokedex::Pokedex() {
   /* this->rc = this->openDB(this->DB_PATH);
    if (this->rc != SQLITE_OK) {
        std::cout << "Database did not open.. Creating Pokedex without data.." << std::endl;
    }*/
}

int Pokedex::openDB() {
    rc = sqlite3_open(getDBpath().c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cout << "SQLite could not open file! SQLite Error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }
    return rc;
}

void Pokedex::closeDB(sqlite3* db) {
    sqlite3_close(db);
}

const std::string Pokedex::getDBpath() {
    return isTestMode ? TEST_DB_PATH : MAIN_DB_PATH;
   // return TEST_DB_PATH;
}

void Pokedex::enableTestMode(bool isTest) { // Used for changing directory paths between testing and live environments
    isTestMode = isTest;
}

std::vector<std::vector<std::string>>* Pokedex::executeSQL(const std::string* SQL_STATEMENT) {
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
    // For testing, delete whenever
    // std::cout << SQLstatement << std::endl << std::endl;
	if (sqlite3_exec(db, SQLstatement.c_str(), callback, results, &zErrMsg) != SQLITE_OK || rc != SQLITE_OK) {
		std::cout << "SQL error: " << zErrMsg << std::endl;
		sqlite3_close(db);
        return {};
	}
    // For testing, delete whenever
    // printSQLresults(results);
    closeDB(db);
	return results;
}

//  copy and pasted from the internet. Not super sure what the heck its doing. 
int Pokedex::callback(void* results, int argc, char** argv, char** azColName) {
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

void Pokedex::printSQLresults(std::vector<std::vector<std::string>>* results) {
    for (const auto& row : (*results)) {
        for (const auto& col : row) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void Pokedex::cleanup() {
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


