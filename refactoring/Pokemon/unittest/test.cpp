#include "pch.h"

class PokemonFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up your test environment here
		PokedexDB::enableTestMode(true);

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
    std::vector<std::string> types;
};

// Using Microsoft unittest naming convention while using a Google unittest library..
// 
// The name of your test should consist of three parts:
//
// The name of the method being tested.
// The scenario under which it's being tested.
// The expected behavior when the scenario is invoked.

TEST_F(PokemonFixture, getID_ReturnIdAsInt) {
    EXPECT_EQ(p->getID(), 4); // charmander is pokemon in fixture. ID should be 4.
}

TEST_F(PokemonFixture, setID_Int_IdSetToInt) {
    int newId = 100;
    p->setID(newId);   

    EXPECT_EQ(p->getID(), 100);
}

TEST_F(PokemonFixture, getName_ReturnNameAsString) {
  EXPECT_EQ(p->getName(), "charmander");
}

TEST_F(PokemonFixture, setName_String_NameSetToString) {
    p->setName("Bulbasaur");
    EXPECT_EQ(p->getName(), "Bulbasaur");
}

TEST_F(PokemonFixture, getGenus_ReturnGenusAsString) {
    std::string currentGenus = p->getGenus();
    EXPECT_EQ(p->getGenus(), "Lizard Pok\xC3\xA9mon"); // UTF-8 encoding for 'é'. 'é' can cause test to fail. 
}                                              

TEST_F(PokemonFixture, setGenus_String_GenusSetToString) {
    std::string newGenus = "Test Genus";

    p->setGenus(newGenus);
    EXPECT_EQ(p->getGenus(), newGenus);
}                                              

TEST_F(PokemonFixture, getTypes_ReturnTypesAsStringVector) {
    std::vector<std::string> currentTypes = p->getTypes();
       
    EXPECT_EQ(currentTypes[0], "fire"); // default pokemon in PokemonFixture is Charmander
    EXPECT_EQ(currentTypes.size(), 1); 
}

TEST_F(PokemonFixture, setTypes_StringVectorOneValue_TypeASetToValue) {
    //set new type
    PokemonFixture::types.push_back("water");
    p->setTypes(PokemonFixture::types);

    std::vector<std::string> currentTypes = p->getTypes();
    EXPECT_EQ(currentTypes[0], PokemonFixture::types[0]);
    EXPECT_EQ(currentTypes.size(), 1); 
}

TEST_F(PokemonFixture, setTypes_StringVectorTwoValues_TypeAAndTypeBSetToValues) {
    //set new types
    PokemonFixture::types.push_back("water");
    PokemonFixture::types.push_back("electric");
    p->setTypes(PokemonFixture::types);

    // get current types
    std::vector<std::string> currentTypes = p->getTypes();

    // test that current types == new types
    for (int i = 0; i < currentTypes.size(); i++) {
        EXPECT_EQ(currentTypes[i], PokemonFixture::types[i]); 
    }
     EXPECT_EQ(currentTypes.size(), 2); 
}

TEST_F(PokemonFixture, getGenderRates_ReturnsGenderRatesDoubleVector) {
    std::vector<double>* currentGenderRates = p->getGenderRates();

    EXPECT_EQ((*currentGenderRates)[0], 12.5);
    EXPECT_EQ((*currentGenderRates)[1], 87.5);
} 

TEST_F(PokemonFixture, setGenderRates_Int_CalculatesGenderRates) {
    const double newGenderRateID = 4;
    p->setGenderRates(newGenderRateID);

    std::vector<double>* currentGenderRates = p->getGenderRates();

    EXPECT_EQ((*currentGenderRates)[0], 50);
    EXPECT_EQ((*currentGenderRates)[1], 50);
}

TEST_F(PokemonFixture, getFlavorText_ReturnFlavorTextAsString) {
    std::string currentFText = p->getFlavorText();

    EXPECT_EQ(currentFText, "Obviously prefers hot places. When it rains, steamis said to spout from the tip of its tail."); // not sure what to do about that ^L.. Im positive it will cause issues. 
}

