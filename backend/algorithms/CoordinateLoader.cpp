#include "CoordinateLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::unordered_map<std::string, Coordinate>
CoordinateLoader::loadCoordinates(const std::string& filename)
{
    std::unordered_map<std::string, Coordinate> coordinates;

    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Unable to open coordinate file: "
                  << filename << std::endl;
        return coordinates;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);

        std::string airport;
        std::string lat;
        std::string lon;

        std::getline(ss, airport, ',');
        std::getline(ss, lat, ',');
        std::getline(ss, lon, ',');

        Coordinate coordinate;

        coordinate.latitude = std::stod(lat);
        coordinate.longitude = std::stod(lon);

        coordinates[airport] = coordinate;
    }

    file.close();

    return coordinates;
}