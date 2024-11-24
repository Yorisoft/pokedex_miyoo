#ifndef POKEMON_H
#define POKEMON_H

#include<iostream>
#include<string>
#include<vector>

class Pokemon {
private:
	std::string
		name,
		typeA, typeB,
		genders, flavorText;
	unsigned short
		id,
		evolutionChainID,
		evolvesFromSpeciesID,
		healPoint,
		attack,
		defence,
		specialAttack,
		specialDefence,
		speed;
	
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
	std::string getName();
	std::vector<std::string> getTypes();
	std::string getGenders();
	std::string getFlavorText();
	unsigned short getID();
	unsigned short getHP();
	unsigned short getAttack();
	unsigned short getDefence();
	unsigned short getSpecialAttack();
	unsigned short getSpecialDefence();
	unsigned short getSpeed();
	std::string getRoutes();
	std::string getEvolutionLine();
	std::string getAbilities();


	// setters
	void setName(const std::string&);
	void setTypes(const std::string);
	void setTypes(const std::string, const std::string);
	void setGenders();
	void setFlavorText(const std::string);
	void setID(const int);
	void setHP(const int);
	void setAttack(const int);
	void setDefence(const int);
	void setSpecialAttack(const int);
	void setSpecialDefence(const int);
	void setSpeed(const int);
	void setRoutes(const std::vector<int>);
	void setEvolutionLine(const std::vector<int>);
	void setAbilities(const std::vector<std::string>);

};

#endif // Pokemon.h

/*std::string Pokemon::getName(const int ID) {

	return;
}*/
