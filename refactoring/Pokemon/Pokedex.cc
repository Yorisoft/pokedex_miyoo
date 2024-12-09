#include<iostream>
#include<vector>
#include<string>
#include"sqlite/sqlite3.h"
#include"Pokedex.h"
#include"Pokemon.h"
#include"PokedexDB.h"
#include"SQLstatements.h"

Pokedex::Pokedex() {
}

int Pokedex::OnExecute() {
	return 0;
}

int main(int argc, char* argv[]) {
	std::string pokemonName= "charmander";
	Pokemon* pokemon = new Pokemon(&pokemonName);

	// for testing.
	std::cout << "This is pokemon object 'name' member variable: " << pokemon->getName() << std::endl;
	std::cin.get();

	return 0;
}