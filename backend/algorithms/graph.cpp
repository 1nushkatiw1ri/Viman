#include <iostream>
#include <vector>
#include <string>

#include "AlgorithmUtils.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "{ \"error\": \"Usage: graph.exe <SOURCE> <DESTINATION>\" }";
        return 1;
    }

    std::string source = argv[1];
    std::string destination = argv[2];

    AlgorithmUtils algorithm(
        "graph.txt",
        "coordinates.txt"
    );

    RouteResult result =
        algorithm.dijkstra(
            source,
            destination,
            OptimizationMode::DISTANCE
        );

    if (result.path.empty())
    {
        std::cout << "{ \"error\": \"Route not found\" }";
        return 0;
    }

    std::cout << "{";

    std::cout << "\"path\":[";

    for (size_t i = 0; i < result.path.size(); i++)
    {
        std::cout << "\"" << result.path[i] << "\"";

        if (i + 1 != result.path.size())
            std::cout << ",";
    }

    std::cout << "],";

    std::cout << "\"distance\":"
              << result.distance
              << ",";

    std::cout << "\"time\":"
              << result.time
              << ",";

    std::cout << "\"cost\":"
              << result.cost;

    std::cout << "}";

    return 0;
}