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
    set<int> comp_set(component.begin(), component.end());

    for (int node : component) {
        if (graph[node].size() <= 1) {
            in_odd[node] = false;
            continue;
        }

        // Check if any two neighbors have the same color
        // This indicates node is likely in an odd cycle
        bool found = false;
        for (int i = 0; i < (int)graph[node].size() && !found; i++) {
            int u = graph[node][i];
            if (!comp_set.count(u)) continue;

            for (int j = i + 1; j < (int)graph[node].size() && !found; j++) {
                int v = graph[node][j];
                if (!comp_set.count(v)) continue;

                if (color[u] == color[v]) {
                    found = true;
                }
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