TEST_F(PokemonFixture, setFlavorText_String_SetFlavorTextToString) {
    std::string newFlavorText = "New Flavor Text";
    p->setFlavorText(newFlavorText);

    std::string currentFText = p->getFlavorText();

    EXPECT_EQ(currentFText, newFlavorText);
}

TEST_F(PokemonFixture, getBasicStats_ReturnBasicStatsAsUnsignedShortVector) {
    std::vector<unsigned short> testBasicStats;  // fill vector with known stats for charmander
    testBasicStats.push_back(39);
    testBasicStats.push_back(52);
    testBasicStats.push_back(43);
    testBasicStats.push_back(60);
    testBasicStats.push_back(50);
    testBasicStats.push_back(65);

    std::vector<unsigned short> currentBasicStats = p->getBasicStats();

    for (int i = 0; i < testBasicStats.size(); i++) {
        EXPECT_EQ(testBasicStats[i], currentBasicStats[i]);
    }
}

TEST_F(PokemonFixture, setBasicStats_Vector_SetBasicStatsToUnsignedShortVector) {
    std::vector<unsigned short> newBasicStats;  // fill vector with known stats for charmander
    newBasicStats.push_back(300);
    newBasicStats.push_back(300);
    newBasicStats.push_back(300);
    newBasicStats.push_back(300);
    newBasicStats.push_back(300);
    newBasicStats.push_back(300);

    p->setBasicStats(&newBasicStats);
    std::vector<unsigned short> currentBasicStats = p->getBasicStats();

    for (int i = 0; i < currentBasicStats.size(); i++) {
        EXPECT_EQ(newBasicStats[i], currentBasicStats[i]);
    }
}

TEST_F(PokemonFixture, getStatsIndividually_ReturnIndividualStatsAsUnsignedShort) {
    EXPECT_EQ(p->getHP(), 39);
    EXPECT_EQ(p->getAttack(), 52);
    EXPECT_EQ(p->getDefense(), 43);
    EXPECT_EQ(p->getSpecialAttack(), 60);
    EXPECT_EQ(p->getSpecialDefense(), 50);
    EXPECT_EQ(p->getSpeed(), 65);
}

TEST_F(PokemonFixture, setStatsIndividually_SetIndividualStatsToUnsignedShort) {
    p->setHP(300);
    p->setAttack(300);
    p->setDefense(300);
    p->setSpecialAttack(300);
    p->setSpecialDefense(300);
    p->setSpeed(300);
    
    EXPECT_EQ(p->getHP(), 300);
    EXPECT_EQ(p->getAttack(), 300);
    EXPECT_EQ(p->getDefense(), 300);
    EXPECT_EQ(p->getSpecialAttack(), 300);
    EXPECT_EQ(p->getSpecialDefense(), 300);
    EXPECT_EQ(p->getSpeed(), 300);
}

TEST_F(PokemonFixture, getWeight_ReturnWeightAsUnsignedShort) {
    EXPECT_EQ(p->getWeight(), 85);
}

TEST_F(PokemonFixture, setWeight_UnsignedShort_SetWeightToUnsignedShort) {
    p->setWeight(300);
    
    EXPECT_EQ(p->getWeight(), 300);
}

TEST_F(PokemonFixture, getHeight_ReturnHeightAsUnsignedShort) {
    EXPECT_EQ(p->getHeight(), 6);
}

TEST_F(PokemonFixture, setHeight_UnsignedShort_SetWeightToUnsignedShort) {
    p->setHeight(300);
    
    EXPECT_EQ(p->getHeight(), 300);
}

TEST_F(PokemonFixture, getAbilities_ReturnAbilitiesAsVectorPrt) {
    std::vector<std::string>* currentAbilities = p->getAbilities();

    EXPECT_EQ((*currentAbilities)[0], "blaze");
    EXPECT_EQ((*currentAbilities)[1], "solar-power");
}

