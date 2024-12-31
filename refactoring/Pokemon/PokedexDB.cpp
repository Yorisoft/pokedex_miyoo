#include "PokedexDB.h"

bool PokedexDB::isTestMode = false;
const std::string PokedexDB::MAIN_DB_PATH = "res/db/pokedex.sqlite";
const std::string PokedexDB::TEST_DB_PATH = "../res/db/pokedex.sqlite";
std::string PokedexDB::gameIdentifier = "red";
std::string PokedexDB::pokemonIdentifier = "charmander";
int PokedexDB::languageID = 9;
int PokedexDB::regionID = 1;
int PokedexDB::generationID = 1;
int PokedexDB::versionGroupID = 1;
int PokedexDB::moveID = 1;
int PokedexDB::evoChainID = 1;
std::vector<std::vector<std::string>>* PokedexDB::results;
sqlite3* PokedexDB::db;
int PokedexDB::rc;   
char* PokedexDB::zErrMsg;

PokedexDB::PokedexDB() {
 }

PokedexDB::~PokedexDB() {
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
    std::string SQLstatement = *SQL_STATEMENT;

    // Open SQLite database
    rc = openDB();
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    // Prepare the statement
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, SQL_STATEMENT->c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return {};
    }

    // Bind parameters
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":language_id"), languageID);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":generation_id"), generationID);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":region_id"), regionID);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":version_group_id"), versionGroupID);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":game_version"), gameIdentifier.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":pokemon_identifier"), pokemonIdentifier.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":evo_chain_id"), evoChainID);

    // Ensure results is initialized
    if (!results) {
        results = new std::vector<std::vector<std::string>>();
    }

    results->clear();

    // Step through rows and collect results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::vector<std::string> row;
        int columnCount = sqlite3_column_count(stmt);

        for (int col = 0; col < columnCount; ++col) {
            const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, col));
            row.push_back(value ? value : "NULL"); // Handle NULL values
        }

        results->push_back(row);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to fetch results: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    closeDB(db);
    return results;
}

//std::vector<std::vector<std::string>>* PokedexDB::executeSQL(const std::string* SQL_STATEMENT) {
//    // change sql statement to be language specific
//    // find position of string to replace with languageVersion
//    std::string SQLstatement = *SQL_STATEMENT;
//    size_t pos;
//    pos = SQLstatement.find("${language_id}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${language_id}").length(), std::to_string(languageVersion));
//        pos = SQLstatement.find("${language_id}");
//    }
//
//    // change sql statement to be generatoin specific
//    // find position of string to replace with generationID
//    pos = SQLstatement.find("${generation_id}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${generation_id}").length(), std::to_string(generationID));
//        pos = SQLstatement.find("${generation_id}");
//    }
//
//    // change sql statement to be game version specific
//    // find position of string to replace with gameVersion
//    pos = SQLstatement.find("${game_version}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${game_version}").length(), gameVersion);
//        pos = SQLstatement.find("${game_version}");
//    }
//
//    // change sql statement to be game region specific
//    // find position of string to replace with regionVersion
//    pos = SQLstatement.find("${region_id}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${region_id}").length(), std::to_string(regionVersion));
//        pos = SQLstatement.find("${region_id}");
//    }
//
//    // change sql statement to be game version group specific
//    // find position of string to replace with regionVersion
//    pos = SQLstatement.find("${versionGroup_id}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${versionGroup_id}").length(), std::to_string(versionGroupID));
//        pos = SQLstatement.find("${versionGroup_id}");
//    }
//
//    // change sql statement to be pokemon specific
//    // find position of string to replace with pokemonIdentifier
//    pos = SQLstatement.find("${pokemon_identifier}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${pokemon_identifier}").length(), pokemonIdentifier);
//        pos = SQLstatement.find("${pokemon_identifier}");
//    }
//
//    // change sql statement to be move specific
//    // find position of string to replace with moveID
//    pos = SQLstatement.find("${move_id}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${move_id}").length(), std::to_string(moveID));
//        pos = SQLstatement.find("${move_id}");
//    }
//
//    // change sql statement to be evoChainID specific
//    // find position of string to replace with evoChainID
//    pos = SQLstatement.find("${evoChain_id}");
//    while (pos != std::string::npos) {
//        SQLstatement.replace(pos, std::string("${evoChain_id}").length(), std::to_string(evoChainID));
//        pos = SQLstatement.find("${evoChain_id}");
//    }
//
//    // clear any previous results
//    results->clear();
//
//    // Open SQLite database
//    rc = openDB();
//
//    // execute sql querry
//    if (sqlite3_exec(db, SQLstatement.c_str(), callback, results, &zErrMsg) != SQLITE_OK || rc != SQLITE_OK) {
//        std::cout << "SQL error: " << zErrMsg << std::endl;
//        sqlite3_close(db);
//        return {};
//    }
//    closeDB(db);
//    return results;
//}

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

void PokedexDB::setLanguageID(int version_id) {
    languageID = version_id;
}

void PokedexDB::setGameIdentifier(std::string& version_name) {
    gameIdentifier = version_name;
}

void PokedexDB::setRegionID(int version_id) {
    regionID = version_id;
}

void PokedexDB::setGenerationID(int gen_id) {
    generationID = gen_id;
}
void PokedexDB::setVersionGroupID(int group_version_id) {
    versionGroupID = group_version_id;
}
void PokedexDB::setMoveID(int move_ID) {
    moveID = move_ID;
}

void PokedexDB::setEvoChainID(int evo_chain_ID) {
    evoChainID = evo_chain_ID;
}

void PokedexDB::setPokemonIdentifier(std::string& pokemon_identifier) {
    pokemonIdentifier = pokemon_identifier;
}

std::string PokedexDB::getPokemonIdentifier() {
    return pokemonIdentifier;
}
