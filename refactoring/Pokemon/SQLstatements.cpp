#include "SQLstatements.h"
#include<string>


// God bless ChatGPT for providing this.. this.. work of art.
const std::string SQL_getPokemonByName = R"(
		SELECT
            pdn.pokedex_number AS regional_pokedex_id,
            p.species_id AS national_pokedex_id,
            p.identifier AS pokemon_identifier,
            psn.name AS pokemon_name_in_language,
            t1.identifier AS type_1,
            t2.identifier AS type_2
        FROM
            pokemon AS p
        JOIN
            pokemon_species_names AS psn ON p.species_id = psn.pokemon_species_id
        JOIN
            pokemon_game_indices AS pgi ON p.id = pgi.pokemon_id
        JOIN
            versions AS v ON pgi.version_id = v.id
        JOIN
            pokemon_dex_numbers AS pdn ON p.species_id = pdn.species_id
        JOIN
            pokedexes AS px ON pdn.pokedex_id = px.id
        JOIN
            regions AS r ON px.region_id = r.id
        JOIN
            pokemon_types AS pt1 ON p.id = pt1.pokemon_id AND pt1.slot = 1
        LEFT JOIN
            pokemon_types AS pt2 ON p.id = pt2.pokemon_id AND pt2.slot = 2
        JOIN
            types AS t1 ON pt1.type_id = t1.id
        LEFT JOIN
            types AS t2 ON pt2.type_id = t2.id
        WHERE
            psn.local_language_id = '${language_id}'
            AND r.id = '${region_id}'
            AND v.identifier = '${game_version}'
            AND px.identifier NOT LIKE '%updated%'
        GROUP BY
            pdn.pokedex_number, p.species_id 
        ORDER BY
            regional_pokedex_id;
)";

const std::string SQL_getNameAndID = R"(
    SELECT 
        p.species_id AS national_pokedex_id,
        p.identifier AS pokemon_identifier,
        psn.name AS pokemon_name_in_language,
        t1.identifier AS type_1,
        t2.identifier AS type_2
    FROM
        pokemon AS p
    JOIN
        pokemon_species AS ps ON p.species_id = ps.id
    JOIN
        pokemon_species_names AS psn ON ps.id = psn.pokemon_species_id
    JOIN
        pokemon_types AS pt1 ON p.id = pt1.pokemon_id AND pt1.slot = 1
    LEFT JOIN
        pokemon_types AS pt2 ON p.id = pt2.pokemon_id AND pt2.slot = 2
    JOIN
        types AS t1 ON pt1.type_id = t1.id
    LEFT JOIN
        types AS t2 ON pt2.type_id = t2.id
    WHERE
        psn.local_language_id = '${language_id}'
        AND ps.generation_id = '${region_id}'
    GROUP BY
        p.species_id
    ORDER BY
        p.species_id;
)";


// // // // //  
const std::string SQL_getGameVersions = R"(
	SELECT
        v.id AS version_id,
        v.identifier AS version_identifier,
        vn.name AS version_name_in_language,
        r.id AS region_id,
        r.identifier AS region_identifier,
        g.id AS generation_id,
        g.identifier AS generation_identifier,
        v.version_group_id AS version_group_id  -- Added version_group_id
    FROM
        versions AS v
    JOIN
        version_names AS vn ON v.id = vn.version_id
    JOIN
        pokedex_version_groups AS pvg ON v.version_group_id = pvg.version_group_id
    JOIN
        pokedexes AS p ON pvg.pokedex_id = p.id
    JOIN
        regions AS r ON p.region_id = r.id
    JOIN
        generations AS g ON r.id = g.main_region_id
    WHERE
        vn.local_language_id = '${language_id}'
        AND v.id <= 22
        AND v.id NOT IN (10, 11, 15, 16, 21, 22)
    ORDER BY
        v.id;
)";

const std::string SQL_getPokeRegionalID = R"(
    SELECT
      ps.id AS pokemon_id,
      pdn.pokedex_number AS regional_pokedex_id
    FROM
      pokemon_species ps
    JOIN
      pokemon_species_names psn ON ps.id = psn.pokemon_species_id
    JOIN
      pokemon_dex_numbers pdn ON ps.id = pdn.species_id
    JOIN
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN
      regions r ON px.region_id = r.id
    JOIN
      pokemon_game_indices pgi ON ps.id = pgi.pokemon_id
    JOIN
      versions v ON pgi.version_id = v.id
    WHERE
      psn.local_language_id = '${language_id}'
      AND r.id = '${region_id}'
      AND v.identifier = '${game_version}'
      AND ps.identifier = '${pokemon_identifier}'
      AND px.identifier NOT LIKE '%updated%';
)";

