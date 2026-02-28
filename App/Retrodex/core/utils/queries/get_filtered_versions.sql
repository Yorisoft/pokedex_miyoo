SELECT
	v.id AS version_id,
	v.identifier AS version_identifier,
	vn.name AS version_name_in_language,
	r.id AS region_id,
	r.identifier AS region_identifier,
	g.id AS generation_id,
	g.identifier AS generation_identifier,
	v.version_group_id AS version_group_id
FROM
	versions AS v
JOIN
	version_names AS vn ON v.id = vn.version_id
JOIN
	pokedex_version_groups AS pvg ON v.version_group_id = pvg.version_group_id
JOIN
	pokedexes AS p ON pvg.pokedex_id = p.id
JOIN
	regions AS r ON p.region_id = r.id
JOIN
	generations AS g ON r.id = g.main_region_id
WHERE
	vn.local_language_id = :language_id
	AND v.id <= 22
	AND v.id NOT IN (10, 11, 15, 16)
ORDER BY
	v.id;