#ifndef POKEDEXDB_H
#define POKEDEXDB_H

#include <iostream> 
#include <vector>
#include <string>
#include"sqlite/sqlite3.h"


class PokedexDB {
private:

	static std::vector<std::vector<std::string>>* results;
    static sqlite3* db;
	static int rc;   
    static char* zErrMsg;
	static bool isTestMode;
	static const std::string MAIN_DB_PATH;
	static const std::string TEST_DB_PATH;

	static int languageID;
	static int versionGroupID;
	static int versionID;
	static int generationID;
    static int pokemonID;
    static int moveID;
	static int evoChainID;
	static std::string pokemonIdentifier;
	static int audioID;

	static const std::string getDBpath();
	static int openDB();
	static void closeDB(sqlite3* db);
	static void cleanup();

public:
	PokedexDB();
	~PokedexDB();
	static std::vector<std::vector<std::string>>* executeSQL(const std::string*);
	static void printSQLresults(std::vector<std::vector<std::string>>*);
	static void enableTestMode(bool);  // To turn on test mode
	static void setLanguageID(int);
	static void setGameIdentifier(std::string&);
	static void setPokemonIdentifier(std::string&);
	static std::string getPokemonIdentifier();
	static void setRegionID(int);
	static void setGenerationID(int);
	static void setVersionGroupID(int);
	static void setVersionID(int);
	static void setPokemonID(int);
	static void setMoveID(int);
	static void setEvoChainID(int);
	static void setAudioID(int);
	static int getAudioID();
};

#endif