// Problem: F. Tick, Tock
// Judge: Codeforces
// URL: https://codeforces.com/gym/667910/problem/F
// Memory Limit: 256 MB
// Time Limit: 1000 ms
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
    ll h;
    in(n, m, h);
    auto grid = make<int>(n, m);
    in(grid);

    // Weighted DSU State
    vector<int> parent(n + m);
    iota(all(parent), 0);
    vector<int> diff(n + m, 0);
    bool consistent = true;

    function<pair<int, int>(int)> find = [&](int i) -> pair<int, int> {
        if (parent[i] == i) return {i, 0};
        auto root = find(parent[i]);
        parent[i] = root.first;
        diff[i] = (diff[i] + root.second) % h;
        return {parent[i], diff[i]};
    };

    function<void(int, int, int)> unite = [&](int i, int j, int w) {
        auto root_i = find(i);
        auto root_j = find(j);
        int ri = root_i.first;
        int rj = root_j.first;
        int val_i = root_i.second;
        int val_j = root_j.second;

        int required_diff = (w - val_i + val_j) % h;
        if (required_diff < 0) required_diff += h;

        if (ri != rj) {
            parent[ri] = rj;
            diff[ri] = required_diff;
        } else {
            int current_diff = (val_i - val_j) % h;
            if (current_diff < 0) current_diff += h;
            if (current_diff != w) {
                consistent = false;
            }
        }
    };

    forn(i, n) {
        forn(j, m) {
            if (grid[i][j] != -1) {
                unite(i, n + j, grid[i][j]);
            }
        }
    }

    if (!consistent) {
        out(0);
        return;
    }

    // Basic DSU State for Component Counting
    vector<int> b_parent(n + m);
    iota(all(b_parent), 0);
    int components = n + m;

    function<int(int)> b_find = [&](int i) -> int {
        if (b_parent[i] == i) return i;
        return b_parent[i] = b_find(b_parent[i]);
    };

    function<void(int, int)> b_unite = [&](int i, int j) {
        int root_i = b_find(i);
        int root_j = b_find(j);
        if (root_i != root_j) {
            b_parent[root_i] = root_j;
            components--;
        }
    };

    // Add Filled
    forn(i, n) {
        forn(j, m) {
            if (grid[i][j] != -1) {
                b_unite(i, n + j);
            }
        }
    }
    int k_pre = components;

    // Add Empty
    forn(i, n) {
        forn(j, m) {
            if (grid[i][j] == -1) {
                b_unite(i, n + j);
            }
        }
    }
    int k_final = components;

    function<ll(ll, ll)> power = [&](ll base, ll exp) -> ll {
        ll res = 1;
        ll mod = 1000000007;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return res;
    };

    out(power(h, k_pre - k_final));
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
