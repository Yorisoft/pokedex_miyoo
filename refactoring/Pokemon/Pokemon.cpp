#include"Pokemon.h"
#include"PokedexDB.h"
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
	this->genderRates = new std::vector<double>();
	this->abilities = new std::vector<std::string>();
	this->routes = new std::vector<std::vector<std::string>>();
	// call pokedex to pokedex for data

	// find position of string to replace with pokemon name
	std::string poke_SQLstatement = SQL_getPokemonByName;
	std::string route_SQLstatement = SQL_getPokemonRoutesByName;

	std::vector<std::vector<std::string>> pokeData = this->queryForPokeData(*pokemon, poke_SQLstatement);	
	std::vector<std::vector<std::string>> routeData = this->queryForPokeData(*pokemon, route_SQLstatement);
	
	this->setMemberVaribles(&pokeData, &routeData);
}

Pokemon::Pokemon(std::vector<std::vector<std::string>>* DBresults) {

}

std::vector<std::vector<std::string>> Pokemon::queryForPokeData(std::string& pName, std::string& sqlStatement) {
	size_t pos = sqlStatement.find("${pokemon_name}");
	while (pos != std::string::npos) {
		// filter by pokemon name
		sqlStatement.replace(pos, std::string("${pokemon_name}").length(), pName);
		pos = sqlStatement.find("${pokemon_name}");
	}
	
	// call databse to pokedex for data
	std::vector<std::vector<std::string>>* results = PokedexDB::executeSQL(&sqlStatement);
	if (!results || results->empty()) {
		std::cout << "Pokemon(std::string*): " << std::endl
			<< "Could not execute sql statement. result is null or empty." << std::endl;
	}
	return *results;
}

