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
