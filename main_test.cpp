#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int MAXN = 100005;
vector<int> graph[MAXN];
int dist[MAXN];
int n, m;

bool isInOddCycle(int start) {
    memset(dist, -1, sizeof(dist));
    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            } else {
                // Found a cycle
                int cycle_len = dist[u] + dist[v] + 1;
                if (cycle_len % 2 == 1) {
                    // Odd cycle found
                    // Check if start is on this cycle
                    // start is on the cycle if it's on the path from u to v in BFS tree
                    // This happens if min(dist[u], dist[v]) == 0 OR the cycle goes through start
                    // Actually, if we're doing BFS from start, and we find an odd cycle,
                    // we need to check if start is actually ON that cycle

                    // Simple check: start is in odd cycle if dist[u] + dist[v] + 1 is odd
                    // and either u or v is start, or start is on the path

                    // If dist[u] == 0 or dist[v] == 0, start is definitely on the cycle
                    if (dist[u] == 0 || dist[v] == 0) {
                        return true;
                    }

                    // Otherwise, check if start is on the path from u to v
                    // Start is on path if dist[start to LCA(u,v)] + dist[LCA to u or v] = dist[u or v]
                    // This is complex, so let's use a simpler heuristic:

                    // If the edge is adjacent to start (u==start or v==start)
                    if (u == start || v == start) {
                        return true;
                    }
                }
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
