import { MapContainer, TileLayer, Marker, Polyline } from "react-leaflet";
import { useMemo } from "react";

import "leaflet/dist/leaflet.css";
import "leaflet-defaulticon-compatibility";
import "leaflet-defaulticon-compatibility/dist/leaflet-defaulticon-compatibility.css";

function MapView({ airports, route, source, destination }) {  const sourceAirport = useMemo(
    () => airports.find((a) => a.iata === source),
    [airports, source]
  );

  const destinationAirport = useMemo(
    () => airports.find((a) => a.iata === destination),
    [airports, destination]
  );

  return (
    <div className="map-card">
      <MapContainer
        center={[20.5937, 78.9629]}
        zoom={5}
        style={{ height: "100%", width: "100%" }}
      >
        <TileLayer
          attribution="&copy; OpenStreetMap contributors"
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />

        {sourceAirport && (
          <Marker
            position={[
              Number(sourceAirport.latitude),
              Number(sourceAirport.longitude),
            ]}
          />
        )}

        {destinationAirport && (
          <Marker
            position={[
              Number(destinationAirport.latitude),
              Number(destinationAirport.longitude),
            ]}
          />
        )}

       {route && route.coordinates ? (
  <Polyline positions={route.coordinates} />
) : (
  sourceAirport &&
  destinationAirport && (
    <Polyline
      positions={[
        [
          Number(sourceAirport.latitude),
          Number(sourceAirport.longitude),
        ],
        [
          Number(destinationAirport.latitude),
          Number(destinationAirport.longitude),
        ],
      ]}
    />
  )
)}
      </MapContainer>
    </div>
  );
}

export default MapView;