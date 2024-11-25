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
    std::vector<std::string> types;
};


// Using Microsoft unittest naming convention while using a Google unittest library..
// 
// The name of your test should consist of three parts:
//
// The name of the method being tested.
// The scenario under which it's being tested.
// The expected behavior when the scenario is invoked.

TEST_F(PokemonFixture, givenGetName_ReturnNameAsString) {
  EXPECT_EQ(p->getName(), "charmander");
}

TEST_F(PokemonFixture, givenSetName_WhenStringName_ThenNameSetToString) {
    p->setName("Bulbasaur");
    EXPECT_EQ(p->getName(), "Bulbasaur");
}

TEST_F(PokemonFixture, givenGetTypes_ReturnTypesAsStringVector) {
    std::vector<std::string> currentTypes = p->getTypes();
       
    EXPECT_EQ(currentTypes[0], "fire"); // default pokemon in PokemonFixture is Charmander
    EXPECT_EQ(currentTypes.size(), 1); 
}

TEST_F(PokemonFixture, givenSetTypes_WhenStringVectorOneValue_ThenTypeASetToValue) {
    //set new type
    PokemonFixture::types.push_back("water");
    p->setTypes(PokemonFixture::types);

    std::vector<std::string> currentTypes = p->getTypes();
    EXPECT_EQ(currentTypes[0], PokemonFixture::types[0]);
    EXPECT_EQ(currentTypes.size(), 1); 
}

TEST_F(PokemonFixture, givenSetTypes_WhenStringVectorTwoValues_ThenTypeAAndTypeBSetToValues) {
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

TEST_F(PokemonFixture, givenSetGenderRates_WhenIntID_ThenCalculatesGenderRates) {
    const double newGenderRateID = 4;
    p->setGenderRates(newGenderRateID);

    std::vector<double> currentGenderRates = p->getGenderRates();

    EXPECT_EQ(currentGenderRates[0], 50);
    EXPECT_EQ(currentGenderRates[1], 50);
}

TEST_F(PokemonFixture, givenSetFlavorText_WhenString_ThenSetFlavorTextToString) {
    std::string newFlavorText = "New Flavor Text";
    p->setFlavorText(newFlavorText);

    std::string currentFText = p->getFlavorText();

    EXPECT_EQ(currentFText, newFlavorText);
}

TEST_F(PokemonFixture, givenSetID_WhenInt_ThenSetIdToInt) {
    int newId = 100;
    p->setID(newId);

    int currentID = p->getID();

    EXPECT_EQ(currentID, 100);
}
// Main function to run all tests and generate XML report
int main(int argc, char** argv) {
    // Set the output to XML format
    // get unique date stamp for each report. 
    auto basePath = std::filesystem::current_path();
    std::string testReportPath = (basePath / "../unittest/reports/pokemon_test_report.xml").string();
    ::testing::GTEST_FLAG(output) = "xml:" + testReportPath;

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

