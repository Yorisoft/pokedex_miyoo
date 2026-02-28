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
