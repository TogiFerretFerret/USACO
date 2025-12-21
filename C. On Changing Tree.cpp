#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MOD = 1000000007;
const int MAXN = 300005;

vector<int> adj[MAXN];
int tin[MAXN], tout[MAXN], depth[MAXN];
int timer;
ll bit1[MAXN], bit2[MAXN];

void add(ll *bit, int idx, int val) {
    for (; idx < MAXN; idx += idx & -idx)
        bit[idx] = (bit[idx] + val) % MOD;
}

void range_add(ll *bit, int l, int r, int val) {
    add(bit, l, val);
    add(bit, r + 1, (MOD - val) % MOD);
}

ll get_val(ll *bit, int idx) {
    ll res = 0;
    for (; idx > 0; idx -= idx & -idx)
        res = (res + bit[idx]) % MOD;
    return res;
}

void dfs(int u, int d) {
    tin[u] = ++timer;
    depth[u] = d;
    for (int v : adj[u]) {
        dfs(v, d + 1);
    }
    tout[u] = timer;
}

void solve() {
    int n;
    cin >> n;
    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        adj[p].push_back(i);
    }

    dfs(1, 0);

    int q;
    cin >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int v, x, k;
            cin >> v >> x >> k;
            ll val1 = (x + 1LL * depth[v] * k) % MOD;
            range_add(bit1, tin[v], tout[v], val1);
            range_add(bit2, tin[v], tout[v], k);
        } else {
            int v;
            cin >> v;
            ll ans = (get_val(bit1, tin[v]) - get_val(bit2, tin[v]) * depth[v]) % MOD;
            if (ans < 0) ans += MOD;
            cout << ans << '\n';
        }
    }
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	solve();
}
