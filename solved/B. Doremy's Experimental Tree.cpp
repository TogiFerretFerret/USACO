// Problem: B. Doremy's Experimental Tree
// Judge: Codeforces
// URL: https://codeforces.com/gym/668106/problem/B
// Memory Limit: 256 MB
// Time Limit: 2500 ms
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
    int n;
    in(n);

    matrix<ll> f = make<ll>(n, n);
    vector<ll> diag(n);

    forn(i, n) {
        forn(j, i + 1) {
            in(f[i][j]);
            if (i == j) diag[i] = f[i][j];
            f[j][i] = f[i][j]; 
        }
    }

    matrix<ll> dist_matrix = make<ll>(n, n);
    forn(i, n) {
        forn(j, i + 1) {
            ll f_uv = f[i][j];
            ll d = (diag[i] + diag[j] - 2 * f_uv) / n;
            dist_matrix[i][j] = d;
            dist_matrix[j][i] = d;
        }
    }

    ll INF = 4e18;
    vector<ll> min_w(n, INF);
    vector<int> parent(n, -1);
    vector<int> visited(n, 0);

    struct Edge {
        int u, v;
        ll w;
    };
    vector<Edge> result_edges;

    min_w[0] = 0;

    forn(i, n) {
        int u = -1;
        ll current_min = INF;

        forn(v, n) {
            if (!visited[v] && min_w[v] < current_min) {
                current_min = min_w[v];
                u = v;
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        if (parent[u] != -1) {
            result_edges.pb({parent[u] + 1, u + 1, min_w[u]});
        }

        forn(v, n) {
            if (!visited[v]) {
                if (dist_matrix[u][v] < min_w[v]) {
                    min_w[v] = dist_matrix[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    for (const auto& edge : result_edges) {
        out(edge.u, edge.v, edge.w);
    }
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
