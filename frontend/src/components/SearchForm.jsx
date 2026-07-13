function SearchForm({
  airports,
  source,
  destination,
  setSource,
  setDestination,
  findRoute
}) {
  return (
    <div className="search-card">

      <label>Source Airport</label>

      <select
        value={source}
        onChange={(e) => setSource(e.target.value)}
      >
        {airports.map((airport) => (
          <option
            key={airport.iata}
            value={airport.iata}
          >
            {airport.iata} - {airport.name}
          </option>
        ))}
      </select>

      <label>Destination Airport</label>

      <select
        value={destination}
        onChange={(e) => setDestination(e.target.value)}
      >
        {airports.map((airport) => (
          <option
            key={airport.iata}
            value={airport.iata}
          >
            {airport.iata} - {airport.name}
          </option>
        ))}
      </select>

      <button onClick={findRoute}>
        Find Cheapest Route
      </button>

    </div>
  );
}

export default SearchForm;