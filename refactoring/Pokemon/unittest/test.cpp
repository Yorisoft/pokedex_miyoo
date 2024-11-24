#include "pch.h"

class PokemonFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up your test environment here
		Pokedex::enableTestMode(true);

		std::string testPokemonName = "charmander";
		p = new Pokemon(&testPokemonName);
    }

    void TearDown() override {
        // Clean up any resources used in the tests 
        delete p;
    }
public:
    Pokemon* p;
    std::string testPokemonName;
    std::string type_a = "fire";
    std::string type_b = "flying";
};

TEST_F(PokemonFixture, setNameVoid) {
    p->setName("Bulbasaur");
    EXPECT_EQ(p->getName(), "Bulbasaur");
}

TEST_F(PokemonFixture, getNameReturnString) {
  EXPECT_EQ(p->getName(), "charmander");
}

TEST_F(PokemonFixture, setTypeAConstString) {
    p->setTypes(type_a);
    std::vector<std::string> types = p->getTypes();
    EXPECT_EQ(types[0], type_a);
}

TEST_F(PokemonFixture, setTypeAConstString) {
    p->setTypes(type_a);
    std::vector<std::string> types = p->getTypes();
    EXPECT_EQ(types[0], type_a);
}