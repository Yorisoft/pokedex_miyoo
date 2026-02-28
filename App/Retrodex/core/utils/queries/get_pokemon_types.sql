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
