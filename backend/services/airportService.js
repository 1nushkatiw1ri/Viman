const pool = require("../db/pool");

async function getAllAirports() {
    const result = await pool.query(`
SELECT
iata,
name,
city,
country,
latitude,
longitude
FROM airports
WHERE iata IS NOT NULL
ORDER BY city
`);

    return result.rows;
}

module.exports = {
    getAllAirports
};