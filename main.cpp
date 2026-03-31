#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int color[MAXN];
bool in_odd_cycle[MAXN];
bool visited[MAXN];
int n, m;

// Check if component starting from 'start' is bipartite
// Mark all nodes in the component
bool isBipartite(int start, vector<int>& component) {
    queue<int> q;
    q.push(start);
    color[start] = 0;
    visited[start] = true;
    component.push_back(start);
    bool is_bipartite = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                color[v] = 1 - color[u];
                component.push_back(v);
                q.push(v);
            } else if (color[v] == color[u]) {
                is_bipartite = false;
            }
        }
    }

    return is_bipartite;
}

// For a non-bipartite component, mark which nodes are in odd cycles
void markOddCycleNodes(const vector<int>& component) {
    // For each node in the component, check if it's in an odd cycle
    for (int node : component) {
        // A node with degree <= 1 cannot be in a cycle
        if (graph[node].size() <= 1) {
            continue;
        }

        // Check if there's an odd cycle containing this node
        // by checking if any two neighbors have the same color
        bool found = false;
        for (int i = 0; i < (int)graph[node].size() && !found; i++) {
            for (int j = i + 1; j < (int)graph[node].size() && !found; j++) {
                int u = graph[node][i];
                int v = graph[node][j];
                // If u and v have the same color, then node-u-...-v-node forms odd cycle
                if (color[u] == color[v]) {
                    found = true;
                }
            }
        }

        if (found) {
            in_odd_cycle[node] = true;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    memset(visited, false, sizeof(visited));
    memset(in_odd_cycle, false, sizeof(in_odd_cycle));

    // Process each connected component
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            vector<int> component;
            bool is_bip = isBipartite(i, component);

            if (!is_bip) {
                // Component has odd cycles, mark which nodes are in them
                markOddCycleNodes(component);
            }
            // If bipartite, no nodes are in odd cycles (already false by default)
        }
    }

    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!in_odd_cycle[i]) {
            answer++;
        }
    }

    cout << answer << endl;

    return 0;
}
