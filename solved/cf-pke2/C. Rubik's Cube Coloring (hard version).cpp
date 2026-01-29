// Problem: C. Rubik's Cube Coloring (hard version)
// Judge: Codeforces
// URL: https://codeforces.com/gym/667910/problem/C
// Memory Limit: 256 MB
// Time Limit: 3000 ms
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
    ll K, N;
    in(K, N);

    hashmap<ll, int> constraints;
    hashmap<ll, int> important;
    vector<ll> G(65);
    // Create 6x0 matrix for adjacency
    matrix<int> adj = make<int>(6, 0); 
    ll MOD = 1e9 + 7;

    function<ll(ll, ll)> power = [&](ll base, ll exp) -> ll {
        ll res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    };

    function<int(string)> get_color_code = [&](string s) -> int {
        if (s == "white") return 0;
        if (s == "yellow") return 1;
        if (s == "green") return 2;
        if (s == "blue") return 3;
        if (s == "red") return 4;
        if (s == "orange") return 5;
        return -1;
    };

    function<void()> init_adj = [&]() -> void {
        forn(i, 6) {
            forn(j, 6) {
                if (i != j && i != (j ^ 1)) {
                    adj[i].pb(j);
                }
            }
        }
    };

    function<void()> init_G = [&]() -> void {
        ll mod_phi = MOD - 1;
        vector<ll> pow2(65);
        pow2[0] = 1;
        for(int i = 1; i <= 64; ++i) {
            pow2[i] = (pow2[i-1] * 2) % mod_phi;
        }
        for(int h = 0; h <= 60; ++h) {
            ll exponent = (pow2[h] - 1 + mod_phi) % mod_phi;
            G[h] = power(4, exponent);
        }
    };

    forn(i, N) {
        ll v;
        string s;
        in(v, s);
        constraints[v] = get_color_code(s);
        ll temp = v;
        while (temp >= 1) {
            important[temp] = 1;
            temp /= 2;
        }
    }

    init_adj();
    init_G();

    if (N == 0) important[1] = 1;

    function<vector<ll>(ll, int)> dfs;
    dfs = [&](ll u, int h) -> vector<ll> {
        int fixed_c = -1;
        if (constraints.count(u)) fixed_c = constraints[u];

        bool left_imp = false, right_imp = false;
        ll left_u = u << 1;
        ll right_u = (u << 1) + 1;

        vector<ll> left_res, right_res;

        if (h > 1) {
            if (important.count(left_u)) {
                left_imp = true;
                left_res = dfs(left_u, h - 1);
            }
            if (important.count(right_u)) {
                right_imp = true;
                right_res = dfs(right_u, h - 1);
            }
        }

        vector<ll> res(6, 0);

        forn(c, 6) {
            if (fixed_c != -1 && c != fixed_c) {
                res[c] = 0;
                continue;
            }

            if (h == 1) {
                res[c] = 1;
                continue;
            }

            ll left_val = 0;
            if (!left_imp) {
                left_val = G[h-1];
            } else {
                for(int neighbor : adj[c]) {
                    left_val = (left_val + left_res[neighbor]) % MOD;
                }
            }

            ll right_val = 0;
            if (!right_imp) {
                right_val = G[h-1];
            } else {
                for(int neighbor : adj[c]) {
                    right_val = (right_val + right_res[neighbor]) % MOD;
                }
            }

            res[c] = (left_val * right_val) % MOD;
        }
        return res;
    };

    vector<ll> root_res = dfs(1, K);
    ll total_ways = 0;
    for(ll w : root_res) {
        total_ways = (total_ways + w) % MOD;
    }
    out(total_ways);
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
