#include "SQLstatements.h"

// God bless ChatGPT for providing this.. this.. work of art.
const std::string SQL_getGameVersions = R"(
	SELECT
        v.id AS version_id,
        v.identifier AS version_identifier,
        vn.name AS version_name_in_language,
        r.id AS region_id,
        r.identifier AS region_identifier,
        g.id AS generation_id,
        g.identifier AS generation_identifier,
        v.version_group_id AS version_group_id
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
        vn.local_language_id = :language_id
        AND v.id <= 22
        AND v.id NOT IN (10, 11, 15, 16)
    ORDER BY
        v.id;
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
    JOIN
        pokemon_game_indices AS pgi ON p.id = pgi.pokemon_id
    JOIN
        versions AS v ON pgi.version_id = v.id
    WHERE
        psn.local_language_id = :language_id
        AND ps.generation_id = :generation_id
    GROUP BY
        p.species_id
    ORDER BY
        p.species_id;
)";

const std::string SQL_getPokeRegionalID = R"(
    SELECT DISTINCT
      ps.id AS pokemon_id,
      pdn.pokedex_number AS regional_pokedex_id
    FROM
      pokemon_species ps
    JOIN
      pokemon_dex_numbers pdn ON ps.id = pdn.species_id
    JOIN
      pokedexes px ON pdn.pokedex_id = px.id
    JOIN
      regions r ON px.region_id = r.id
    JOIN
      generations g ON r.id = g.main_region_id 
    WHERE
      g.id = :generation_id 
      AND ps.id = :pokemon_id;
)";

const std::string SQL_getPokeName = R"(
    SELECT
      ps.id AS pokemon_id,
      psn.name AS pokemon_name,
      v.identifier AS version_identifier
    FROM
      pokemon_species ps
    JOIN
      pokemon_species_names psn ON ps.id = psn.pokemon_species_id
    JOIN
      pokemon_game_indices pgi ON ps.id = pgi.pokemon_id
    JOIN
      versions v ON pgi.version_id = v.id
    WHERE
      psn.local_language_id = :language_id
      AND v.id = :version_id
      AND ps.id = :pokemon_id;
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
      v.id = :version_id
      AND p.id = :pokemon_id
      AND px.identifier NOT LIKE '%updated%';
)";

const std::string SQL_getPokeGenus = R"(
    SELECT DISTINCT
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
    WHERE 
      psn.local_language_id = :language_id
      AND v.id = :version_id 
      AND ps.id = :pokemon_id;
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
    WHERE 
      v.id = :version_id 
      AND p.id = :pokemon_id;
)";

const std::string SQL_getPokeFlavorText = R"(
    SELECT 
      psft.flavor_text
    FROM 
      pokemon_species_flavor_text psft
    JOIN 
      versions v ON psft.version_id = v.id
    WHERE 
      psft.language_id = :language_id
      AND psft.species_id = :pokemon_id;
)";

const std::string SQL_getPokeAbilities = R"(
    SELECT 
        an.name AS ability_name,
        MAX(aft.flavor_text) AS flavor_text
    FROM 
        pokemon_abilities pa
    JOIN 
        abilities a ON pa.ability_id = a.id
    JOIN 
        ability_names an ON a.id = an.ability_id
    JOIN 
        ability_flavor_text aft ON a.id = aft.ability_id
    JOIN 
        versions v ON v.id = aft.version_group_id
    JOIN 
        version_groups vg ON v.version_group_id = vg.id
    WHERE 
        pa.pokemon_id = :pokemon_id
        AND an.local_language_id = :language_id
        AND aft.language_id = :language_id
        AND vg.generation_id = :generation_id
    GROUP BY
        an.name;
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
    WHERE 
      p.id = :pokemon_id
      AND v.id = :version_id;
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
      pokemon_game_indices pgi ON ps.pokemon_id = pgi.pokemon_id
    JOIN 
      versions v ON pgi.version_id = v.id
    WHERE 
      ps.pokemon_id = :pokemon_id
      AND pgi.version_id = :version_id;
)";

