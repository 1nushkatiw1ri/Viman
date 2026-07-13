function ResultCard({ route }) {

  if (!route) return null;

  return (

    <div className="result-card">

      <h2>Optimized Route</h2>

      <p>

        <strong>Route:</strong>

        {route.path.join(" → ")}

      </p>

    </div>

  );

}

export default ResultCard;