const std::string SQL_getPokeEvoID = R"(
    SELECT evolution_chain_id FROM pokemon_species
    WHERE identifier = '${pokemon_identifier}'
)";

const std::string SQL_getPokeName = R"(
    SELECT
      ps.id AS pokemon_id,
      psn.name AS pokemon_name
    FROM
      pokemon_species ps
    JOIN
      pokemon_species_names psn ON ps.id = psn.pokemon_species_id
    JOIN
      pokemon_dex_numbers pdn ON ps.id = pdn.species_id
    JOIN
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN
      regions r ON px.region_id = r.id
    JOIN
      pokemon_game_indices pgi ON ps.id = pgi.pokemon_id
    JOIN
      versions v ON pgi.version_id = v.id
    WHERE
      psn.local_language_id = '${language_id}'
      AND r.id = '${region_id}'
      AND v.identifier = '${game_version}'
      AND ps.identifier = '${pokemon_identifier}'
      AND px.identifier NOT LIKE '%updated%';
)";

const std::string SQL_getPokeTypes = R"(
    SELECT 
      t1.identifier AS type_1, 
      t2.identifier AS type_2
    FROM 
      pokemon p
    JOIN 
      pokemon_game_indices pgi ON p.id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    JOIN 
      pokemon_dex_numbers pdn ON p.species_id = pdn.species_id
    JOIN 
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN 
      regions r ON px.region_id = r.id
    LEFT JOIN 
      pokemon_types pt1 ON p.id = pt1.pokemon_id AND pt1.slot = 1
    LEFT JOIN 
      types t1 ON pt1.type_id = t1.id
    LEFT JOIN 
      pokemon_types pt2 ON p.id = pt2.pokemon_id AND pt2.slot = 2
    LEFT JOIN 
      types t2 ON pt2.type_id = t2.id
    WHERE 
      v.identifier = '${game_version}'
      AND r.id = '${region_id}'
      AND p.species_id = (SELECT id FROM pokemon_species WHERE identifier = '${pokemon_identifier}')
      AND px.identifier NOT LIKE '%updated%';
)";


const std::string SQL_getPokeGenus = R"(
    SELECT 
      psn.name AS species_name,
      psn.genus AS species_genus
    FROM 
      pokemon_species ps
    JOIN 
      pokemon_species_names psn ON ps.id = psn.pokemon_species_id
    JOIN 
      pokemon_game_indices pgi ON ps.id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    JOIN 
      pokemon_dex_numbers pdn ON ps.id = pdn.species_id
    JOIN 
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN 
      regions r ON px.region_id = r.id
    WHERE 
      psn.local_language_id = '${language_id}'
      AND v.identifier = '${game_version}' 
      AND r.id = '${region_id}'
      AND ps.identifier = '${pokemon_identifier}'
      AND px.identifier NOT LIKE '%updated%';
)";


const std::string SQL_getPokeHW = R"(
    SELECT 
      p.height AS pokemon_height,
      p.weight AS pokemon_weight
    FROM 
      pokemon p
    JOIN 
      pokemon_game_indices pgi ON p.id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    JOIN 
      pokemon_dex_numbers pdn ON p.species_id = pdn.species_id
    JOIN 
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN 
      regions r ON px.region_id = r.id
    WHERE 
      v.identifier = '${game_version}' 
      AND r.id = '${region_id}'
      AND p.species_id = (SELECT id FROM pokemon_species WHERE identifier = '${pokemon_identifier}')
      AND px.identifier NOT LIKE '%updated%';
)";

const std::string SQL_getPokeFlavorText = R"(
    SELECT 
      psft.flavor_text
    FROM 
      pokemon_species_flavor_text psft
    JOIN 
      pokemon_game_indices pgi ON psft.species_id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    JOIN 
      pokemon_dex_numbers pdn ON psft.species_id = pdn.species_id
    JOIN 
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN 
      regions r ON px.region_id = r.id
    WHERE 
      psft.language_id = '${language_id}'
      AND v.identifier = '${game_version}'
      AND r.id = '${region_id}'
      AND psft.species_id = (SELECT id FROM pokemon_species WHERE identifier = '${pokemon_identifier}')
      AND px.identifier NOT LIKE '%updated%';
)";

