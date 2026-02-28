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
