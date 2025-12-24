#include <bits/stdc++.h>
using namespace std;

int p[4005];
int pos[4005];
int n;

void perform_swap(int u, int v) {
    swap(p[u], p[v]);
    pos[p[u]] = u;
    pos[p[v]] = v;
}

void ask(int u, int v) {
    cout << "? " << u << " " << v << endl;
    int ru, rv;
    cin >> ru >> rv;
    if (ru == -1) exit(0);
    perform_swap(ru, rv);
}

void solve() {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> p[i];
        pos[p[i]] = i;
    }

    // Step 0: Handle odd n, move (n+1)/2 to center
    if (n % 2 != 0) {
        int mid = (n + 1) / 2;
        while (pos[mid] != mid) {
            ask(pos[mid], mid);
        }
    }

    // Phase 1: Make pairs symmetric
    for (int x = 1; x <= n / 2; ++x) {
        int y = n + 1 - x;
        if (pos[x] + pos[y] != n + 1) {
            ask(pos[x], n + 1 - pos[y]);
        }
    }

    // Phase 2: Move groups to correct target positions
    for (int x = 1; x <= n / 2; ++x) {
        int y = n + 1 - x;
        // Goal: x at index x, y at index y
        while (pos[x] != x || pos[y] != y) {
            if (pos[x] == x) {
                // x is fixed, try to fix y
                ask(y, pos[y]);
            } else {
                // try to fix x
                ask(x, pos[x]);
            }
        }
    }

    cout << "!" << endl;
}

int main() {
    int t;
    if (cin >> t) {
        while (t--) solve();
    }
    return 0;
}