// Problem: E. Sum Over Zero
// Judge: Codeforces
// URL: https://codeforces.com/contest/1788/problem/E
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
    int n;
    in(n);
    vector<ll> a(n);
    in(a);

    vector<ll> P(n + 1, 0);
    forn(i, n) {
        P[i + 1] = P[i] + a[i];
    }

    vector<ll> sorted_P = P;
    sort(all(sorted_P));
    sorted_P.erase(unique(all(sorted_P)), sorted_P.end());

    function<int(ll)> get_rank = [&](ll val) -> int {
        return (int)(lower_bound(all(sorted_P), val) - sorted_P.begin()) + 1;
    };

    vector<int> dp(n + 1, 0);
    
    int max_rank = sorted_P.size();
    vector<int> tree(max_rank + 1, -1e9);

    function<void(int, int)> update = [&](int index, int value) {
        for (; index <= max_rank; index += index & -index) {
            tree[index] = max(tree[index], value);
        }
    };

    function<int(int)> query = [&](int index) -> int {
        int res = -1e9;
        for (; index > 0; index -= index & -index) {
            res = max(res, tree[index]);
        }
        return res;
    };

    update(get_rank(0), 0);

    fOrn(i, 1, n + 1) {
        dp[i] = dp[i - 1];
        
        int best_prev = query(get_rank(P[i]));
        
        if (best_prev > -5e8) {
            dp[i] = max(dp[i], best_prev + i);
        }
        
        update(get_rank(P[i]), dp[i] - i);
    }

    out(dp[n]);
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
