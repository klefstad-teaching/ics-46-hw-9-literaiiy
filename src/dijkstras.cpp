#include "dijkstras.h"

#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

struct Node {
    int vertex;
    int distance;
    Node(int v, int d) : vertex(v), distance(d) {}
};

struct NodeCompare {
    bool operator()(const Node &a, const Node &b) {
        return a.distance > b.distance;
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    // Initialize distance vector with INF and visited vector with false.
    vector<int> distances(G.numVertices, INF);
    vector<bool> visited(G.numVertices, false);
    previous.assign(G.numVertices, -1);  // Set all previous vertices to -1 initially.

    // Priority queue for processing vertices in order of increasing distance.
    priority_queue<Node, vector<Node>, NodeCompare> pq;
    distances[source] = 0;
    pq.push(Node(source, 0));

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        int u = current.vertex;

        // Skip if this vertex has been visited already.
        if (visited[u]) continue;
        visited[u] = true;

        // For every edge from u to v, try to relax the edge.
        for (const Edge &edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            if (!visited[v] && distances[u] != INF && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                pq.push(Node(v, distances[v]));
            }
        }
    }

    return distances;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;
    if (distances[destination] == INF) {
        return path;
    }
    for (int at = destination; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& v, int total) {
    if (v.empty()) {
        cout << "No path exists." << endl;
        return;
    }
    cout << "Path: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i != v.size() - 1) {
            cout << " -> ";
        }
    }
    cout << "\nTotal cost: " << total << endl;
}
