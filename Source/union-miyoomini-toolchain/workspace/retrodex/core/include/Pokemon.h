#ifndef POKEMON_H
#define POKEMON_H

#include<iomanip>
#include<cmath>
#include<sstream>
#include"PokedexDB.h"
#include"SQLstatements.h"

class Pokemon {
private:
	std::string
		name, genus,
		weight, height,
		typeA, typeB,	
		flavorText;
	unsigned short
		id, regionalID, 
		healthPoint, attack, defense,
		specialAttack, specialDefense, speed,
		evolutionChainID, evolvesFromSpeciesID;

	std::vector<double>* genderRates;	
	std::vector<std::vector<std::string>>* abilities;
	std::vector<std::vector<std::string>>* routes;
	std::vector<std::vector<std::string>>* evoChain;
	
public:
	Pokemon();
	~Pokemon();

	// setters
	void setID(const int);
	void setRegionalID(const int);
	void setName(const std::string&);
	void setGenus(const std::string&);
	void setTypes(const std::vector<std::string>&);
	void setWeight(const unsigned short);
	void setHeight(const unsigned short);
	void setGenderRates(const double);
	void setFlavorText(const std::string&);
	void setBasicStats(const std::vector<unsigned short>*);
	void setHP(const unsigned short);
	void setAttack(const unsigned short);
	void setDefense(const unsigned short);
	void setSpecialAttack(const unsigned short);
	void setSpecialDefense(const unsigned short);
	void setSpeed(const unsigned short);
	void setAbilities(std::vector<std::vector<std::string>>&);
	void setRoutes(const std::vector<std::vector<std::string>>);
	void setEvolutionChainID(const unsigned short);
	void setEvolutionChain(const std::vector<std::vector<std::string>>);
	void setEvolvesFromSpeciesID(const unsigned short);

	// getters
	unsigned short getID() const;
	unsigned short getRegionalID() const;
	std::string getName() const;
	std::string getGenus() const;
	std::vector<std::string> getTypes();
	std::string getWeight() const;
	std::string getHeight() const;
	std::vector<double>* getGenderRates() const;
	std::string getFlavorText() const;
	std::vector<unsigned short> getBasicStats() const;
	unsigned short getHP() const;
	unsigned short getAttack() const;
	unsigned short getDefense() const;
	unsigned short getSpecialAttack() const;
	unsigned short getSpecialDefense() const;
	unsigned short getSpeed() const;
	std::vector<std::vector<std::string>>* getAbilities() const;
	std::vector<std::vector<std::string>>* getRoutes() const;
	unsigned short getEvolutionChainID() const;
	std::vector<std::vector<std::string>>* getEvolutionChain() const;
	unsigned short getEvolvesFromSpeciesID() const;
};
#endif // Pokemon.h
