// Problem: E. The Robotic Rush
// Judge: Codeforces
// URL: https://codeforces.com/contest/2185/problem/E
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
#define m2(x) (int[]){(x forward<U>(b),0)...}
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << "\n"; }
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
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
#define MULTITEST true
#define pb push_back
void solve(){
	int n,m,k;
    in(n, m, k);vector<ll> a(n);in(a);
    vector<ll> b(m);in(b);string gdc;in(gdc);
    sort(all(b));
    matrix<int> adj_u(k + 2), adj_v(k + 2); // forgot i had this
    for(int i = 0; i < n; ++i) {
        ll u_val = k + 5; 
        ll v_val = k + 5; 
        vector<ll>::iterator it = lower_bound(all(b), a[i]);
        if(it != b.end()) {
            v_val = *it - a[i];
        }

        if(it != b.begin()) {
            u_val = a[i] - *prev(it);
        }

        if(u_val > k) u_val = k + 1;
        if(v_val > k) v_val = k + 1;

        adj_u[(int)u_val].pb(i);
        adj_v[(int)v_val].pb(i);
    }
    vector<int> dead(n, 0);
    int ac = n;
    ll cpos = 0,mip=cpos,mpo=mip;
    vector<int> ans;
    ans.reserve(k);
    for(char c : gdc) {
        if(c == 'L') {
            cpos--;
        } else {
            cpos++;
        }
        if(cpos < mip) {
            mip = cpos;
            ll dist = -mip; 
            if(dist <= k) {
                for(int id : adj_u[dist]) {
                    if(!dead[id]) {
                        dead[id] = 1;
                        ac--;
                    }
                }
            }
        }
        if(cpos > mpo) {
            mpo = cpos;
            ll dist = mpo; 
            if(dist <= k) {
                for(int id : adj_v[dist]) {
                    if(!dead[id]) {
                        dead[id] = 1;
                        ac--;
                    }
                }
            }
        }
        ans.pb(ac);
    }
    out(ans);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