// Query hard coded to retrieve information in english, dont think support for other languages exist in the db. 
const std::string SQL_getPokeRoutes = R"(
    SELECT 
        v.identifier AS version,
        ln.name AS route_or_location,
        em.identifier AS encounter_method,
        MIN(e.min_level) AS min_level,
        MAX(e.max_level) AS max_level,
        SUM(COALESCE(laer.rate, es.rarity, 0)) AS total_encounter_rate,
        COUNT(DISTINCT e.encounter_slot_id) AS total_slot_number,
        la.identifier AS location_area_name
    FROM 
        encounters e
    JOIN 
        location_areas la ON e.location_area_id = la.id
    JOIN 
        locations l ON la.location_id = l.id
    JOIN 
        versions v ON e.version_id = v.id
    LEFT JOIN 
        location_area_encounter_rates laer ON laer.location_area_id = la.id AND laer.version_id = v.id
    LEFT JOIN 
        encounter_slots es ON e.encounter_slot_id = es.id
    LEFT JOIN 
        encounter_methods em ON es.encounter_method_id = em.id
    LEFT JOIN 
        location_names ln ON l.id = ln.location_id AND ln.local_language_id = 9 
    WHERE 
        e.pokemon_id = :pokemon_id
        AND v.id = :version_id
    GROUP BY 
        v.identifier, ln.name, em.identifier, la.identifier
    ORDER BY 
        l.id, v.identifier, ln.name, em.identifier;
)";

const std::string SQL_getPokeEvoID = R"(
    SELECT evolution_chain_id
    FROM pokemon_species
    WHERE id = :pokemon_id;
)";

const std::string SQL_getPokeEvoChain = R"(
    SELECT
        ps.evolution_chain_id,
        p.id AS pokemon_id,
        p.identifier AS pokemon_identifier,
        psn.name AS localized_name,
        COALESCE(et.identifier, 'Base') AS evolution_method,
        pe.minimum_level,
        item_locale.name AS trigger_item_name,
        pe.time_of_day,
        pe.minimum_happiness,
        pe.minimum_affection,
        pe.known_move_type_id,
        MIN(pgi.version_id) AS min_version_id,
        MIN(vg.id) AS version_group_id,
        ps.generation_id AS pokemon_generation_id
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
        item_names item_locale ON i.id = item_locale.item_id AND item_locale.local_language_id = :language_id
    LEFT JOIN
        pokemon_game_indices pgi ON p.id = pgi.pokemon_id
    LEFT JOIN
        versions v ON pgi.version_id = v.id
    LEFT JOIN
        version_groups vg ON v.version_group_id = vg.id
    WHERE
        ps.evolution_chain_id = :evo_chain_id
        AND psn.local_language_id = :language_id
        AND p.id <= 648
    GROUP BY
        ps.evolution_chain_id,
        p.id,
        p.identifier,
        psn.name,
        COALESCE(et.identifier, 'Base'), 
        pe.minimum_level,
        item_locale.name,
        pe.time_of_day,
        pe.minimum_happiness,
        pe.minimum_affection,
        pe.known_move_type_id;
)";

const std::string SQL_getPokeMovesDetail = R"(
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
        pokemon_moves AS pm
    INNER JOIN 
        moves AS m ON pm.move_id = m.id
    INNER JOIN 
        move_names AS mn ON m.id = mn.move_id
    INNER JOIN 
        types AS t ON m.type_id = t.id
    INNER JOIN 
        move_damage_classes AS dc ON m.damage_class_id = dc.id
    LEFT JOIN 
        move_flavor_text AS mft ON m.id = mft.move_id AND mft.language_id = :language_id
    INNER JOIN 
        pokemon AS p ON pm.pokemon_id = p.id
    INNER JOIN 
        version_groups AS vg ON vg.id = pm.version_group_id
    INNER JOIN 
        pokemon_move_methods AS pmm ON pm.pokemon_move_method_id = pmm.id
    WHERE 
        mn.local_language_id = :language_id
        AND vg.id = :version_group_id
        AND p.id = :pokemon_id
    GROUP BY 
        m.id, mn.name, t.identifier, m.pp, pm.level, pmm.identifier, dc.identifier, m.power, m.accuracy
    ORDER BY 
        Learn_Method, Learn_Level;
)";

const std::string SQL_getLanguageVersion = R"(
    SELECT
        l.id AS language_id,
        ln.name AS language_name,
        l.identifier AS language_identifier
    FROM
        languages AS l
    JOIN
        language_names AS ln ON l.id = ln.language_id AND ln.language_id = ln.local_language_id
    ORDER BY
        l.id;
)";

const std::string SQL_getStatNames = R"(
    SELECT
        s.id AS stat_id,
        s.identifier AS stat_identifier,
        sn.name AS stat_name
    FROM
        stats s
    LEFT JOIN
        stat_names sn ON s.id = sn.stat_id
    WHERE
        sn.local_language_id = :language_id
    ORDER BY
        s.id;
)";
