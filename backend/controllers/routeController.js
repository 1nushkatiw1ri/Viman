const algorithmService = require("../services/algorithmService");

async function getRoute(req, res) {
    try {

        const { source, destination } = req.query;

        const route = await algorithmService.findRoute(
            source,
            destination
        );

        res.json(route);

    } catch (err) {

        res.status(500).json({
            error: err.toString()
        });

    }
}

module.exports = {
    getRoute
};