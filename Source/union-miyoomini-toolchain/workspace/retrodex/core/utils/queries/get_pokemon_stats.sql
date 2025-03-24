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
