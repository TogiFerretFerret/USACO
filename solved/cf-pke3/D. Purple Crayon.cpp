// Problem: D. Purple Crayon
// Judge: Codeforces
// URL: https://codeforces.com/gym/668106/problem/D
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
#define MULTITEST false
#define pb push_back
void solve(){
    int n, k;
    in(n, k);

    graph adj(n + 1);
    forn(i, n - 1) {
        int u, v;
        in(u, v);
        adj[u].pb(v);
        adj[v].pb(u);
    }

    vector<int> chain_lens;

    function<int(int, int)> dfs = [&](int u, int p) -> int {
        int max_h = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            int h = dfs(v, u);
            if (h > max_h) {
                if (max_h > 0) chain_lens.pb(max_h);
                max_h = h;
            } else {
                chain_lens.pb(h);
            }
        }
        return max_h + 1;
    };

    int root_chain = dfs(1, 0);
    chain_lens.pb(root_chain);

    sort(chain_lens.rbegin(), chain_lens.rend());

    ll max_score = -4e18; 
    ll current_S = 0;

    int limit = min((int)chain_lens.size(), k);
    
    for (int x = 1; x <= limit; ++x) {
        current_S += chain_lens[x - 1];

        vector<ll> r_candidates;
        r_candidates.pb(x);
        r_candidates.pb(k);
        ll mid1 = n / 2;
        ll mid2 = (n + 1) / 2;
        if (mid1 >= x && mid1 <= k) r_candidates.pb(mid1);
        if (mid2 >= x && mid2 <= k) r_candidates.pb(mid2);

        ll current_config_max = -4e18;

        for (ll r_val : r_candidates) {
            ll b_limit = n - current_S;
            vector<ll> b_candidates;
            b_candidates.pb(b_limit);
            if (mid1 <= b_limit) b_candidates.pb(mid1);
            if (mid2 <= b_limit) b_candidates.pb(mid2);
            
            ll best_blue_response_score = 4e18;
            
            for (ll b_val : b_candidates) {
                ll score = (ll)(n - r_val - b_val) * (r_val - b_val);
                if (score < best_blue_response_score) {
                    best_blue_response_score = score;
                }
            }
            
            if (best_blue_response_score > current_config_max) {
                current_config_max = best_blue_response_score;
            }
        }
        
        if (current_config_max > max_score) {
            max_score = current_config_max;
        }
    }

    out(max_score);
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
