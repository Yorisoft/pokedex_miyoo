SELECT
    l.id AS language_id,
    ln.name AS language_name,
    l.identifier AS language_identifier
FROM
    languages AS l
    JOIN language_names AS ln ON l.id = ln.language_id
    AND ln.language_id = ln.local_language_id
ORDER BY
    l.id;
