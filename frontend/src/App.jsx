import { useEffect, useState } from "react";
import axios from "axios";
import "./App.css";

import SearchForm from "./components/SearchForm";
import ResultCard from "./components/ResultCard";
import MapView from "./components/MapView";

function App() {

  const [airports, setAirports] = useState([]);
  const [source, setSource] = useState("DEL");
  const [destination, setDestination] = useState("BOM");
  const [route, setRoute] = useState(null);

  useEffect(() => {

    axios
      .get("http://localhost:5000/api/airports")
      .then((res) => {
        setAirports(res.data);
      })
      .catch(console.error);

  }, []);

  async function findRoute() {

    try {

      const res = await axios.get(
        `http://localhost:5000/api/route?source=${source}&destination=${destination}`
      );

      setRoute(res.data);

    } catch (err) {

      console.error(err);

    }

  }

  return (

    <div className="container">

      <h1>Viman</h1>

      <h3>Airline Route Optimizer</h3>

      <SearchForm
        airports={airports}
        source={source}
        destination={destination}
        setSource={setSource}
        setDestination={setDestination}
        findRoute={findRoute}
      />

      <ResultCard route={route} />

      <MapView
        airports={airports}
        route={route}
        source={source}
        destination={destination}
      />

    </div>

  );

}

export default App;