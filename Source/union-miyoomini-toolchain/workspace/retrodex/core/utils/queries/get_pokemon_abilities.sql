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
