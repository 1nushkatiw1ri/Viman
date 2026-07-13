#ifndef ALGORITHM_UTILS_H
#define ALGORITHM_UTILS_H

#include <vector>
#include <string>
#include <unordered_map>

#include "CoordinateLoader.h"

struct Edge
{
    std::string destination;

    int distance;

    int time;

    int cost;
};

struct RouteResult
{
    std::vector<std::string> path;

    int distance = 0;

    int time = 0;

    int cost = 0;
};

enum class OptimizationMode
{
    DISTANCE,
    TIME,
    COST
};

class AlgorithmUtils
{

private:

    std::unordered_map<
        std::string,
        std::vector<Edge>
    > graph;

    std::unordered_map<
        std::string,
        Coordinate
    > coordinates;

public:

    AlgorithmUtils(
        const std::string& graphFile,
        const std::string& coordinateFile
    );

    void loadGraph(
        const std::string& filename
    );

    double heuristic(
        const std::string& source,
        const std::string& destination
    );

    RouteResult dijkstra(
        const std::string& source,
        const std::string& destination,
        OptimizationMode mode
    );

    RouteResult aStar(
        const std::string& source,
        const std::string& destination,
        OptimizationMode mode
    );

};

#endif