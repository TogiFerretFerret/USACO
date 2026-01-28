// Problem: D. Prufer Vertex
// Judge: Codeforces
// URL: https://codeforces.com/contest/2190/problem/D
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
#define MULTITEST true
#define pb push_back
void solve(){
    int n, m;
    in(n, m);

    matrix<int> g(n + 1);
    forn(i, m) {
        int u, v;
        in(u, v);
        g[u].pb(v); 
        g[v].pb(u);
    }

    const int mod = 998244353;
    vector<int> comp(n + 1, -1), parent(n + 1), sizes(n);
    vector<ll> dp(n + 1, 1);
    int k = 0;

    function<void(int, int)> dfs = [&](int v, int p) -> void {
        comp[v] = k; 
        sizes[k]++;
        for(auto to : g[v]) {
            if(to == p) continue;
            parent[to] = v;
            dfs(to, v);
            dp[v] += dp[to];
        }
    };

    f0rn(v, n, 0) {
        if(comp[v] == -1) {
            dfs(v, 0);
            k++;
        }
    }

    vector<int> sz(n + 1);
    fOrn(v, 1, n + 1) sz[v] = sizes[comp[v]];

    vector<ll> inv(n + 1); 
    inv[1] = 1;
    fOrn(i, 2, n + 1) {
        inv[i] = (mod - inv[mod % i] * (mod / i) % mod) % mod;
    }

    vector<ll> ans(n);
    ll p = 1;
    forn(i, k) p = (p * sizes[i]) % mod;

    vector<ll> n_power(n + 1);
    n_power[0] = 1; 
    fOrn(i, 1, n + 1) n_power[i] = (n_power[i - 1] * n) % mod;

    function<ll(int)> n_pow = [&](int k_exp) -> ll {
        if(k_exp >= 0) return n_power[k_exp];
        else if(k_exp == -1) return inv[n];
        else return 0;
    };

    if(comp[n] == comp[n - 1]) {
        int v = n - 1;
        while(parent[v] != n) v = parent[v];

        ans[v] = (p * n_pow(k - 2)) % mod;
    }
    else {
        fOrn(v, 1, n) {
            if(comp[v] == comp[n]) {
                if(parent[v] != n) continue;

                ll val = (dp[v] % mod * p % mod);
                val = (val * inv[sz[n]]) % mod;
                val = (val * n_pow(k - 2)) % mod;
                ans[v] = val;
            }
            else if(comp[v] == comp[n - 1]) {
                ll val = (p * inv[sz[n]]) % mod;
                val = (val * inv[sz[n - 1]]) % mod;
                val = (val * (sz[n] + sz[n - 1])) % mod;
                val = (val * n_pow(k - 3)) % mod;
                ans[v] = val;
            }
            else {
                ll val = ((ll)sz[v] * p) % mod;
                val = (val * inv[sz[n]]) % mod;
                val = (val * inv[sz[v]]) % mod;
                val = (val * n_pow(k - 3)) % mod;
                ans[v] = val;
            }
        }
    }

    vector<ll> res;
    fOrn(v, 1, n) res.pb(ans[v]);
    out(res);
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
