#include"Pokemon.h"

Pokemon::Pokemon() {
	std::vector<std::vector<std::string>>* results;

	this->genderRates = new std::vector<double>();
	this->abilities = new std::vector<std::vector<std::string>>();
	this->routes = new std::vector<std::vector<std::string>>();
	this->evoChain = new std::vector<std::vector<std::string>>();

	// set id
	results = PokedexDB::executeSQL(&SQL_getPokeRegionalID);
	setID(std::stoi((*results)[0][0]));
	// set regionalID
	setRegionalID(std::stoi((*results)[0][1]));

	// set name;
	results = PokedexDB::executeSQL(&SQL_getPokeName);
	setName((*results)[0][1]);

	// set types;
	results = PokedexDB::executeSQL(&SQL_getPokeTypes);
	setTypes((*results)[0]);

	// set genus;
	results = PokedexDB::executeSQL(&SQL_getPokeGenus);
	setGenus((*results)[0][1]);

	// set height & weight;
	results = PokedexDB::executeSQL(&SQL_getPokeHW);
	setHeight(std::stoi((*results)[0][0]));
	setWeight(std::stoi((*results)[0][1]));

	// set flavor text;
	results = PokedexDB::executeSQL(&SQL_getPokeFlavorText);
	setFlavorText((*results)[0][0]);

	// set gender Rates;
	results = PokedexDB::executeSQL(&SQL_getPokeGenderRates);
	setGenderRates(std::stoi((*results)[0][0]));

	// set stats;
	results = PokedexDB::executeSQL(&SQL_getPokeStats);
	std::vector<unsigned short>* stats = new std::vector<unsigned short>();
	for (std::vector<std::string> stat : *results) {
		stats->push_back(std::stoi(stat[0]));
	}
	setBasicStats(stats);

	// //set abilities;
	results = PokedexDB::executeSQL(&SQL_getPokeAbilities);
	setAbilities(*results);

	// set routes;
	results = PokedexDB::executeSQL(&SQL_getPokeRoutes);
	setRoutes(*results);

	// set evoChainID;
	results = PokedexDB::executeSQL(&SQL_getPokeEvoID);
	setEvolutionChainID(std::stoi((*results)[0][0]));

	PokedexDB::setEvoChainID(getEvolutionChainID());

	// set evoChain;
	results = PokedexDB::executeSQL(&SQL_getPokeEvoChain);
	setEvolutionChain(*results);
}

Pokemon::~Pokemon() {
	delete abilities;
	delete genderRates;
	delete routes;
	delete evoChain;
}

void Pokemon::setID(const int ID) {
	this->id = static_cast<unsigned short>(ID);
}
unsigned short Pokemon::getID() const {
	return this->id;
}

void Pokemon::setRegionalID(const int regional_id) {
	this->regionalID = static_cast<unsigned short>(regional_id);
}
unsigned short Pokemon::getRegionalID() const {
	return this->regionalID;
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
	double weightInPounds = w * 0.22;
	weightInPounds = std::ceil(weightInPounds * 10.0) / 10.0;

	// Store the rounded weight as a string with 1 decimal place
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(1) << weightInPounds;
	this->weight = stream.str();
}
std::string Pokemon::getWeight() const {
	return this->weight;
}

void Pokemon::setHeight(const unsigned short h) {
	double heightDecimeters = h * 0.1;         
	double heightInInches = heightDecimeters * 39.37;         
	int totalInches = static_cast<int>(std::round(heightInInches)); // Round to nearest inch

	int feet = totalInches / 12;       
	int inches = totalInches % 12;     

	this->height = std::to_string(feet) + '\'' + std::to_string(inches);
}
std::string Pokemon::getHeight() const {
	return this->height;
}

void Pokemon::setAbilities(std::vector<std::vector<std::string>>& a) {
	abilities->clear();
	for (int i = 0; i < a.size(); i++) {
        std::string replacement = " "; 
        std::string result = a[i][1];
        size_t pos = 0;
        while ((pos = result.find("\n", pos)) != std::string::npos) {
            result.replace(pos, 1, replacement);
            pos += replacement.length();
        }

        pos = 0;
        while ((pos = result.find("\r", pos)) != std::string::npos) {
            result.replace(pos, 1, replacement);
            pos += replacement.length();
        }

        pos = 0;
        while ((pos = result.find("\f", pos)) != std::string::npos) {
            result.replace(pos, 1, replacement);
            pos += replacement.length();
        }
		a[i][1] = result;
	}
	(*abilities) = a;
}
std::vector<std::vector<std::string>>* Pokemon::getAbilities() const {
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
	//for (char c : fText) {
	//	std::cout << static_cast<int>(c) << " "; // Print each character's ASCII value
	//}
	// Or to replace \n with a string:
	std::string replacement = " "; 
	std::string result = fText;
	size_t pos = 0;
	while ((pos = result.find("\n", pos)) != std::string::npos) {
		result.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = result.find("\r", pos)) != std::string::npos) {
		result.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = result.find("\f", pos)) != std::string::npos) {
		result.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	this->flavorText = result;
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
	std::string speedStr = std::to_string(s);
	size_t pos = 0;
	std::string replacement = ""; 
	while ((pos = speedStr.find("\n", pos)) != std::string::npos) {
		speedStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = speedStr.find("\r", pos)) != std::string::npos) {
		speedStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = speedStr.find("\f", pos)) != std::string::npos) {
		speedStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	this->speed = std::stoi(speedStr);
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

void Pokemon::setEvolutionChain(const std::vector<std::vector<std::string>> evo_chain) {
	if (!(*this->evoChain).empty())
		(*this->evoChain).clear();
	for (auto& evo : evo_chain) {
		(*this->evoChain).push_back(evo);
	}
}
std::vector<std::vector<std::string>>* Pokemon::getEvolutionChain() const {
	return this->evoChain;
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
	for (auto& route : routes) {
		(*this->routes).push_back(route);
	}
}
std::vector<std::vector<std::string>>* Pokemon::getRoutes() const {
	return this->routes;
}

