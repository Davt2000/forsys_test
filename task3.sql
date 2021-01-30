CREATE TABLE table1(
                       id INTEGER PRIMARY KEY AUTOINCREMENT,
                       value TEXT
);
CREATE TABLE table2(
                       id INTEGER PRIMARY KEY AUTOINCREMENT,
                       value TEXT
);
INSERT INTO table1 (value) VALUES ("Ivanov");
INSERT INTO table1 (value) VALUES ("Petrov");
INSERT INTO table1 (value) VALUES ("Sidorov");
INSERT INTO table1 (value) VALUES ("Kotov");


INSERT INTO table2 (value) VALUES ("Ivanov");
INSERT INTO table2 (value) VALUES ("Kozlov");
INSERT INTO table2 (value) VALUES ("Vorobyov");
INSERT INTO table2 (value) VALUES ("Sidorov");

--  Написать SQL скрипт создания таблицы table3(id, value)
CREATE TABLE table3(
                       id INTEGER PRIMARY KEY AUTOINCREMENT,
                       value TEXT
);

-- Написать SQL скрипт наполняющий созданную таблицу данными по следующим правилам:
-- все записи из table1, которых нет в table2 поместить в table3
INSERT INTO table3 (value)
SELECT t1.value FROM
    table1 t1 LEFT OUTER JOIN table2 t2
                              ON t1.value = t2.value
WHERE t1.value IS NULL OR t2.value IS NULL
;

-- все записи из table2, которых нет в table1 поместить в table3
INSERT INTO table3 (value)
SELECT t2.value FROM
    table2 t2 LEFT OUTER JOIN table1 t1
                              ON t1.value = t2.value
WHERE t1.value IS NULL OR t2.value IS NULL
;