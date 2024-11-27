#ifndef POKEMON_H
#define POKEMON_H

#include<iostream>
#include<string>
#include<vector>

class Pokemon {
private:
	std::string
		name, genus,
		typeA, typeB,	
		flavorText;
	unsigned short
		id, 
		weight, height,
		healthPoint, attack, defense,
		specialAttack, specialDefense, speed,
		evolutionChainID, evolvesFromSpeciesID;

	std::vector<double> genderRates;	
	std::vector<std::string> abilities;
	std::vector<std::string> routes;
	
	void setMemberVaribles(std::vector<std::vector<std::string>>*);

public:
	Pokemon(const int);
	Pokemon(std::string*);
	Pokemon(std::vector<std::vector<std::string>>*);
	~Pokemon();
	
	// getters

	unsigned short getID() const;
	std::string getName() const;
	std::string getGenus() const;
	std::vector<std::string> getTypes();
	unsigned short getWeight() const;
	unsigned short getHeight() const;
	std::vector<double> getGenderRates() const;
	std::string getFlavorText() const;
	std::vector<unsigned short> getBasicStats() const;
	unsigned short getHP() const;
	unsigned short getAttack() const;
	unsigned short getDefense() const;
	unsigned short getSpecialAttack() const;
	unsigned short getSpecialDefense() const;
	unsigned short getSpeed() const;
	std::string getRoutes();
	std::string getEvolutionLine();
	std::string getAbilities();


	// setters

	void setID(const int);
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
	void setRoutes(const std::vector<int>);
	void setEvolutionLine(const std::vector<int>);
	void setAbilities(const std::vector<std::string>);

};

#endif // Pokemon.h

/*std::string Pokemon::getName(const int ID) {

	return;
}*/