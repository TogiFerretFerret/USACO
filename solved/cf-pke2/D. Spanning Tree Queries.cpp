// Problem: D. Spanning Tree Queries
// Judge: Codeforces
// URL: https://codeforces.com/gym/667910/problem/D
// Memory Limit: 256 MB
// Time Limit: 4000 ms
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
#define m2(x) (int[]){(x std::forward<U>(b),0)...}
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
m1(out) { cout << std::forward<T>(a);  m2(cout << " " <<); cout << endl; }//softmod for interactive
m1(debug) { cerr << std::forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> std::forward<T>(a); m2(cin >>); }
#else
m1(out) { cout << std::forward<T>(a);  m2(cout << " " <<); cout << "\n"; }//softmod for interactive
m1(debug) { cerr << std::forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> std::forward<T>(a); m2(cin >>); }
#endif
#define MULTITEST false
#define pb push_back
void solve(){
    struct Edge {
        int u, v;
        int w;
        int id;
    };
    int n, m;
    in(n, m);
    vector<Edge> edges;
    forn(i, m) {
        int u, v, w;
        in(u, v, w);
        edges.pb({u, v, w, i});
    }

    int p;
    ll k, a, b, c;
    in(p, k, a, b, c);

    vector<ll> explicit_queries(p);
    in(explicit_queries);

    vector<int> critical_points;
    critical_points.pb(0);
    for(const Edge& e : edges) {
        critical_points.pb(e.w);
        critical_points.pb(e.w + 1);
    }
    forn(i, m) {
        for (int j = i + 1; j < m; ++j) {
            ll sum = (ll)edges[i].w + edges[j].w;
            int mid = (int)(sum / 2);
            critical_points.pb(mid);
            critical_points.pb(mid + 1);
        }
    }
    sort(all(critical_points));
    critical_points.erase(unique(all(critical_points)), critical_points.end());

    struct IntervalInfo {
        int start_x;
        ll slope;
        ll intercept;
    };
    vector<IntervalInfo> intervals;
    vector<int> p_indices(m);
    iota(all(p_indices), 0);

    for (size_t i = 0; i < critical_points.size(); ++i) {
        int start = critical_points[i];
        
        sort(all(p_indices), [&](int id1, int id2) -> bool {
            ll w1 = abs((ll)edges[id1].w - start);
            ll w2 = abs((ll)edges[id2].w - start);
            if (w1 != w2) return w1 < w2;
            return edges[id1].w < edges[id2].w;
        });

        vector<int> dsu(n + 1);
        iota(all(dsu), 0);
        function<int(int)> find = [&](int idx) -> int {
            if (dsu[idx] == idx) return idx;
            return dsu[idx] = find(dsu[idx]);
        };
        function<bool(int, int)> unite = [&](int u, int v) -> bool {
            int root_u = find(u);
            int root_v = find(v);
            if (root_u != root_v) {
                dsu[root_u] = root_v;
                return true;
            }
            return false;
        };

        ll current_slope = 0;
        ll current_intercept = 0;
        
        for (int idx : p_indices) {
            if (unite(edges[idx].u, edges[idx].v)) {
                int w = edges[idx].w;
                if (w >= start) {
                    current_intercept += w;
                    current_slope += -1;
                } else {
                    current_intercept -= w;
                    current_slope += 1;
                }
            }
        }
        intervals.pb({start, current_slope, current_intercept});
    }

    function<ll(ll)> get_cost = [&](ll x) -> ll {
        IntervalInfo target = { (int)x, 0, 0 };
        vector<IntervalInfo>::iterator it = upper_bound(all(intervals), target, [](const IntervalInfo& aa, const IntervalInfo& bb){
            return aa.start_x < bb.start_x;
        });
        --it;
        return it->slope * x + it->intercept;
    };

    ll final_xor = 0;
    ll last_q = 0;
    forn(i, k) {
        ll q;
        if (i < p) {
            q = explicit_queries[i];
        } else {
            q = (last_q * a + b) % c;
        }
        last_q = q;
        final_xor ^= get_cost(q);
    }
    out(final_xor);
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