const std::string SQL_getPokeAbilities = R"(
    SELECT 
      psft.flavor_text
    FROM 
      pokemon_species_flavor_text psft
    JOIN 
      pokemon_game_indices pgi ON psft.species_id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    JOIN 
      pokemon_dex_numbers pdn ON psft.species_id = pdn.species_id
    JOIN 
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN 
      regions r ON px.region_id = r.id
    WHERE 
      psft.language_id = '${language_id}'
      AND v.identifier = '${game_version}'
      AND r.id = '${region_id}'
      AND psft.species_id = (SELECT id FROM pokemon_species WHERE identifier = '${pokemon_identifier}')
      AND px.identifier NOT LIKE '%updated%';
)";

const std::string SQL_getPokeStats = R"(
    SELECT DISTINCT
      ps.base_stat, 
      s.identifier AS stat_name
    FROM 
      pokemon_stats ps
    JOIN 
      stats s ON ps.stat_id = s.id
    JOIN 
      pokemon p ON ps.pokemon_id = p.id
    JOIN 
      pokemon_game_indices pgi ON p.id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    JOIN 
      pokemon_dex_numbers pdn ON p.species_id = pdn.species_id
    JOIN 
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN 
      regions r ON px.region_id = r.id
    WHERE 
      p.identifier = '${pokemon_identifier}'
      AND v.identifier = '${game_version}'
      AND r.id = '${region_id}';
)";

const std::string SQL_getPokeGenderRates = R"(
    SELECT DISTINCT
      ps.gender_rate
    FROM 
      pokemon_species ps
    JOIN 
      pokemon p ON ps.id = p.species_id
    JOIN 
      pokemon_game_indices pgi ON p.id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    JOIN 
      pokemon_dex_numbers pdn ON ps.id = pdn.species_id
    JOIN 
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN 
      regions r ON px.region_id = r.id
    WHERE 
      p.identifier = '${pokemon_identifier}'
      AND v.identifier = '${game_version}'
      AND r.id = '${region_id}';
)";

const std::string SQL_getPokeMoves = R"(
    SELECT DISTINCT
        m.id AS Move_ID,
        mn.name AS Move_Name_Localized,
        t.identifier AS Move_Type,
        m.pp AS Move_PP,
        pm.level AS Learn_Level,
        pmm.identifier AS Learn_Method
    FROM 
        pokemon AS p
    INNER JOIN 
        pokemon_moves AS pm 
        ON p.id = pm.pokemon_id
    INNER JOIN 
        moves AS m 
        ON pm.move_id = m.id
    INNER JOIN 
        move_names AS mn
        ON m.id = mn.move_id
    INNER JOIN 
        types AS t 
        ON m.type_id = t.id
    INNER JOIN 
        version_groups vg
        ON vg.id = pm.version_group_id
    INNER JOIN 
        pokemon_move_methods AS pmm
        ON pm.pokemon_move_method_id = pmm.id
    WHERE 
         mn.local_language_id = '${language_id}'
         AND vg.id = '${regionGroup_id}'
         AND p.identifier = '${pokemon_identifier}'
    ORDER BY 
        pmm.identifier, pm.level;
)";

const std::string SQL_getPokeMovesDetail = R"(
    SELECT *
    FROM (
        SELECT DISTINCT
            m.id AS Move_ID,
            mn.name AS Move_Name_Localized,
            t.identifier AS Move_Type,
            m.pp AS Move_PP,
            dc.identifier AS Class,
            m.power AS Power,
            m.accuracy AS Accuracy,
            MAX(mft.flavor_text) AS Flavor_Text,
            pm.level AS Learn_Level,
            pmm.identifier AS Learn_Method
        FROM 
            pokemon AS p
        INNER JOIN 
            pokemon_moves AS pm 
            ON p.id = pm.pokemon_id
        INNER JOIN 
            moves AS m 
            ON pm.move_id = m.id
        INNER JOIN 
            move_names AS mn
            ON m.id = mn.move_id
        INNER JOIN 
            types AS t 
            ON m.type_id = t.id
        INNER JOIN 
            move_damage_classes AS dc
            ON m.damage_class_id = dc.id
        LEFT JOIN 
            move_flavor_text mft
            ON m.id = mft.move_id
            AND mft.language_id = '${language_id}'
        INNER JOIN 
            version_groups vg
            ON vg.id = pm.version_group_id
        INNER JOIN 
            pokemon_move_methods AS pmm
            ON pm.pokemon_move_method_id = pmm.id
        WHERE 
            mn.local_language_id = '${language_id}'
            AND vg.id = '${regionGroup_id}'
            AND p.identifier = '${pokemon_identifier}'
        GROUP BY 
            m.id, mn.name, t.identifier, m.pp, pm.level, pmm.identifier, dc.identifier, m.power, m.accuracy
    ) AS subquery
    ORDER BY 
        Learn_Method, Learn_Level;
)";


