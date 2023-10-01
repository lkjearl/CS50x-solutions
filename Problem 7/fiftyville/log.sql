-- Keep a log of any SQL queries you execute as you solve the mystery.

-- View the report on the crime by the police
SELECT description FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = "Humphrey Street";

-- CHeck interview
SELECT name, transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";

-- check security log for theft car -> result in 8 suspect
SELECT license_plate, activity FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND (minute BETWEEN 15 AND 25);

-- check against license plate, phone number and bank account -> result in 2 suspect (usage of intersect)
SELECT name, people.passport_number FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE people.license_plate = "5P2BI95" OR people.license_plate = "94KL13X" OR people.license_plate = "6P58WS2" OR people.license_plate = "4328GD8" OR people.license_plate = "G412CB7" OR people.license_plate = "L93JTIZ" OR people.license_plate = "322W7JE" OR people.license_plate = "0NTHK55"
INTERSECT
SELECT name, people.passport_number FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT name, people.passport_number FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28 AND atm_transactions.atm_location LIKE "%Leggett Street%" AND transaction_type = "withdraw";

-- narrow suspect to 1 by using flight details (usage of nested)
SELECT passport_number, destination_airport_id FROM passengers
JOIN flights ON passengers.flight_id = flights.id
WHERE flight_id =
(
    SELECT id FROM flights
    WHERE flights.year = 2021 AND flights.month = 7 AND flights.day = 29 AND origin_airport_id =
    (
        SELECT id FROM airports
        WHERE city LIKE "%Fiftyville%"
    )
    ORDER BY hour, minute
    LIMIT 1
)
AND
passport_number IN
(
    SELECT passport_number FROM passengers
    WHERE passport_number = 5773159633 OR passport_number = 3592750733
);

-- find destination theft is flying
SELECT full_name, city FROM airports
WHERE id = 4;


-- find accomplice
SELECT name FROM people
WHERE phone_number IN
(
    SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller IN
    (
        SELECT phone_number FROM people WHERE passport_number = 5773159633
    )
);