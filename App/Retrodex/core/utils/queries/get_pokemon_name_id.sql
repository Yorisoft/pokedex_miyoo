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
