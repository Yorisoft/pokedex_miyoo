#include<iostream>
#include<vector>
#include<string>
#include"sqlite/sqlite3.h"
#include"Pokemon.h"
#include"Pokedex.h"
#include"SQLstatements.h"

int main() {
	std::string pokemonName= "charmander";
	Pokemon* pokemon = new Pokemon(&pokemonName);

	// for testing.
	std::cout << "This is pokemon object 'name' member variable: " << pokemon->getName() << std::endl;
	std::cin.get();

	return 0;
}