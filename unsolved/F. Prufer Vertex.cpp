// Problem: F. Prufer Vertex
// Judge: Codeforces
// URL: https://codeforces.com/contest/2191/problem/F
// Memory Limit: 256 MB
// Time Limit: 2000 ms
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
static const uint64_t CHRONO_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
static const uint64_t PIDRNDP = (uint64_t)(getpid())*0xbffbffbffULL;
static mt19937_64 PIDRNG(PIDRNDP);
static const uint64_t PIDRND = PIDRNG();
static const uint64_t FIXED_RANDOM = CHRONO_RANDOM ^ PIDRND;
struct CHASH {
    template <typename T> size_t operator()(const T& x) const {
        return hash<T>{}(x) ^ FIXED_RANDOM;
    }
    template <typename T1, typename T2> size_t operator()(const pair<T1, T2>& x) const {
        return (*this)(x.first) ^ ((*this)(x.second) + 0x9e3779b9 + (x.first << 6) + (x.first >> 2));
    }
};
template<class T, class U> istream& operator>>(istream& i, pair<T, U>& p) { return i >> p.first >> p.second; }
template<class T, class U> ostream& operator<<(ostream& o, const pair<T, U>& p) { return o << p.first << " " << p.second; }
template<class T> istream& operator>>(istream& i, vector<T>& v) { for(auto& x : v) i >> x; return i; }
template<class T> ostream& operator<<(ostream& o, const vector<T>& v) { for(int i=0; i<v.size(); ++i) o << v[i] << (i==v.size()-1?"":" "); return o; }
#define m1(x) template<class T, class... U> void x(T&& a, U&&... b)
#define m2(x) (int[]){(x forward<U>(b),0)...}
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << "\n"; }
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
template<typename T1,typename T2> using hashmap=unordered_map<T1,T2,CHASH>;
template<typename TM> using matrix=vector<vector<TM>>;
using graph=matrix<int>;
template<typename TM> using tensor=vector<matrix<TM>>;
template<typename TM> using hypermatrix=vector<tensor<TM>>;
template<typename TM, TM Val = TM(), typename... Args> auto make(size_t first, Args... args){
	if constexpr(sizeof...(args) == 0){
		return vector<TM>(first, Val);
	} else {
		return vector<decltype(make<TM, Val>(args...))>(first, make<TM, Val>(args...));
	}
}
#define all(x) (x).begin(),(x).end()
#define forn(i,n) for(int i=0;i<(n);++i)
#define MULTITEST true
#define pb push_back
struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int ri = find(i);
        int rj = find(j);
        if(ri != rj) parent[ri] = rj;
    }
};

void solve(){
	const int MOD = 998244353;
    function<ll(ll,ll)> power=[&](ll base, ll exp)->ll {
        ll res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;    
    };
    function<ll(ll)> modInverse=[&](ll a)->ll {
        return power(a, MOD - 2);
    };
    int n, m;
    in(n, m);
    
    vector<vector<int>> adj(n + 1);
    DSU dsu(n);
    
    forn(i, m) {
        int u, v;
        in(u, v);
        adj[u].pb(v);
        adj[v].pb(u);
        dsu.unite(u, v);
    }

    int comps = 0;
    for(int i = 1; i <= n; ++i) if (dsu.parent[i] == i) comps++;
    
    if (comps == 1) {
        int target = n - 1;
        vector<int> parent(n + 1, 0);
        vector<bool> vis(n + 1, false);
        queue<int> q;
        q.push(n);
        vis[n] = true;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == target) break;
            for (int v : adj[u]) {
                if (!vis[v]) {
                    vis[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        int curr = target;
        while (parent[curr] != n) {
            curr = parent[curr];
        }
        
        vector<int> ans(n - 1, 0);
        ans[curr - 1] = 1;
        out(ans);
        return;
    }

    int root_n = dsu.find(n);
    int target_M = n - 1;
    int root_M = dsu.find(target_M);
    
    vector<int> sz(n + 1, 0);
    for(int i = 1; i <= n; ++i) sz[dsu.find(i)]++;
    
    ll sn = sz[root_n];
    ll sM = sz[root_M];
    int k = comps;
    
    ll prod_s = 1;
    vector<bool> is_root(n + 1, false);
    for(int i = 1; i <= n; ++i) {
        int r = dsu.find(i);
        if (!is_root[r]) {
            is_root[r] = true;
            prod_s = (prod_s * sz[r]) % MOD;
        }
    }
    
    ll nk3;
    if (k == 2) {
        nk3 = modInverse(n);
    } else {
        nk3 = power(n, k - 3);
    }
    ll Q = (nk3 * prod_s) % MOD;
    
    vector<ll> final_ans(n + 1, 0);
    
    vector<int> subtree_sz(n + 1, 0);
    {
        vector<int> nodes;
        queue<int> q;
        q.push(n);
        vector<bool> vis(n + 1, false);
        vis[n] = true;
        vector<int> p(n+1, 0);
        
        while(!q.empty()){
            int u = q.front(); q.pop();
            nodes.pb(u);
            for(int v : adj[u]) {
                if(!vis[v]) {
                    vis[v] = true;
                    p[v] = u;
                    q.push(v);
                }
            }
        }
        
        fill(all(subtree_sz), 1);
        for(int i = nodes.size() - 1; i >= 0; --i) {
            int u = nodes[i];
            if(u == n) continue;
            subtree_sz[p[u]] += subtree_sz[u];
        }
    }
    
    ll inv_sn = modInverse(sn);
    ll internal_mult = (Q * n) % MOD;
    internal_mult = (internal_mult * inv_sn) % MOD;
    
    for (int v : adj[n]) {
        final_ans[v] = (internal_mult * subtree_sz[v]) % MOD;
    }
    
    ll term_target = (sn + sM) % MOD;
    term_target = (term_target * modInverse((sn * sM) % MOD)) % MOD;
    ll val_target = (Q * term_target) % MOD;
    
    ll pre_non = (Q * sM) % MOD;
    pre_non = (pre_non * inv_sn) % MOD;
    
    for(int i = 1; i < n; ++i) { 
        int v = i;
        if (dsu.find(v) == root_n) continue; 
        
        if (dsu.find(v) == root_M) {
            final_ans[v] = val_target;
        } else {
            ll sj = sz[dsu.find(v)];
            final_ans[v] = (pre_non * modInverse(sj)) % MOD;
        }
    }
    
    vector<ll> res_vec;
    for(int i = 1; i < n; ++i) res_vec.pb(final_ans[i]);
    out(res_vec);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


