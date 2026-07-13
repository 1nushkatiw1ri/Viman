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

    std::vector<std::string> path =
        algorithm.dijkstra(source, destination);

    if (path.empty())
    {
        std::cout << "{ \"error\": \"Route not found\" }";
        return 0;
    }

    std::cout << "{";
    std::cout << "\"path\":[";

    for (size_t i = 0; i < path.size(); i++)
    {
        std::cout << "\"" << path[i] << "\"";

        if (i + 1 != path.size())
            std::cout << ",";
    }

    std::cout << "]";
    std::cout << "}";

    return 0;
}