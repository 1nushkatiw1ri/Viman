const pool = require("../config/db");

async function createTables() {
    try {
        await pool.query("DROP TABLE IF EXISTS routes;");
        await pool.query("DROP TABLE IF EXISTS airlines;");
        await pool.query("DROP TABLE IF EXISTS airports;");


        await pool.query(`
            CREATE TABLE IF NOT EXISTS airports (
                airport_id INTEGER PRIMARY KEY,
                name TEXT NOT NULL,
                city TEXT,
                country TEXT,
                iata CHAR(3),
                icao CHAR(4),
                latitude DOUBLE PRECISION,
                longitude DOUBLE PRECISION,
                altitude INTEGER,
                timezone DOUBLE PRECISION
            );
        `);

        await pool.query(`
        CREATE TABLE IF NOT EXISTS airlines (
        airline_id INTEGER PRIMARY KEY,
        name TEXT NOT NULL,
        alias TEXT,
        iata VARCHAR(10),
        icao VARCHAR(10),
        callsign TEXT,
        country TEXT,
        active CHAR(1)
    );
`);

        await pool.query(`
            CREATE TABLE IF NOT EXISTS routes (
                route_id SERIAL PRIMARY KEY,
                airline_code VARCHAR(5),
                airline_id INTEGER,
                source_airport CHAR(3),
                source_airport_id INTEGER,
                destination_airport CHAR(3),
                destination_airport_id INTEGER,
                codeshare CHAR(1),
                stops INTEGER,
                equipment TEXT
            );
        `);

        console.log("✅ Tables created successfully");

    } catch (err) {

        console.error(err);

    } finally {

        await pool.end();

    }
}

createTables();