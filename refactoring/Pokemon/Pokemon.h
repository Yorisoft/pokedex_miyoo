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
		evolutionChainID,
		evolvesFromSpeciesID, 
		healthPoint,
		attack,
		defense,
		specialAttack,
		specialDefense,
		speed;

	std::vector<double> genderRates;

	
	std::vector<std::string> abilities;
	std::vector<std::string> natures;

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
	std::vector<double> getGenderRates() const;
	std::string getFlavorText() const;
	std::vector<unsigned short> getBasicStats() const;
	unsigned short getHP();
	unsigned short getAttack();
	unsigned short getDefense();
	unsigned short getSpecialAttack();
	unsigned short getSpecialDefense();
	unsigned short getSpeed();
	std::string getRoutes();
	std::string getEvolutionLine();
	std::string getAbilities();


	// setters

	void setID(const int);
	void setName(const std::string&);
	void setGenus(const std::string&);
	void setTypes(const std::vector<std::string>&);
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