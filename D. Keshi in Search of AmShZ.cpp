#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // We process the graph backwards from N to 1.
    // Graph 'adj' stores reversed edges: v -> u if original has u -> v
    vector<vector<int>> adj(n + 1);
    vector<int> out_degree(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        // Edge u -> v exists
        out_degree[u]++;
        adj[v].push_back(u); // Reverse for propagation
    }

    // dist[u] = minimal days to guarantee arrival at N
    vector<int> dist(n + 1, INF);
    // processed_count[u] = how many neighbors of u have been settled so far
    vector<int> processed_count(n + 1, 0);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[n] = 0;
    pq.push({0, n});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == 1) {
            cout << d << endl;
            return 0;
        }

        for (int v : adj[u]) {
            // Propagate distance to v
            // v has edge to u. u is a neighbor of v.
            // AmShZ keeps u as an option.
            processed_count[v]++;
            
            // Current best neighbor is u with dist[u].
            // To ensure worst case is u, we block all OTHER remaining (worse) neighbors.
            // Cost = dist[u] (worst allowed move) + 1 (day to move) + remaining_bad (days to block)
            int remaining_neighbors = out_degree[v] - processed_count[v];
            int candidate = dist[u] + 1 + remaining_neighbors;

            if (candidate < dist[v]) {
                dist[v] = candidate;
                pq.push({candidate, v});
            }
        }
    }

    return 0;
}
