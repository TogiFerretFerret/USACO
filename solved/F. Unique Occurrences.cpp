// Problem: F. Unique Occurrences
// Judge: Codeforces
// URL: https://codeforces.com/problemset/problem/1681/F
// Memory Limit: 1024 MB
// Time Limit: 6000 ms
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
#define f0rn(v,s,e) for(int v=(s);v>(e);--v)
#define fOrn(v,s,e) for(int v=(s);v<(e);++v)
#define INTERACTIVE false
#ifndef LOCAL_JUDGE
#define FILEMODE false
#define FILENAME "pname"
#endif
#if INTERACTIVE
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << endl; }//softmod for interactive
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
#else
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << "\n"; }//softmod for interactive
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
#endif
#define MULTITEST false
#define pb push_back
void solve(){
    int n;
    in(n);
    struct Edge {
        int u, v, w;
    };
    vector<Edge> edges;
    edges.reserve(n - 1);
    vector<int> coords;
    coords.reserve(n - 1);

    forn(i, n - 1) {
        int u, v, w;
        in(u, v, w);
        --u; --v;
        edges.pb({u, v, w});
        coords.pb(w);
    }

    sort(all(coords));
    coords.erase(unique(all(coords)), coords.end());
    int k = coords.size();

    vector<vector<pair<int, int>>> edges_by_color(k);
    for (const auto& e : edges) {
        int c = lower_bound(all(coords), e.w) - coords.begin();
        edges_by_color[c].pb({e.u, e.v});
    }

    int size_st = 1;
    while(size_st < k) size_st *= 2;
    // Using standard constructor for container of non-primitive types (vector<pair>)
    vector<vector<pair<int, int>>> st(2 * size_st);

    function<void(int, int, int, int, int, const pair<int, int>&)> add_edge = 
        [&](int node, int l, int r, int ql, int qr, const pair<int, int>& e) {
        if (ql > qr) return;
        if (l >= ql && r <= qr) {
            st[node].pb(e);
            return;
        }
        int mid = (l + r) / 2;
        if (ql <= mid) add_edge(2 * node, l, mid, ql, min(qr, mid), e);
        if (qr > mid) add_edge(2 * node + 1, mid + 1, r, max(ql, mid + 1), qr, e);
    };

    forn(c, k) {
        // We need DSU to contain all edges EXCEPT color c.
        // So we add edges of color c to the ranges [0, c-1] and [c+1, k-1] in the timeline.
        for (const auto& e : edges_by_color[c]) {
            if (c > 0) add_edge(1, 0, k - 1, 0, c - 1, e);
            if (c < k - 1) add_edge(1, 0, k - 1, c + 1, k - 1, e);
        }
    }

    vector<int> p(n), sz(n, 1);
    iota(all(p), 0);
    vector<pair<int, int>> history;

    auto find = [&](int x) -> int {
        // No path compression allowed with rollback
        while (x != p[x]) x = p[x];
        return x;
    };

    auto unite = [&](int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (sz[x] < sz[y]) swap(x, y);
        p[y] = x;
        sz[x] += sz[y];
        history.pb({y, x});
    };

    auto rollback = [&](int snap) {
        while (history.size() > snap) {
            auto [y, x] = history.back();
            history.pop_back();
            p[y] = y;
            sz[x] -= sz[y];
        }
    };

    ll total_ans = 0;

    function<void(int, int, int)> traverse = [&](int node, int l, int r) {
        int snap = history.size();
        for (const auto& e : st[node]) {
            unite(e.first, e.second);
        }

        if (l == r) {
            if (l < k) {
                // At leaf representing color l.
                // Current DSU state contains all edges except those of color l.
                // We iterate over edges of color l to count contribution.
                for (const auto& e : edges_by_color[l]) {
                    int root_u = find(e.first);
                    int root_v = find(e.second);
                    // Edges of color l connect components formed by other edges.
                    // The path is unique in original tree, so u and v are disjoint in current DSU.
                    total_ans += (ll)sz[root_u] * sz[root_v];
                }
            }
        } else {
            int mid = (l + r) / 2;
            traverse(2 * node, l, mid);
            traverse(2 * node + 1, mid + 1, r);
        }

        rollback(snap);
    };

    if (k > 0) traverse(1, 0, k - 1);
    
    out(total_ans);
}
int main(){
    if(!INTERACTIVE)cin.tie(0)->sync_with_stdio(0);
    #ifndef LOCAL_JUDGE
    #if FILEMODE
    freopen(FILENAME".in","r",stdin);
    freopen(FILENAME".out","w",stdout);
    #endif
    #endif
    int t=1;
    if (MULTITEST) cin>>t;
    forn(i,t)solve();
}
