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
	// set id
	this->setID(std::stoi((*pokemon)[0][0]));
	
	//set pokemon name
	this->setName((*pokemon)[0][1]);

	//set pokemon types
	//setup vector<string> for types
	std::vector<std::string> types;
	if((*pokemon)[0][2] != "NULL" && (*pokemon)[0][2] != "None" )
		types.push_back((*pokemon)[0][2]);
	if((*pokemon)[0][3] != "NULL" && (*pokemon)[0][3].find("None") )
		types.push_back((*pokemon)[0][3]);
	this->setTypes(types);

	//set pokemon gender rates
	//pass gender rate ID
	std::string genderRate = (*pokemon)[0][4];
	this->setGenderRates(std::stoi(genderRate));

	// set flavor text
	std::string flavorText = (*pokemon)[0][5];
	this->setFlavorText(flavorText);
}

void Pokemon::setID(const int ID) {
	this->id = static_cast<unsigned short>(ID);
}


unsigned short Pokemon::getID() const {
	return this->id;
}

void Pokemon::setName(const std::string& name) {
	this->name = name;
}

std::string Pokemon::getName() {
	return this->name;
}

void Pokemon::setTypes(const std::vector<std::string>& types) {
	if (types.size() != 2) {
		this->typeA = types[0];
	}
	else {
		this->typeA = types[0];
		this->typeB = types[1];
	}
}

std::vector<std::string> Pokemon::getTypes() {
	std::vector<std::string> types;

	if (typeB.size() < 1 ) {
		types.push_back(typeA);			
	}
	else {
		types.push_back(typeA);	
		types.push_back(typeB);
	}

	return types;
}

void Pokemon::setGenderRates(const double genderRateID) {
	// if vector not empty, then empty it
	if (!this->genderRates.empty())
		this->genderRates.clear();
	// Calculate female gender rate = fgenderRate
	// subtrace fgenderRate from 100 = mgenderRate
	this->genderRates.push_back((genderRateID / 8.0) * 100.0);
	this->genderRates.push_back(100.0 - this->genderRates[0]);
}

std::vector<double> Pokemon::getGenderRates() const {
	return this->genderRates;
}

void Pokemon::setFlavorText(const std::string& fText) {
	this->flavorText = fText;
}

std::string Pokemon::getFlavorText() const {
	return this->flavorText;
}


Pokemon::~Pokemon() {
	

}


	/*
	void setID(const int);
	void setHP(const int);
	void setAttack(const int);
	void setDefence(const int);
	void setSpecialAttack(const int);
	void setSpecialDefence(const int);
	void setSpeed(const int);
	*/
