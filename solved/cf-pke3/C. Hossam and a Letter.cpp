// Problem: C. Hossam and a Letter
// Judge: Codeforces
// URL: https://codeforces.com/gym/668106/problem/C
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
    int n, m;
    in(n, m);

    // 0=Perfect(.), 1=Medium(m), 2=Bad(#)
    auto grid = make<int>(n, m);

    forn(i, n) {
        string s;
        in(s);
        forn(j, m) {
            if (s[j] == '.') grid[i][j] = 0;
            else if (s[j] == 'm') grid[i][j] = 1;
            else grid[i][j] = 2;
        }
    }

    auto P_Bad = make<int>(n, m + 1);
    auto P_M = make<int>(n, m + 1);

    forn(i, n) {
        P_Bad[i][0] = 0;
        P_M[i][0] = 0;
        forn(j, m) {
            P_Bad[i][j+1] = P_Bad[i][j] + (grid[i][j] == 2);
            P_M[i][j+1] = P_M[i][j] + (grid[i][j] == 1);
        }
    }

    int max_walls = 0;
    
    // DP arrays reuse
    vector<int> up0(n), up1(n);
    vector<int> down0(n), down1(n);

    forn(c1, m) {
        fOrn(c2, c1 + 2, m) {
            // Upward scan
            forn(r, n) {
                if (grid[r][c1] == 2 || grid[r][c2] == 2) {
                    up0[r] = 0;
                    up1[r] = 0;
                } else {
                    int cost = (grid[r][c1] == 1) + (grid[r][c2] == 1);
                    int prev0 = (r > 0) ? up0[r-1] : 0;
                    int prev1 = (r > 0) ? up1[r-1] : 0;
                    
                    if (cost == 0) {
                        up0[r] = prev0 + 1;
                        up1[r] = prev1 + 1;
                    } else if (cost == 1) {
                        up0[r] = 0;
                        up1[r] = prev0 + 1;
                    } else {
                        up0[r] = 0;
                        up1[r] = 0;
                    }
                }
            }

            // Downward scan
            f0rn(r, n - 1, -1) {
                if (grid[r][c1] == 2 || grid[r][c2] == 2) {
                    down0[r] = 0;
                    down1[r] = 0;
                } else {
                    int cost = (grid[r][c1] == 1) + (grid[r][c2] == 1);
                    int prev0 = (r < n - 1) ? down0[r+1] : 0;
                    int prev1 = (r < n - 1) ? down1[r+1] : 0;

                    if (cost == 0) {
                        down0[r] = prev0 + 1;
                        down1[r] = prev1 + 1;
                    } else if (cost == 1) {
                        down0[r] = 0;
                        down1[r] = prev0 + 1;
                    } else {
                        down0[r] = 0;
                        down1[r] = 0;
                    }
                }
            }

            int width = c2 - c1 - 1;
            fOrn(r, 1, n - 1) {
                if (grid[r][c1] == 2 || grid[r][c2] == 2) continue;

                int bad_h = P_Bad[r][c2] - P_Bad[r][c1+1];
                if (bad_h > 0) continue;

                int m_h = P_M[r][c2] - P_M[r][c1+1];
                if (m_h > 1) continue;

                int m_joints = (grid[r][c1] == 1) + (grid[r][c2] == 1);
                int m_total_fixed = m_h + m_joints;
                
                if (m_total_fixed > 1) continue;

                int u0 = up0[r-1], u1 = up1[r-1];
                int d0 = down0[r+1], d1 = down1[r+1];

                if (u1 == 0 || d1 == 0) continue;

                int extension_len = 0;

                if (m_total_fixed == 1) {
                    if (u0 > 0 && d0 > 0) {
                        extension_len = u0 + d0;
                    } else continue;
                } else {
                    int opt1 = (u1 > 0 && d0 > 0) ? u1 + d0 : 0;
                    int opt2 = (u0 > 0 && d1 > 0) ? u0 + d1 : 0;
                    extension_len = max(opt1, opt2);
                    if (extension_len == 0) continue;
                }
                
                int current_total = width + 2 + 2 * extension_len;
                max_walls = max(max_walls, current_total);
            }
        }
    }
    out(max_walls);
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
