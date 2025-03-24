SELECT
	pe.evolution_trigger_id,
	COALESCE(pe.evolved_species_id, ps.id) AS pokemon_id,
	ps.identifier AS pokemon_identifier,
	psn.name AS localized_name,
	et.identifier AS evolution_trigger_identifier,
	i.identifier AS item_identifier,
	pe.minimum_level,
	pe.gender_id,
	ln.name AS location_name,
	held_item.identifier AS held_item_identifier,
	pe.time_of_day,
	mn.name AS known_move_name,
	t.identifier AS move_type_identifier,
	pe.minimum_happiness,
	pe.minimum_beauty,
	pe.minimum_affection,
	pe.relative_physical_stats,
	pe.party_species_id,
	pe.party_type_id,
	pe.trade_species_id,
	pe.needs_overworld_rain,
	pe.turn_upside_down,
	MIN(pgi.version_id) AS min_version_id,
	MIN(vg.id) AS version_group_id,
	ps.generation_id AS pokemon_generation_id
FROM
	pokemon_species ps
LEFT JOIN
	pokemon_evolution pe ON ps.id = pe.evolved_species_id
LEFT JOIN
	pokemon_species_names psn ON ps.id = psn.pokemon_species_id AND psn.local_language_id = :language_id
LEFT JOIN
	evolution_triggers et ON pe.evolution_trigger_id = et.id
LEFT JOIN
	items i ON pe.trigger_item_id = i.id
LEFT JOIN
	items held_item ON pe.held_item_id = held_item.id
LEFT JOIN
	types t ON pe.known_move_type_id = t.id
LEFT JOIN
	moves m ON pe.known_move_id = m.id
LEFT JOIN
	move_names mn ON m.id = mn.move_id AND mn.local_language_id = :language_id
LEFT JOIN
	location_names ln ON pe.location_id = ln.location_id AND ln.local_language_id = 9
-- New joins for version-related columns
LEFT JOIN
	pokemon p ON ps.id = p.species_id
LEFT JOIN
	pokemon_game_indices pgi ON p.id = pgi.pokemon_id
LEFT JOIN
	versions v ON pgi.version_id = v.id
LEFT JOIN
	version_groups vg ON v.version_group_id = vg.id
WHERE
	ps.evolution_chain_id = :evo_chain_id
	AND ps.id <= 649
GROUP BY
	pe.evolution_trigger_id,
	ps.id,
	ps.identifier,
	psn.name,
	et.identifier,
	i.identifier,
	pe.minimum_level,
	pe.gender_id,
	ln.name,
	held_item.identifier,
	pe.time_of_day,
	mn.name,
	t.identifier,
	pe.minimum_happiness,
	pe.minimum_beauty,
	pe.minimum_affection,
	pe.relative_physical_stats,
	pe.party_species_id,
	pe.party_type_id,
	pe.trade_species_id,
	pe.needs_overworld_rain,
	pe.turn_upside_down,
	ps.generation_id
ORDER BY
	pokemon_id ASC;
