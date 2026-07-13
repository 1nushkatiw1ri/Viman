const executeAlgorithm = require("../utils/executeAlgorithm");
const pool = require("../db/pool");

async function findRoute(source, destination) {

    const result = await executeAlgorithm(source, destination);

    const coordinates = [];

    for (const airport of result.path) {

        const query = await pool.query(
            `SELECT latitude, longitude
             FROM airports
             WHERE iata = $1`,
            [airport]
        );

        if (query.rows.length) {

            coordinates.push([
                Number(query.rows[0].latitude),
                Number(query.rows[0].longitude)
            ]);

        }

    }

    return {
        path: result.path,
        coordinates
    };

}

module.exports = {
    findRoute
};