TEST_F(PokemonFixture, setAbilities_VectorString_SetAbilitiesToVectorString) {
    std::vector<std::string> newAbilities;
    newAbilities.push_back("test_ability_1");
    newAbilities.push_back("test_ability_2");
    newAbilities.push_back("test_ability_3");

    p->setAbilities(newAbilities);
    
    std::vector<std::string>* currentAbilities = p->getAbilities();
   
    for (int i = 0; i < (*currentAbilities).size(); i++) {
        EXPECT_EQ((*currentAbilities)[i], newAbilities[i]);
    }
}

TEST_F(PokemonFixture, getRoutes_ReturnRoutesAsVectorVectorStringPrt) {
    std::vector<std::vector<std::string>>* currentRoutes = p->getRoutes();
    std::vector<std::vector<std::string>> testRoutes;

    std::vector<std::string> routes;
    routes.push_back("4");
    routes.push_back("charmander");
    routes.push_back("red");
    routes.push_back("pallet-town");
    routes.push_back("gift");
    routes.push_back("5");
    routes.push_back("5");
    routes.push_back("100");

    testRoutes.push_back(routes);

    for (int r = 0; r < (*currentRoutes).size(); r++) {
        for (int c = 0; c < (*currentRoutes)[0].size(); c++) {
            EXPECT_EQ((*currentRoutes)[r][c], testRoutes[r][c]);
        }
    }
}

TEST_F(PokemonFixture, setRoutes_VetorVectorString_SetRoutesToVectorVectorString) {
    std::vector<std::vector<std::string>> testRoutes;
    std::vector<std::string> routes;

    routes.push_back("TEST");
    routes.push_back("TEST");
    routes.push_back("TEST");
    routes.push_back("TEST");
    routes.push_back("TEST");
    routes.push_back("TEST");
    routes.push_back("TEST");
    routes.push_back("TEST");
    testRoutes.push_back(routes);
    p->setRoutes(testRoutes);

    std::vector<std::vector<std::string>>* currentRoutes = p->getRoutes();
    for (int r = 0; r < (*currentRoutes).size(); r++) {
        for (int c = 0; c < (*currentRoutes)[0].size(); c++) {
            EXPECT_EQ((*currentRoutes)[r][c], testRoutes[r][c]);
        }
    }
}

TEST_F(PokemonFixture, getEvolutionChainId_ReturnEvolutionChainIdAsUnsignedShort) {
    EXPECT_EQ(p->getEvolutionChainID(), 2);
}

TEST_F(PokemonFixture, setEvolutionChainId_UnsignedShort_SetEvolutionChainIdToUnsignedShort) {
    p->setEvolutionChainID(300);
    EXPECT_EQ(p->getEvolutionChainID(), 300);
}

TEST_F(PokemonFixture, getEvolvesFromSpeciesID_ReturnEvolvesFromSpeciesIDAsUnsingedShort) {
    std::string newPokeName = "pikachu";
    delete p;
    p = new Pokemon(&newPokeName);

    EXPECT_EQ(p->getEvolvesFromSpeciesID(), 172);
}

TEST_F(PokemonFixture, setEvolvesFromSpeciesID_UnsignedShort_SetsEvolvesFromSpeciesIDToUnsingedShort) {
    p->setEvolvesFromSpeciesID(300);
    EXPECT_EQ(p->getEvolvesFromSpeciesID(), 300);
}

// Main function to run all tests and generate XML report
int main(int argc, char** argv) {
    // Set the output to XML format
    // get unique date stamp for each report. 
    auto basePath = std::filesystem::current_path();
    std::string testReportPath = (basePath / "../unittest/reports/pokemon_test_report.xml").string();
    ::testing::GTEST_FLAG(output) = "xml:" + testReportPath;

    // Set verbosity level to verbose
    //::testing::GTEST_FLAG(verbose) = "info";  // or "detailed"

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

