const fs = require("fs");
const path = require("path");
const { parse } = require("csv-parse");
const pool = require("../config/db");

const filePath = path.join(
    __dirname,
    "../../../database/raw/airlines.dat"
);

async function importAirlines() {
    try {
        console.log("📂 Looking for file:");
        console.log(filePath);

        if (!fs.existsSync(filePath)) {
            throw new Error("airlines.dat not found!");
        }

        console.log("✅ airlines.dat found");
        console.log("📖 Reading airlines.dat...");

        const parser = fs.createReadStream(filePath).pipe(
            parse({
                delimiter: ",",
                relax_quotes: true,
                trim: true,
            })
        );

        let count = 0;

        for await (const row of parser) {

            await pool.query(
                `
                INSERT INTO airlines (
                    airline_id,
                    name,
                    alias,
                    iata,
                    icao,
                    callsign,
                    country,
                    active
                )
                VALUES ($1,$2,$3,$4,$5,$6,$7,$8)
                ON CONFLICT (airline_id) DO NOTHING;
                `,
                [
                    Number(row[0]),
                    row[1],
                    row[2] === "\\N" ? null : row[2],
                    row[3] === "\\N" ? null : row[3],
                    row[4] === "\\N" ? null : row[4],
                    row[5] === "\\N" ? null : row[5],
                    row[6] === "\\N" ? null : row[6],
                    row[7]
                ]
            );

            count++;

            if (count % 1000 === 0) {
                console.log(`✅ Imported ${count} airlines`);
            }
        }

        console.log(`🎉 Successfully imported ${count} airlines.`);
    } catch (err) {
        console.error("❌ Import Failed!");
        console.error(err);
    } finally {
        await pool.end();
    }
}

importAirlines();