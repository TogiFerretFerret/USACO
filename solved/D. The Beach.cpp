// Problem: D. The Beach
// Judge: Codeforces
// URL: https://codeforces.com/problemset/problem/1753/D
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
#define MULTITEST false
#define pb push_back
void solve(){
    int n, m;
    ll P, Q;
    in(n, m, P, Q);
    vector<string> grid(n);
    in(grid);
    ll INF = 1e18;
    vector<ll> dist(n * m, INF);
    vector<int> dr = {-1, 1, 0, 0};
    vector<int> dc = {0, 0, -1, 1};

    function<int(int, int)> id = [&](int r, int c) -> int { return r * m + c; };
    function<bool(int, int)> valid = [&](int r, int c) -> bool { return r >= 0 && r < n && c >= 0 && c < m; };

    function<ll(int, int, int, int)> solve_eviction = [&](int r1, int c1, int r2, int c2) -> ll {
        ll res = INF;
        function<void(int, int, int, int)> try_move_part = [&](int p1r, int p1c, int p2r, int p2c) -> void {
            forn(i, 4) {
                int kr = p1r + dr[i];
                int kc = p1c + dc[i];
                if (!valid(kr, kc) || grid[kr][kc] == '#' || (kr == p2r && kc == p2c)) continue;
                if (dist[id(kr, kc)] == INF) continue;

                bool shift1 = false;
                if ((kr == p1r && p1r == p2r) || (kc == p1c && p1c == p2c)) shift1 = true;
                ll cost1 = dist[id(kr, kc)] + (shift1 ? Q : P);

                forn(j, 4) {
                    int zr = kr + dr[j];
                    int zc = kc + dc[j];
                    if (!valid(zr, zc) || grid[zr][zc] == '#') continue;
                    if (zr == p1r && zc == p1c) continue; 

                    ll cost_z = (zr == p2r && zc == p2c) ? 0 : dist[id(zr, zc)];
                    if (cost_z == INF) continue;

                    bool shift2 = false;
                    if ((zr == kr && kr == p1r) || (zc == kc && kc == p1c)) shift2 = true;
                    res = min(res, cost1 + cost_z + (shift2 ? Q : P));
                }
            }
        };
        try_move_part(r1, c1, r2, c2); 
        try_move_part(r2, c2, r1, c1); 
        return res;
    };

    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    forn(r, n) {
        forn(c, m) {
            if (grid[r][c] == '.') {
                dist[id(r, c)] = 0;
                pq.push({0, id(r, c)});
            }
        }
    }

    while (!pq.empty()) {
        ll d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        int r = u / m;
        int c = u % m;

        forn(i, 4) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            if (!valid(nr, nc) || grid[nr][nc] == '.' || grid[nr][nc] == '#') continue;

            int r1 = nr, c1 = nc; 
            int r2, c2; 
            char t = grid[r1][c1];
            if (t == 'L') { r2 = r1; c2 = c1 + 1; }
            else if (t == 'R') { r2 = r1; c2 = c1 - 1; }
            else if (t == 'U') { r2 = r1 + 1; c2 = c1; }
            else { r2 = r1 - 1; c2 = c1; } 

            bool shift = false;
            if ((r == r1 && r1 == r2) || (c == c1 && c1 == c2)) shift = true;

            ll weight = shift ? Q : P;
            int target = id(r2, c2); 

            if (dist[u] + weight < dist[target]) {
                dist[target] = dist[u] + weight;
                pq.push({dist[target], target});
            }
        }
    }

    ll ans = INF;

    forn(r, n) {
        forn(c, m) {
            if (c + 1 < m) {
                int r2 = r, c2 = c + 1;
                bool same_sunbed = false;
                char t1 = grid[r][c];
                char t2 = grid[r2][c2];
                if (t1 == 'L' && t2 == 'R') same_sunbed = true; 

                if (same_sunbed) {
                    ans = min(ans, solve_eviction(r, c, r2, c2));
                } else {
                    if (dist[id(r, c)] != INF && dist[id(r2, c2)] != INF)
                        ans = min(ans, dist[id(r, c)] + dist[id(r2, c2)]);
                }
            }

            if (r + 1 < n) {
                int r2 = r + 1, c2 = c;
                bool same_sunbed = false;
                char t1 = grid[r][c];
                char t2 = grid[r2][c2];
                if (t1 == 'U' && t2 == 'D') same_sunbed = true;

                if (same_sunbed) {
                    ans = min(ans, solve_eviction(r, c, r2, c2));
                } else {
                    if (dist[id(r, c)] != INF && dist[id(r2, c2)] != INF)
                        ans = min(ans, dist[id(r, c)] + dist[id(r2, c2)]);
                }
            }
        }
    }

    if (ans >= INF) out(-1);
    else out(ans);
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
