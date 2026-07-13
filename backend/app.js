const express = require("express");
const cors = require("cors");

const routeRoutes = require("./routes/routeRoutes");
const airportRoutes = require("./routes/airportRoutes");

const app = express();

app.use(cors());
app.use(express.json());

app.use("/api/routes", routeRoutes);
app.use("/api/airports", airportRoutes);

module.exports = app;