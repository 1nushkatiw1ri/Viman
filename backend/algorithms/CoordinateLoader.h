#ifndef COORDINATE_LOADER_H
#define COORDINATE_LOADER_H

#include <string>
#include <unordered_map>

struct Coordinate
{
    double latitude;
    double longitude;
};

class CoordinateLoader
{
public:
    static std::unordered_map<std::string, Coordinate>
    loadCoordinates(const std::string& filename);
};

#endif