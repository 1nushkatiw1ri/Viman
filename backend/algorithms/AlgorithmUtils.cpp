#include "AlgorithmUtils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

static int getWeight(const Edge& edge, OptimizationMode mode)
{
    switch (mode)
    {
        case OptimizationMode::DISTANCE:
            return edge.distance;

        case OptimizationMode::TIME:
            return edge.time;

        case OptimizationMode::COST:
            return edge.cost;
    }

    return edge.distance;
}

AlgorithmUtils::AlgorithmUtils(
    const string& graphFile,
    const string& coordinateFile)
{
    loadGraph(graphFile);
    coordinates =
        CoordinateLoader::loadCoordinates(
            coordinateFile
        );
}

void AlgorithmUtils::loadGraph(
    const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open graph file: "
             << filename << endl;
        return;
    }

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string source;

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
    const string& source,
    const string& destination)
{
    if (coordinates.find(source) ==
        coordinates.end())
        return 0;

    if (coordinates.find(destination) ==
        coordinates.end())
        return 0;

    const Coordinate& a =
        coordinates[source];

    const Coordinate& b =
        coordinates[destination];

    double dx =
        a.latitude -
        b.latitude;

    double dy =
        a.longitude -
        b.longitude;

    return sqrt(dx * dx + dy * dy);
}
RouteResult AlgorithmUtils::dijkstra(
    const string& source,
    const string& destination,
    OptimizationMode mode)
{
    using Node = pair<int, string>;

    priority_queue<
        Node,
        vector<Node>,
        greater<Node>
    > pq;

    unordered_map<string, int> dist;
    unordered_map<string, string> parent;

    for (const auto& airport : graph)
    {
        dist[airport.first] = INT_MAX;
    }

    dist[source] = 0;

    pq.push({0, source});

    while (!pq.empty())
{
    Node top = pq.top();
    pq.pop();

    int currentWeight = top.first;
    std::string current = top.second;

        if (currentWeight > dist[current])
            continue;

        if (current == destination)
            break;

        for (const Edge& edge : graph[current])
        {
            int weight = getWeight(edge, mode);

            int newWeight =
                currentWeight + weight;

            if (newWeight < dist[edge.destination])
            {
                dist[edge.destination] =
                    newWeight;

                parent[edge.destination] =
                    current;

                pq.push({
                    newWeight,
                    edge.destination
                });
            }
        }
    }

    RouteResult result;

    if (dist[destination] == INT_MAX)
        return result;

    vector<string> reversePath;

    string current = destination;

    while (current != source)
    {
        reversePath.push_back(current);
        current = parent[current];
    }

    reversePath.push_back(source);

    reverse(
        reversePath.begin(),
        reversePath.end()
    );

    result.path = reversePath;

    for (size_t i = 0;
         i + 1 < result.path.size();
         i++)
    {
        string u = result.path[i];
        string v = result.path[i + 1];

        for (const Edge& edge : graph[u])
        {
            if (edge.destination == v)
            {
                result.distance += edge.distance;
                result.time += edge.time;
                result.cost += edge.cost;
                break;
            }
        }
    }

    return result;
}
RouteResult AlgorithmUtils::aStar(
    const string& source,
    const string& destination,
    OptimizationMode mode)
{
    using Node = pair<double, string>;

    priority_queue<
        Node,
        vector<Node>,
        greater<Node>
    > open;

    unordered_map<string, double> gScore;
    unordered_map<string, double> fScore;
    unordered_map<string, string> parent;

    for (const auto& airport : graph)
    {
        gScore[airport.first] = 1e18;
        fScore[airport.first] = 1e18;
    }

    gScore[source] = 0;

    fScore[source] =
        heuristic(source, destination);

    open.push({
        fScore[source],
        source
    });

    while (!open.empty())
    {
        string current =
            open.top().second;

        open.pop();

        if (current == destination)
            break;

        for (const Edge& edge : graph[current])
        {
            double tentative =
                gScore[current] +
                getWeight(edge, mode);

            if (tentative <
                gScore[edge.destination])
            {
                parent[edge.destination] =
                    current;

                gScore[edge.destination] =
                    tentative;

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

    RouteResult result;

    if (parent.find(destination) ==
        parent.end())
        return result;

    vector<string> reversePath;

    string current = destination;

    while (current != source)
    {
        reversePath.push_back(current);
        current = parent[current];
    }

    reversePath.push_back(source);

    reverse(
        reversePath.begin(),
        reversePath.end()
    );

    result.path = reversePath;

    for (size_t i = 0;
         i + 1 < result.path.size();
         i++)
    {
        string u = result.path[i];
        string v = result.path[i + 1];

        for (const Edge& edge : graph[u])
        {
            if (edge.destination == v)
            {
                result.distance += edge.distance;
                result.time += edge.time;
                result.cost += edge.cost;
                break;
            }
        }
    }

    return result;
}