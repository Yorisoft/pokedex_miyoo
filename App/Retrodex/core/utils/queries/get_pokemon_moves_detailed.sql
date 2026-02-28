SELECT DISTINCT
	m.id AS Move_ID,
	mn.name AS Move_Name_Localized,
	t.identifier AS Move_Type,
	m.pp AS Move_PP,
	dc.identifier AS Class,
	m.power AS Power,
	m.accuracy AS Accuracy,
	MAX(mft.flavor_text) AS Flavor_Text,
	pm.level AS Learn_Level,
	pmm.identifier AS Learn_Method
FROM 
	pokemon_moves AS pm
INNER JOIN 
	moves AS m ON pm.move_id = m.id
INNER JOIN 
	move_names AS mn ON m.id = mn.move_id
INNER JOIN 
	types AS t ON m.type_id = t.id
INNER JOIN 
	move_damage_classes AS dc ON m.damage_class_id = dc.id
LEFT JOIN 
	move_flavor_text AS mft ON m.id = mft.move_id AND mft.language_id = :language_id
INNER JOIN 
	pokemon AS p ON pm.pokemon_id = p.id
INNER JOIN 
	version_groups AS vg ON vg.id = pm.version_group_id
INNER JOIN 
	pokemon_move_methods AS pmm ON pm.pokemon_move_method_id = pmm.id
WHERE 
	mn.local_language_id = :language_id
	AND vg.id = :version_group_id
	AND p.id = :pokemon_id
GROUP BY 
	m.id, mn.name, t.identifier, m.pp, pm.level, pmm.identifier, dc.identifier, m.power, m.accuracy
ORDER BY 
	Learn_Method, Learn_Level;
