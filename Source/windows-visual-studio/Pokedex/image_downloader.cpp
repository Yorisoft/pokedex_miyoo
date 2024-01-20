#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <curl/curl.h>
#include "sqlite/sqlite3.h"

using namespace std;

size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream) {
    size_t totalSize = size * nmemb;
    ((vector<char>*)stream)->insert(((vector<char>*)stream)->end(), (char*)ptr, (char*)ptr + totalSize);
    return totalSize;
}


int callback(void* data, int argc, char** argv, char** azColName) {
    vector<vector<string>>* results = static_cast<vector<vector<string>>*>(data);
    vector<string> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    results->push_back(row);
    return 0;
};


// Function to execute a SQL query and return the result as a string
vector<vector<string>> executeSQL(const string& sql) {
    vector<vector<string>> results;
    sqlite3* db;
    char* zErrMsg = 0;
    if (sqlite3_open("pokedex.sqlite", &db) != SQLITE_OK) {
        sqlite3_close(db);
        return results;
    }
    else {
        if (sqlite3_exec(db, sql.c_str(), callback, &results, &zErrMsg) != SQLITE_OK) {
            cerr << "SQL error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
        }
        sqlite3_close(db);
        return results;
    }
};

int main() {
    CURL* curl;
    CURLcode res;
    vector<char> readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        const string SQL_STRING_GET_POKEMONS_COUNT = "SELECT identifier FROM pokemon";
        vector<vector<string>> pokemon = executeSQL(SQL_STRING_GET_POKEMONS_COUNT);

        for (int i = 0; i < 898; i++) {
            string pokemon_name = pokemon[i][0]; // Assuming the name is at index 0
            cout << "pokemon_name : " << pokemon_name << endl;
            ostringstream oss_url;
            //oss_url << "https://img.pokemondb.net/sprites/black-white/normal/" << pokemon_name << ".png";
            string url = "https://img.pokemondb.net/sprites/sword-shield/icon/" + pokemon_name + ".png";
            cout << "url.c_str() : " << url.c_str() << endl;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // 0 disables verification
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // 0 disables verification
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            }
            else {
                ofstream outFile("resources/icons/" + pokemon_name + ".png", ios::out | ios::binary);
                outFile.write(&readBuffer[0], readBuffer.size());
            }
            readBuffer.clear();
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}