void Pokemon::setMemberVaribles(std::vector<std::vector<std::string>>* pokemon, std::vector<std::vector<std::string>>* routes) {
	// set id
	this->setID(std::stoi((*pokemon)[0][0]));
	
	//set pokemon name
	this->setName((*pokemon)[0][1]);
	
	//set pokemon genus
	this->setGenus((*pokemon)[0][2]);

	//set pokemon types
	//setup vector<string> for types
	std::vector<std::string> types;
	if((*pokemon)[0][2] != "NULL" && (*pokemon)[0][3] != "None" )
		types.push_back((*pokemon)[0][3]);
	if((*pokemon)[0][3] != "NULL" && (*pokemon)[0][4].find("None") )
		types.push_back((*pokemon)[0][4]);
	this->setTypes(types);

	//set hight and weight
	if ((*pokemon)[0][5] != "NULL" && (*pokemon)[0][5] != "None")
		this->setWeight(std::stoi((*pokemon)[0][5]));
	if((*pokemon)[0][6] != "NULL" && (*pokemon)[0][6] != "None" )
		this->setHeight(std::stoi((*pokemon)[0][6]));

	//set abilities
	std::vector<std::string> pAbilities;
	for (int i = 7; i <= 9; i++) {	
		if ((*pokemon)[0][i] != "NULL" && (*pokemon)[0][i] != "None") {
			pAbilities.push_back((*pokemon)[0][i]);
		}
	}
	//abilities(new std::vector<std::string>());
	this->setAbilities(pAbilities);

	//set pokemon gender rates
	//pass gender rate ID
	std::string genderRate = (*pokemon)[0][10];
	this->setGenderRates(std::stoi(genderRate));

	// set flavor text
	std::string flavorText = (*pokemon)[0][11];
	this->setFlavorText(flavorText);

	// index 9 is game versoin

	//set basic stats
	// index 13 - 18 correspond to HP, Atck, Def, Sp. Atk, Sp. Def, Spd
	std::vector<unsigned short>* basicStats = new std::vector<unsigned short>; 
	for (int i = 13; i < 19; i++) {
			
		if ((*pokemon)[0][i] != "NULL" && (*pokemon)[0][i] != "None") {
			(*basicStats).push_back(std::stoi((*pokemon)[0][i]));
		}
	}
	this->setBasicStats(basicStats);

	//set evolution chain Id
	if ((*pokemon)[0][19] != "NULL" && (*pokemon)[0][19] != "None") {
		unsigned short evoChainId = std::stoi((*pokemon)[0][19]);
		this->setEvolutionChainID(evoChainId);
	}

	// set evolves from species Id
	if ((*pokemon)[0][20] != "NULL" && (*pokemon)[0][20] != "None") {
		unsigned short evoFromSpeciesId = std::stoi((*pokemon)[0][20]);
		this->setEvolvesFromSpeciesID(evoFromSpeciesId);
	}

	//set routes ptr
	this->setRoutes(*routes);
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

std::string Pokemon::getName() const {
	return this->name;
}

void Pokemon::setGenus(const std::string& genus) {
	this->genus = genus;
}

std::string Pokemon::getGenus() const {
	return this->genus;
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

void Pokemon::setWeight(const unsigned short w) {
	this->weight = w;
}

unsigned short Pokemon::getWeight() const {
	return this->weight;
}

void Pokemon::setHeight(const unsigned short h) {
	this->height = h;
}

unsigned short Pokemon::getHeight() const {
	return this->height;
}

void Pokemon::setAbilities(const std::vector<std::string> a) {
	if (!(*this->abilities).empty()) {
		(*this->abilities).clear();
	}

	for (int i = 0; i < a.size(); i++) {
		(*this->abilities).push_back(a[i]);
	}
}

std::vector<std::string>* Pokemon::getAbilities() const {
	return this->abilities;
}

void Pokemon::setGenderRates(const double genderRateID) {
	// if vector not empty, then empty it
	if (!(*this->genderRates).empty())
		(*this->genderRates).clear();
	// Calculate female gender rate = fgenderRate
	// subtrace fgenderRate from 100 = mgenderRate
	(*this->genderRates).push_back((genderRateID / 8.0) * 100.0);
	(*this->genderRates).push_back(100.0 - (*this->genderRates)[0]);
}

std::vector<double>* Pokemon::getGenderRates() const {
	return this->genderRates;
}

void Pokemon::setFlavorText(const std::string& fText) {
	this->flavorText = fText;
}

std::string Pokemon::getFlavorText() const {
	return this->flavorText;
}

void Pokemon::setBasicStats(const std::vector<unsigned short>* stats) {
	// set HP
	this->setHP((*stats)[0]);
	this->setAttack((*stats)[1]);
	this->setDefense((*stats)[2]);
	this->setSpecialAttack((*stats)[3]);
	this->setSpecialDefense((*stats)[4]);
	this->setSpeed((*stats)[5]);
}
std::vector<unsigned short> Pokemon::getBasicStats() const {
	std::vector<unsigned short> stats;
	stats.push_back(this->healthPoint);
	stats.push_back(this->attack);
	stats.push_back(this->defense);
	stats.push_back(this->specialAttack);
	stats.push_back(this->specialDefense);
	stats.push_back(this->speed);

	return stats;
}

//setter and getter for HP
void Pokemon::setHP(const unsigned short i) {
	this->healthPoint = i;
}
unsigned short Pokemon::getHP() const {
	return this->healthPoint;
}
//setter and getter for Attack
void Pokemon::setAttack(const unsigned short a){
	this->attack = a;
}
unsigned short Pokemon::getAttack() const {
	return this->attack;
}
//setter and getter for Defense
void Pokemon::setDefense(const unsigned short d){
	this->defense = d;
}
unsigned short Pokemon::getDefense() const {
	return this->defense;
}
//setter and getter for Special Attack
void Pokemon::setSpecialAttack(const unsigned short spa){
	this->specialAttack = spa;
}
unsigned short Pokemon::getSpecialAttack() const {
	return this->specialAttack;
}
//setter and getter for Special Defense
void Pokemon::setSpecialDefense(const unsigned short spd){
	this->specialDefense = spd;
}
unsigned short Pokemon::getSpecialDefense() const {
	return this->specialDefense;
}
//setter and getter for Speed
void Pokemon::setSpeed(const unsigned short s){
	this->speed = s;
}
unsigned short Pokemon::getSpeed() const{
	return this->speed;
}

void Pokemon::setEvolutionChainID(const unsigned short ID) {
	this->evolutionChainID = ID;
}
unsigned short Pokemon::getEvolutionChainID() const {
	return this->evolutionChainID;
}

void Pokemon::setEvolvesFromSpeciesID(const unsigned short ID) {
	this->evolvesFromSpeciesID = ID;
}
unsigned short Pokemon::getEvolvesFromSpeciesID() const {
	return this->evolvesFromSpeciesID;
}

void Pokemon::setRoutes(const std::vector<std::vector<std::string>> routes) {
	if (!(*this->routes).empty())
		(*this->routes).clear();
	for (auto route : routes) {
		(*this->routes).push_back(route);
	}
}
std::vector<std::vector<std::string>>* Pokemon::getRoutes() const {
	return this->routes;
}

Pokemon::~Pokemon() {
	delete abilities;
	delete genderRates;
	delete routes;

}
