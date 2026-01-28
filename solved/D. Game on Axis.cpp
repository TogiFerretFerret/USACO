// Problem: D. Game on Axis
// Judge: Codeforces
// URL: https://codeforces.com/problemset/problem/1787/D
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
    int n;
    in(n);
    vector<int> a(n + 1);
    fOrn(i, 1, n + 1) cin >> a[i];

    auto get_next = [&](int i) -> int {
        ll next_pos = (ll)i + a[i];
        if (next_pos < 1 || next_pos > n) return n + 1;
        return (int)next_pos;
    };

    vector<int> status(n + 2, 0);
    status[n + 1] = 2; // 2 = Exit (Good), 3 = Loop (Bad), 1 = Visiting

    fOrn(i, 1, n + 1) {
        if (status[i] != 0) continue;
        
        int curr = i;
        vector<int> path;
        while (curr != n + 1 && status[curr] == 0) {
            status[curr] = 1; 
            path.pb(curr);
            curr = get_next(curr);
        }

        int final_status = 0;
        if (curr == n + 1 || status[curr] == 2) {
            final_status = 2;
        } else {
            final_status = 3;
        }

        if (final_status == 3 && status[curr] == 1) { 
            for (int u : path) {
                status[u] = 3; 
            }
        } else {
            for (int u : path) status[u] = final_status;
        }
    }

    ll s_bad = 0;
    fOrn(i, 1, n + 1) {
        if (status[i] == 3) s_bad++;
    }

    vector<int> main_path;
    vector<int> visited_path(n + 2, 0);
    int curr = 1;
    bool path_loops = false;
    
    while (true) {
        if (curr == n + 1) break;
        if (visited_path[curr]) {
            path_loops = true;
            break;
        }
        visited_path[curr] = 1;
        main_path.pb(curr);
        curr = get_next(curr);
    }

    ll total_range = 2LL * n + 1;
    ll ans = 0;

    if (path_loops) {
        ans = (ll)main_path.size() * (total_range - s_bad);
    } else {
        ll non_path_nodes = n - main_path.size();
        ans += non_path_nodes * total_range;

        matrix<int> rev_adj(n + 2);
        fOrn(u, 1, n + 1) {
            if (status[u] == 2) {
                int v = get_next(u); 
                rev_adj[v].pb(u);
            }
        }

        vector<int> subtree_size(n + 2, 0);
        vector<int> q;
        q.pb(n + 1);
        vector<int> order; 
        
        int head = 0;
        while(head < q.size()){
            int u = q[head++];
            order.pb(u);
            for(int v : rev_adj[u]){
                q.pb(v);
            }
        }
        
        fill(all(subtree_size), 1);
        
        for(int k = order.size() - 1; k >= 0; --k){
            int u = order[k];
            for(int v : rev_adj[u]){
                subtree_size[u] += subtree_size[v];
            }
        }

        for (int x : main_path) {
            ll bad_moves = s_bad + subtree_size[x];
            ans += (total_range - bad_moves);
        }
    }

    out(ans);
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
