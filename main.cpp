#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <unordered_set>

using namespace std;

const int MAXN = 100005;
const int DEGREE_THRESHOLD = 50;  // Use full check for degree <= this

vector<int> graph[MAXN];
int color[MAXN];
bool visited[MAXN];
int n, m;

// Check if component is bipartite
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

// Fast path check with BFS
bool hasOddPath(int start, int end, int avoid) {
    if (start == avoid || end == avoid) return false;

    static int dist[MAXN];
    static bool vis[MAXN];

    fill(vis, vis + n + 1, false);
    fill(dist, dist + n + 1, -1);

    queue<int> q;
    q.push(start);
    dist[start] = 0;
    vis[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == end) {
            return dist[end] % 2 == 1;
        }

        for (int v : graph[u]) {
            if (!vis[v] && v != avoid) {
                vis[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return false;
}

// Check if node is in odd cycle
bool isInOddCycle(int node) {
    if (graph[node].size() <= 1) return false;

    // For low-degree nodes, do full check
    if (graph[node].size() <= DEGREE_THRESHOLD) {
        for (size_t i = 0; i < graph[node].size(); i++) {
            for (size_t j = i + 1; j < graph[node].size(); j++) {
                int u = graph[node][i];
                int v = graph[node][j];

                if (hasOddPath(u, v, node)) {
                    return true;
                }
            }
        }
        return false;
    }

    // For high-degree nodes, use heuristic (same color neighbors)
    for (size_t i = 0; i < graph[node].size(); i++) {
        for (size_t j = i + 1; j < graph[node].size(); j++) {
            int u = graph[node][i];
            int v = graph[node][j];

            if (color[u] == color[v]) {
                return true;
            }
        }
    }

    return false;
}

void markOddCycleNodes(const vector<int>& component, vector<bool>& in_odd) {
    for (int node : component) {
        in_odd[node] = isInOddCycle(node);
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