const std::string SQL_getPokeRoutes = R"(
    SELECT 
        vg.identifier AS version_group,
        l.identifier AS route_or_location,
        em.identifier AS encounter_method,
        MIN(e.min_level) AS min_level,
        MAX(e.max_level) AS max_level,
        SUM(COALESCE(laer.rate, es.rarity, 0)) AS total_encounter_rate
    FROM 
        pokemon p
    JOIN 
        encounters e ON p.id = e.pokemon_id
    JOIN 
        location_areas la ON e.location_area_id = la.id
    JOIN 
        locations l ON la.location_id = l.id
    JOIN 
        location_game_indices lgi ON l.id = lgi.location_id
    JOIN 
        versions v ON e.version_id = v.id
    JOIN 
        version_groups vg ON v.version_group_id = vg.id
    LEFT JOIN 
        location_area_encounter_rates laer 
        ON laer.location_area_id = la.id 
        AND laer.version_id = v.id
    LEFT JOIN 
        encounter_slots es ON e.encounter_slot_id = es.id
    LEFT JOIN 
        encounter_methods em ON es.encounter_method_id = em.id
    WHERE 
        p.identifier = '${pokemon_identifier}'
        AND vg.id = '${regionGroup_id}'
    GROUP BY 
        vg.id, l.id, em.id, lgi.game_index
    ORDER BY 
        vg.id, lgi.game_index, l.identifier, em.identifier;
)";

const std::string SQL_getPokeEvoChain = R"(
    SELECT
        ps.evolution_chain_id,
        p.id AS pokemon_id,
        p.identifier AS pokemon_identifier,
        ps.generation_id,
        psn.name AS localized_name,
        COALESCE(et.identifier, 'Base') AS evolution_method,
        pe.minimum_level,
        item_locale.name AS trigger_item_name,
        pe.time_of_day,
        pe.minimum_happiness,
        pe.minimum_affection,
        pe.known_move_type_id
    FROM
        pokemon_species ps
    JOIN
        pokemon p ON ps.id = p.species_id
    LEFT JOIN
        pokemon_species_names psn ON ps.id = psn.pokemon_species_id
    LEFT JOIN
        pokemon_evolution pe ON ps.id = pe.evolved_species_id
    LEFT JOIN
        evolution_triggers et ON pe.evolution_trigger_id = et.id
    LEFT JOIN
        items i ON pe.trigger_item_id = i.id
    LEFT JOIN
        item_names item_locale ON i.id = item_locale.item_id AND item_locale.local_language_id = '${language_id}'
    WHERE
        ps.evolution_chain_id = '${evoChain_id}'
        AND psn.local_language_id = '${language_id}'
    GROUP BY
        ps.evolution_chain_id,
        p.id,
        p.identifier,
        ps.generation_id,
        psn.name,
        COALESCE(et.identifier, 'Base'), 
        pe.minimum_level,
        item_locale.name,
        pe.time_of_day,
        pe.minimum_happiness,
        pe.minimum_affection,
        pe.known_move_type_id;
)";
// // // // // // 

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

const std::string SQL_getPokemonRoutesByName = R"(
SELECT 
    p.id AS pokemon_id,
    p.identifier AS pokemon_name,
    v.identifier AS game_version,
    l.identifier AS route_or_location,
    em.identifier AS encounter_method,
    min(e.min_level) AS min_level,
    max(e.max_level) AS max_level,
    SUM(COALESCE(laer.rate, es.rarity, 0)) AS total_encounter_rate
FROM pokemon p
JOIN encounters e ON p.id = e.pokemon_id
JOIN location_areas la ON e.location_area_id = la.id
JOIN locations l ON la.location_id = l.id
JOIN versions v ON e.version_id = v.id
LEFT JOIN location_area_encounter_rates laer 
    ON laer.location_area_id = la.id 
    AND laer.version_id = v.id
LEFT JOIN encounter_slots es ON e.encounter_slot_id = es.id
LEFT JOIN encounter_methods em ON es.encounter_method_id = em.id
WHERE 
    p.identifier = '${pokemon_name}'
    AND v.identifier = '${game_version}'
GROUP BY 
    p.id, v.id, l.id, em.id
ORDER BY 
    p.id, v.id, l.identifier, em.identifier
LIMIT 1000;
)";

