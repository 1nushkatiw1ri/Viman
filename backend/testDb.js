require("dotenv").config();

const pool = require("./db/pool");

async function test() {
    try {
        const result = await pool.query(
            "SELECT COUNT(*) FROM airports"
        );
        console.log(result.rows);
    } catch (err) {
        console.error(err);
    } finally {
        await pool.end();
    }
}

test();