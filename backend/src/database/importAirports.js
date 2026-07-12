const fs = require("fs");
const path = require("path");
const { parse } = require("csv-parse");
const pool = require("../config/db");

const filePath = path.join(
    __dirname,
    "../../../database/raw/airports.dat"
);

async function importAirports() {
    try {
        console.log("📂 Looking for file:");
        console.log(filePath);

        if (!fs.existsSync(filePath)) {
            throw new Error("airports.dat not found!");
        }

        console.log("✅ airports.dat found");
        console.log("📖 Reading airports.dat...");

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
                INSERT INTO airports (
                    airport_id,
                    name,
                    city,
                    country,
                    iata,
                    icao,
                    latitude,
                    longitude,
                    altitude,
                    timezone
                )
                VALUES ($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)
                ON CONFLICT (airport_id) DO NOTHING;
                `,
                [
                    Number(row[0]),
                    row[1],
                    row[2],
                    row[3],
                    row[4] === "\\N" ? null : row[4],
                    row[5] === "\\N" ? null : row[5],
                    Number(row[6]),
                    Number(row[7]),
                    Number(row[8]),
                    Number(row[9])
                ]
            );

            count++;

            if (count % 1000 === 0) {
                console.log(`✅ Imported ${count} airports`);
            }
        }

        console.log(`\n🎉 Successfully imported ${count} airports.`);
    } catch (err) {
        console.error("❌ Import Failed!");
        console.error(err);
    } finally {
        await pool.end();
    }
}

importAirports();