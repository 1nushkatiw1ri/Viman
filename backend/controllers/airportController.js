const airportService = require("../services/airportService");

async function getAirports(req, res) {
    try {
        const airports = await airportService.getAllAirports();
        res.json(airports);
    } catch (err) {
        res.status(500).json({
            error: err.message
        });
    }
}

module.exports = {
    getAirports
};