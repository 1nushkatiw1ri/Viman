const fs = require("fs");
const path = require("path");
const { parse } = require("csv-parse");
const pool = require("../config/db");

const filePath = path.join(
    __dirname,
    "../../../database/raw/routes.dat"
);

async function importRoutes() {
    try {

        console.log("📂 Looking for file:");
        console.log(filePath);

        if (!fs.existsSync(filePath)) {
            throw new Error("routes.dat not found!");
        }

        console.log("✅ routes.dat found");
        console.log("📖 Reading routes.dat...");

        const parser = fs.createReadStream(filePath).pipe(
            parse({
                delimiter: ",",
                relax_quotes: true,
                trim: true
            })
        );

        let count = 0;

        for await (const row of parser) {

            await pool.query(
                `
                INSERT INTO routes (
                    airline_code,
                    airline_id,
                    source_airport,
                    source_airport_id,
                    destination_airport,
                    destination_airport_id,
                    codeshare,
                    stops,
                    equipment
                )
                VALUES($1,$2,$3,$4,$5,$6,$7,$8,$9);
                `,
                [
                    row[0] === "\\N" ? null : row[0],
                    row[1] === "\\N" ? null : Number(row[1]),
                    row[2] === "\\N" ? null : row[2],
                    row[3] === "\\N" ? null : Number(row[3]),
                    row[4] === "\\N" ? null : row[4],
                    row[5] === "\\N" ? null : Number(row[5]),
                    row[6] === "\\N" ? null : row[6],
                    Number(row[7]),
                    row[8] === "\\N" ? null : row[8]
                ]
            );

            count++;

            if (count % 10000 === 0) {
                console.log(`✅ Imported ${count} routes`);
            }

        }

        console.log(`🎉 Successfully imported ${count} routes.`);

    } catch (err) {

        console.error("❌ Import Failed!");
        console.error(err);

    } finally {

        await pool.end();

    }
}

importRoutes();