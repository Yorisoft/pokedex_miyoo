SELECT 
  psft.flavor_text
FROM 
  pokemon_species_flavor_text psft
JOIN 
  versions v ON psft.version_id = v.id
WHERE 
  psft.language_id = :language_id
  AND psft.species_id = :pokemon_id;
