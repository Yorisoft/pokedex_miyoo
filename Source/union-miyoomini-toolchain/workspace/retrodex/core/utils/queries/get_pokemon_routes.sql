-- Query hard coded to retrieve information in english, dont think support for other languages exist in the db.
SELECT 
	v.identifier AS version_name,
	ln.name AS location_name,
	em.identifier AS encounter_method,
	MIN(e.min_level) AS min_level,
	MAX(e.max_level) AS max_level,
	SUM(es.rarity) AS total_encounter_rate,
	la.identifier AS location_area_name,
	ecv.identifier AS time_of_day
FROM 
	encounters e
JOIN 
	location_areas la ON e.location_area_id = la.id
JOIN 
	locations l ON la.location_id = l.id
JOIN 
	versions v ON e.version_id = v.id
LEFT JOIN 
	location_names ln ON l.id = ln.location_id AND ln.local_language_id = 9
LEFT JOIN 
	encounter_slots es ON e.encounter_slot_id = es.id
LEFT JOIN 
	encounter_methods em ON es.encounter_method_id = em.id
LEFT JOIN 
	encounter_condition_value_map ecm ON ecm.encounter_id = e.id
LEFT JOIN 
	encounter_condition_values ecv ON ecm.encounter_condition_value_id = ecv.id
WHERE 
	e.pokemon_id = :pokemon_id
	AND v.id = :version_id
GROUP BY 
	v.identifier, ln.name, em.identifier, ecv.identifier, la.identifier
ORDER BY 
	ln.name, em.identifier, ecv.identifier;
