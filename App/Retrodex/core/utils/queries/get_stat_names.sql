SELECT
	s.id AS stat_id,
	s.identifier AS stat_identifier,
	sn.name AS stat_name
FROM
	stats s
LEFT JOIN
	stat_names sn ON s.id = sn.stat_id
WHERE
	sn.local_language_id = :language_id
ORDER BY
	s.id;
