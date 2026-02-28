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
