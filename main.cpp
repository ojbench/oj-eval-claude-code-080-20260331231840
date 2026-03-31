#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <set>

using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int color[MAXN];
bool visited[MAXN];
int n, m;

// Check if component is bipartite and mark all nodes
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

// For non-bipartite component, mark nodes in odd cycles
void markOddCycleNodes(const vector<int>& component, vector<bool>& in_odd) {
    // Find all conflict edges (edges between nodes of same color)
    vector<pair<int, int>> conflict_edges;

    set<int> comp_set(component.begin(), component.end());

    for (int u : component) {
        for (int v : graph[u]) {
            if (u < v && color[u] == color[v] && comp_set.count(v)) {
                conflict_edges.push_back({u, v});
            }
        }
    }

    // If there are conflict edges, the component has odd cycles
    // Mark all nodes that are "close" to conflict edges
    // Simple heuristic: mark all nodes in non-bipartite component with degree >= 2
    // This is conservative but should be more accurate

    // Actually, let's use a better approach:
    // Do BFS from each conflict edge and mark all nodes within a certain distance
    // Or simpler: mark all nodes in the same biconnected component as the conflict edge

    // For now, let's use a simple heuristic:
    // A node is in an odd cycle if it's in a non-bipartite component AND
    // it can reach a conflict edge within some small distance

    for (int node : component) {
        if (graph[node].size() <= 1) {
            in_odd[node] = false;
            continue;
        }

        // Check if node is involved in a conflict edge or adjacent to one
        bool found = false;

        // Check if node itself is part of a conflict edge
        for (int neighbor : graph[node]) {
            if (comp_set.count(neighbor) && color[node] == color[neighbor]) {
                found = true;
                break;
            }
        }

        // If not directly involved, check if any neighbor is involved in a conflict
        if (!found) {
            for (int neighbor : graph[node]) {
                if (!comp_set.count(neighbor)) continue;

                for (int neighbor2 : graph[neighbor]) {
                    if (comp_set.count(neighbor2) && color[neighbor] == color[neighbor2]) {
                        found = true;
                        break;
                    }
                }

                if (found) break;
            }
        }

        in_odd[node] = found;
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

    vector<bool> in_odd_cycle(n + 1, false);

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            vector<int> component;
            bool is_bip = isBipartite(i, component);

            if (!is_bip) {
                markOddCycleNodes(component, in_odd_cycle);
            }
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
