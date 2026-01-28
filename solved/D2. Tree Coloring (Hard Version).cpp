// Problem: D2. Tree Coloring (Hard Version)
// Judge: Codeforces
// URL: https://codeforces.com/contest/2183/problem/D2
// Memory Limit: 512 MB
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
    int n;
    in(n);
    matrix<int> e(n + 1);
    forn(i, n - 1) {
        int u, v;
        in(u, v);
        e[u].pb(v);
        e[v].pb(u);
    }

    vector<int> d(n + 1, 0), t(n + 1, 0), id(n + 1, 0), son(n + 1, 0), snum(n + 1, 0), fa(n + 1, 0);
    vector<int> flag(n + 2, 0);
    matrix<int> g(n + 1);
    matrix<int> ans_v(n + 1);

    function<void(int, int)> dfs = [&](int pos, int f) {
        fa[pos] = f;
        d[pos] = d[f] + 1;
        t[d[pos]]++;
        g[d[pos]].pb(pos);
        for(int x : e[pos]) {
            if(x == f) continue;
            snum[pos]++;
            dfs(x, pos);
        }
    };

    dfs(1, 0);

    int mx = 0, mt = 0;
    fOrn(i, 1, n + 1) {
        if(t[i] >= mx) {
            mx = t[i];
            mt = i;
        }
    }

    int ans = mx;
    fOrn(i, 1, mx + 1) {
        id[g[mt][i - 1]] = i;
        ans_v[i].pb(g[mt][i - 1]);
    }

    fOrn(i, mt + 1, n + 1) {
        vector<int> vfa;
        for(int j : g[i - 1]) {
            son[j] = 0;
            if(snum[j]) flag[id[j]] = 1;
        }
        int pos = 1;
        for(int j : g[i]) {
            if(!son[fa[j]]) {
                son[fa[j]] = j;
                vfa.pb(fa[j]);
            } else {
                while(flag[pos]) pos++;
                id[j] = pos;
                ans_v[pos].pb(j);
                pos++;
            }
        }
        while(flag[pos]) pos++;
        int lst = pos;
        for(int x : vfa) {
            id[son[x]] = lst;
            ans_v[lst].pb(son[x]);
            lst = id[x];
        }
        for(int j : g[i - 1]) flag[id[j]] = 0;
    }

    f0rn(i, mt - 1, 0) {
        vector<int> vfa;
        for(int j : g[i]) son[j] = 0;
        for(int j : g[i + 1]) {
            flag[id[j]] = 1;
            if(!son[fa[j]]) {
                son[fa[j]] = j;
                vfa.pb(fa[j]);
            }
        }
        
        if(vfa.size() == 1) {
            if(ans == t[i + 1]) {
                ans++;
                if (ans >= ans_v.size()) ans_v.resize(ans + 1);
                id[vfa[0]] = ans;
                ans_v[ans].pb(vfa[0]);
            } else {
                int pos = 1;
                while(flag[pos]) pos++;
                id[vfa[0]] = pos;
                ans_v[pos].pb(vfa[0]);
            }
        } else {
            forn(k, vfa.size()) {
                id[vfa[k]] = id[son[vfa[(k + 1) % vfa.size()]]];
                ans_v[id[vfa[k]]].pb(vfa[k]);
            }
        }
        
        for(int j : g[i + 1]) flag[id[j]] = 0;
        for(int x : vfa) flag[id[x]] = 1;
        
        int pos = 1;
        for(int x : g[i]) {
            if(!son[x]) {
                while(flag[pos]) pos++;
                id[x] = pos;
                ans_v[pos].pb(x);
                pos++;
            }
        }
        for(int x : vfa) flag[id[x]] = 0;
    }

    out(ans);
    fOrn(i, 1, ans + 1) {
        cout << ans_v[i].size() << " ";
        out(ans_v[i]);
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
