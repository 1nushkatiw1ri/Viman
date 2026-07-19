function ResultCard({ route }) {

  if (!route) return null;

  return (

    <div className="result-card">

      <h2>Optimized Route</h2>

      <p>
        <strong>Route:</strong>{" "}
        {route.path.join(" → ")}
      </p>

      {route.distance !== undefined && (
        <p>
          <strong>Distance:</strong> {route.distance} km
        </p>
      )}

      {route.time !== undefined && (
        <p>
          <strong>Time:</strong> {route.time} min
        </p>
      )}

      {route.cost !== undefined && (
        <p>
          <strong>Cost:</strong> ₹{route.cost}
        </p>
      )}

    </div>

  );

}

export default ResultCard;