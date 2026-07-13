#include "AlgorithmUtils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <climits>

AlgorithmUtils::AlgorithmUtils(
    const std::string& graphFile,
    const std::string& coordinateFile)
{
    loadGraph(graphFile);
    coordinates = CoordinateLoader::loadCoordinates(coordinateFile);
}

void AlgorithmUtils::loadGraph(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Unable to open graph file: "
                  << filename << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);

        std::string source;
        Edge edge;

        ss >> source
           >> edge.destination
           >> edge.distance
           >> edge.time
           >> edge.cost;

        graph[source].push_back(edge);

        Edge reverse;

        reverse.destination = source;
        reverse.distance = edge.distance;
        reverse.time = edge.time;
        reverse.cost = edge.cost;

        graph[edge.destination].push_back(reverse);
    }

    file.close();
}

double AlgorithmUtils::heuristic(
    const std::string& source,
    const std::string& destination)
{
    if (coordinates.find(source) == coordinates.end())
        return 0;

    if (coordinates.find(destination) == coordinates.end())
        return 0;

    double lat1 = coordinates[source].latitude;
    double lon1 = coordinates[source].longitude;

    double lat2 = coordinates[destination].latitude;
    double lon2 = coordinates[destination].longitude;

    double dx = lat1 - lat2;
    double dy = lon1 - lon2;

    return std::sqrt(dx * dx + dy * dy);
}

std::vector<std::string> AlgorithmUtils::dijkstra(
    const std::string& source,
    const std::string& destination)
{
    using Node = std::pair<int, std::string>;

    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
    > pq;

    std::unordered_map<std::string, int> distance;
    std::unordered_map<std::string, std::string> parent;

    for (auto& airport : graph)
        distance[airport.first] = INT_MAX;

    distance[source] = 0;

    pq.push({0, source});

    while (!pq.empty())
    {
        auto current = pq.top();
        pq.pop();

        int currentDistance = current.first;
        std::string airport = current.second;

        if (airport == destination)
            break;

        for (const Edge& edge : graph[airport])
        {
            int newDistance =
                currentDistance + edge.distance;

            if (newDistance < distance[edge.destination])
            {
                distance[edge.destination] = newDistance;
                parent[edge.destination] = airport;

                pq.push({
                    newDistance,
                    edge.destination
                });
            }
        }
    }

    std::vector<std::string> path;

    if (distance[destination] == INT_MAX)
        return path;

    std::string current = destination;

    while (current != source)
    {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(source);

    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<std::string> AlgorithmUtils::aStar(
    const std::string& source,
    const std::string& destination)
{
    using Node = std::pair<double, std::string>;

    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
    > open;

    std::unordered_map<std::string, double> gScore;
    std::unordered_map<std::string, double> fScore;
    std::unordered_map<std::string, std::string> parent;

    for (auto& airport : graph)
    {
        gScore[airport.first] = 1e18;
        fScore[airport.first] = 1e18;
    }

    gScore[source] = 0;
    fScore[source] = heuristic(source, destination);

    open.push({
        fScore[source],
        source
    });

    while (!open.empty())
    {
        std::string current = open.top().second;
        open.pop();

        if (current == destination)
            break;

        for (const Edge& edge : graph[current])
        {
            double tentative =
                gScore[current] + edge.distance;

            if (tentative < gScore[edge.destination])
            {
                parent[edge.destination] = current;

                gScore[edge.destination] = tentative;

                fScore[edge.destination] =
                    tentative +
                    heuristic(
                        edge.destination,
                        destination
                    );

                open.push({
                    fScore[edge.destination],
                    edge.destination
                });
            }
        }
    }

    std::vector<std::string> path;

    if (parent.find(destination) == parent.end())
        return path;

    std::string current = destination;

    while (current != source)
    {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(source);

    std::reverse(path.begin(), path.end());

    return path;
}