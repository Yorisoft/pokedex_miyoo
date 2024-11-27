#include "SQLstatements.h"
#include<string>


// God bless ChatGPT for providing this.. this.. work of art.
const std::string SQL_getPokemonByName = R"(
		SELECT 
		ps.id AS pokemon_id, 
		ps.identifier AS pokemon_name, 
		MIN(REPLACE(psn.genus, CHAR(10), ' ')) AS genus,
		ta.identifier AS type_a, 
		COALESCE(tb.identifier, 'None') AS type_b, 
		p.weight, 
		p.height, 
		MAX(CASE WHEN pa.slot = 1 THEN a.identifier END) AS ability_1,
		MAX(CASE WHEN pa.slot = 2 THEN a.identifier END) AS ability_2,
		MAX(CASE WHEN pa.slot = 3 THEN a.identifier END) AS ability_3,
		ps.gender_rate, 
		MIN(REPLACE(f.flavor_text, CHAR(10), ' ')) AS flavor_text, 
		v.identifier AS game_version, 
		MAX(CASE WHEN s.stat_id = 1 THEN s.base_stat END) AS hp, 
		MAX(CASE WHEN s.stat_id = 2 THEN s.base_stat END) AS attack, 
		MAX(CASE WHEN s.stat_id = 3 THEN s.base_stat END) AS defense, 
		MAX(CASE WHEN s.stat_id = 4 THEN s.base_stat END) AS special_attack, 
		MAX(CASE WHEN s.stat_id = 5 THEN s.base_stat END) AS special_defense, 
		MAX(CASE WHEN s.stat_id = 6 THEN s.base_stat END) AS speed, 
		ps.evolution_chain_id, 
		ps.evolves_from_species_id
	FROM pokemon_species ps 
	JOIN pokemon_types pta ON ps.id = pta.pokemon_id AND pta.slot = 1 
	JOIN types ta ON pta.type_id = ta.id 
	LEFT JOIN pokemon_types ptb ON ps.id = ptb.pokemon_id AND ptb.slot = 2 
	LEFT JOIN types tb ON ptb.type_id = tb.id 
	JOIN pokemon_species_flavor_text f ON ps.id = f.species_id 
	JOIN versions v ON f.version_id = v.id 
	JOIN pokemon_stats s ON ps.id = s.pokemon_id 
	JOIN pokemon p ON ps.id = p.species_id  
	JOIN pokemon_abilities pa ON ps.id = pa.pokemon_id 
	JOIN abilities a ON pa.ability_id = a.id 
	JOIN pokemon_species_names psn ON ps.id = psn.pokemon_species_id
	WHERE ps.identifier = '${pokemon_name}' 
	AND v.identifier = '${game_version}'
	AND f.language_id = '${language_id}'  
	AND psn.local_language_id = '${language_id}' 
	GROUP BY ps.id, v.id, ta.identifier, tb.identifier, ps.gender_rate, p.weight, p.height, ps.evolution_chain_id, ps.evolves_from_species_id 
	ORDER BY ps.id, v.id;)";

const std::string SQL_getPokemonByNameTest =
	"SELECT "
	"ps.id AS pokemon_id, "
	"ps.identifier AS pokemon_name, "
	"MIN(REPLACE(psn.genus, CHAR(10), ' ')) AS species_classification, "
	"ta.identifier AS type_a, "
	"COALESCE(tb.identifier, 'None') AS type_b, "
	"p.weight, "
	"p.height, "
	"ps.gender_rate, "
	"MIN(REPLACE(f.flavor_text, CHAR(10), ' ')) AS flavor_text, "
	"v.identifier AS game_version, "
	"MAX(CASE WHEN s.stat_id = 1 THEN s.base_stat END) AS hp, "
	"MAX(CASE WHEN s.stat_id = 2 THEN s.base_stat END) AS attack, "
	"MAX(CASE WHEN s.stat_id = 3 THEN s.base_stat END) AS defense, "
	"MAX(CASE WHEN s.stat_id = 4 THEN s.base_stat END) AS special_attack, "
	"MAX(CASE WHEN s.stat_id = 5 THEN s.base_stat END) AS special_defense, "
	"MAX(CASE WHEN s.stat_id = 6 THEN s.base_stat END) AS speed, "
	"ps.evolution_chain_id, "
	"ps.evolves_from_species_id, "
	"GROUP_CONCAT(DISTINCT a.identifier) AS abilities "
	"FROM pokemon_species ps "
	"JOIN pokemon_types pta ON ps.id = pta.pokemon_id AND pta.slot = 1 "
	"JOIN types ta ON pta.type_id = ta.id "
	"LEFT JOIN pokemon_types ptb ON ps.id = ptb.pokemon_id AND ptb.slot = 2 "
	"LEFT JOIN types tb ON ptb.type_id = tb.id "
	"JOIN pokemon_species_flavor_text f ON ps.id = f.species_id "
	"JOIN versions v ON f.version_id = v.id "
	"JOIN pokemon_stats s ON ps.id = s.pokemon_id "
	"JOIN pokemon p ON ps.id = p.species_id  "
	"JOIN pokemon_abilities pa ON ps.id = pa.pokemon_id "
	"JOIN abilities a ON pa.ability_id = a.id "
	"JOIN pokemon_species_names psn ON ps.id = psn.pokemon_species_id "
	"WHERE ps.identifier = 'charmander' "
	"AND v.identifier = '${game_version}'  "
	"AND f.language_id = '${language_id}'  "
	"AND psn.local_language_id = '${language_id}' "
	"GROUP BY ps.id, v.id, ta.identifier, tb.identifier, ps.gender_rate, p.weight, p.height, ps.evolution_chain_id, ps.evolves_from_species_id "
	"ORDER BY ps.id, v.id; ";

