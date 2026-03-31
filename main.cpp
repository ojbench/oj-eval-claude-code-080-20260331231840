#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int n, m;

// Check if there's an odd-length path from start to end, avoiding the node 'avoid'
bool hasOddPath(int start, int end, int avoid) {
    if (start == avoid || end == avoid) return false;

    int dist[MAXN];
    memset(dist, -1, sizeof(dist));

    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == end && dist[u] % 2 == 1) {
            return true;
        }

        for (int v : graph[u]) {
            if (v != avoid && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist[end] != -1 && dist[end] % 2 == 1;
}

bool isInOddCycle(int node) {
    // Check all pairs of neighbors
    vector<int>& neighbors = graph[node];

    for (size_t i = 0; i < neighbors.size(); i++) {
        for (size_t j = i + 1; j < neighbors.size(); j++) {
            int u = neighbors[i];
            int w = neighbors[j];

            // Check if there's an odd path from u to w not going through 'node'
            if (hasOddPath(u, w, node)) {
                return true;
            }
        }
    }

    return false;
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

    int answer = 0;

    for (int i = 1; i <= n; i++) {
        if (!isInOddCycle(i)) {
            answer++;
        }
    }

    cout << answer << endl;

    return 0;
}
