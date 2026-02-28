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
