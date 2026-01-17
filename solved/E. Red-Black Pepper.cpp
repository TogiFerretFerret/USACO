// Problem: E. Red-Black Pepper
// Judge: Codeforces
// URL: https://codeforces.com/problemset/problem/1728/E
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
#define MULTITEST false
#define pb push_back
void solve(){
	function<ll(ll,ll,ll&,ll&)> gcd=[&](ll a,ll b,ll& x,ll& y)->ll{
		if(b==0){
			x=1; y=0;
			return a;
		}
		ll x1,y1;
		ll g=gcd(b,a%b,x1,y1);
		x=y1;
		y=x1-(a/b)*y1;
		return g;
	};
	function<bool(ll,ll,ll,ll&,ll&,ll&)> find=[&](ll a,ll b,ll c, ll&d, ll&e, ll&f)->bool {
		f=gcd(abs(a),abs(b),d,e);
		if(c%f)return 0;
		d*=(c/f)*(a<0?-1:1);
		e*=c/f*(b<0?-1:1);
		return 1;
	};
	int n;in(n);debug(n);
	vector<int> a(n), b(n),dif(n);
	ll sb=0;
	forn(i,n)in(a[i],b[i]),sb+=(ll)b[i],dif[i]=a[i]-b[i];
	sort(all(dif),greater<int>());
	vector<ll> bst(n + 1);
	forn(i,n+1)bst[i] = sb,sb+=(i<n?dif[i]:0);
	int mx = max_element(bst.begin(), bst.end()) - bst.begin();
	int m;in(m);
	forn(_, m){
		int x, y;in(x,y);
		ll xi,yi,f;
		if (!find(x,y,n,xi,yi,f)){
			out(-1);
			continue;
		}
		ll l=x*1ll*y/f;
		ll red=xi*1ll*x;
		red=red+(max(0ll, mx-red)+l-1)/l*l;
		red=red-max(0ll, red-mx)/l*l;
		ll ans=-1;
		if(red<=n)ans=max(ans, bst[red]);
		red -= l;
		if (red>=0)ans=max(ans, bst[red]);
		out(ans);
	}

}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


