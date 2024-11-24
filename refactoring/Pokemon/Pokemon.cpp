#include"Pokemon.h"
#include"Pokedex.h"
#include"SQLstatements.h"
#include<iostream> 
#include<string>

Pokemon::Pokemon(const int ID) {
	// make a call to pokedex class 
	// use pokedex sqlite functions to get pokemon information. 

	// call function to set all of the member variables   
	

	//this->setMemberVaribles(results);

}

Pokemon::Pokemon(std::string* pokemon) {
	// call pokedex to pokedex for data

	// find position of string to replace with pokemon name
	std::string SQLstatement = SQL_getPokemonByName;
	size_t pos = SQLstatement.find("${pokemon_name}");
	if (pos != std::string::npos) {
		// filter by pokemon name
		SQLstatement.replace(pos, std::string("${pokemon_name}").length(), *pokemon);
	}
	
	// call databse to pokedex for data
	std::vector<std::vector<std::string>>* results = Pokedex::executeSQL(&SQLstatement);
	if (!results || results->empty()) {
		std::cout << "Pokemon(std::string*): " << std::endl
			<< "Could not execute sql statement. result is null or empty." << std::endl;
	}
	else {
		this->setMemberVaribles(results);
	}
}

Pokemon::Pokemon(std::vector<std::vector<std::string>>* DBresults) {

}

void Pokemon::setMemberVaribles(std::vector<std::vector<std::string>>* pokemon) {
	this->setName((*pokemon)[0][1]);
}

void Pokemon::setName(const std::string& name) {
	this->name = name;
}

std::string Pokemon::getName() {
	return this->name;
}

Pokemon::~Pokemon() {
	

}

void Pokemon::setTypes(const std::string type) {
	typeA = type;
}

void Pokemon::setTypes(const std::string type_A, const std::string type_B) {
	typeA = type_A;
	typeB = type_B;
}

std::vector<std::string> Pokemon::getTypes() {
	std::vector<std::string> types;
	types.push_back(typeA);
	types.push_back(typeB);
	return types;
}



	/*
		void setGenders();
	void setFlavorText(const std::string);
	void setID(const int);
	void setHP(const int);
	void setAttack(const int);
	void setDefence(const int);
	void setSpecialAttack(const int);
	void setSpecialDefence(const int);
	void setSpeed(const int);
	*/

