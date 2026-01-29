// Problem: E. Colorful Operations
// Judge: Codeforces
// URL: https://codeforces.com/gym/667910/problem/E
// Memory Limit: 256 MB
// Time Limit: 4000 ms
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
    int n, q;
    in(n, q);

    vector<ll> bit_tree(n + 2, 0);

    function<void(int, ll)> bit_add = [&](int i, ll delta)->void {
        for (; i <= n; i += i & -i) {
            bit_tree[i] += delta;
        }
    };

    function<void(int, int, ll)> bit_range_add = [&](int l, int r, ll delta) ->void{
        bit_add(l, delta);
        bit_add(r + 1, -delta);
    };

    function<ll(int)> bit_query = [&](int i) -> ll {
        ll sum = 0;
        for (; i > 0; i -= i & -i) {
            sum += bit_tree[i];
        }
        return sum;
    };

    struct Node {
        int l, r;
        mutable int c;
        bool operator<(const Node& other) const {
            return l < other.l;
        }
    };

    set<Node> odt;
    vector<ll> lazy(n + 1, 0);
    odt.insert({1, n, 1});

    function<set<Node>::iterator(int)> split = [&](int x) -> set<Node>::iterator {
        if (x > n) return odt.end();
        set<Node>::iterator it = odt.lower_bound({x, 0, 0});
        if (it != odt.end() && it->l == x) return it;
        --it;
        int l = it->l, r = it->r, c = it->c;
        odt.erase(it);
        odt.insert({l, x - 1, c});
        return odt.insert({x, r, c}).first;
    };

    function<void(int, int, int)> perform_color = [&](int l, int r, int new_c) -> void {
        _Rb_tree_const_iterator<Node> itr = split(r + 1);
        _Rb_tree_const_iterator<Node> itl = split(l);
        
        _Rb_tree_const_iterator<Node> it = itl;
		
        while (it != itr) {
            ll diff = lazy[it->c] - lazy[new_c];
            if (diff != 0) {
                bit_range_add(it->l, it->r, diff);
            }
            it = odt.erase(it);
        }
        odt.insert({l, r, new_c});
    };

    forn(k, q) {
        string type;
        in(type);
        if (type[0] == 'C') {
            int l, r, c;
            in(l, r, c);
            perform_color(l, r, c);
        } else if (type[0] == 'A') {
            int c;
            ll x;
            in(c, x);
            lazy[c] += x;
        } else if (type[0] == 'Q') {
            int i;
            in(i);
            set<Node>::iterator it = odt.upper_bound({i, 0, 0});
            --it;
            int c = it->c;
            ll val = bit_query(i);
            out(val + lazy[c]);
        }
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
