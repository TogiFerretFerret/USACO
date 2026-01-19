// Problem: G. Sakura Adachi and Optimal Sequences
// Judge: Codeforces
// URL: https://codeforces.com/contest/2171/problem/G
// Memory Limit: 256 MB
// Time Limit: 4000 ms
// OMG I LOVE YURIIII! edit: LET'S GO ITS THE YURI CONTEST!!! https://codeforces.com/contest/2171
// edits: making list 
// problem F: in love with the villainess
// Problem G: light novel; adachi and shimamura
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
static const uint64_t CHRONO_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
static const uint64_t PIDRNDP = (uint64_t)(getpid())*0x1f4ef1337fULL;
static mt19937_64 PIDRNG(PIDRNDP);
static const uint64_t PIDRND = PIDRNG();
static const uint64_t FIXED_RANDOM = CHRONO_RANDOM ^ PIDRND;
struct CHASH {
    template <typename T> size_t operator()(const T& x) const {
        return hash<T>{}(x) ^ FIXED_RANDOM;
    }
    template <typename T1, typename T2> size_t operator()(const pair<T1, T2>& x) const {
        return (*this)(x.first) ^ ((*this)(x.second) + 0x9e1337e9 + (x.first << 6) + (x.first >> 2));
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
//////////////////////////////////////////////////////////////////////////////
const ll mod=1e6+3;vector<ll> fact(mod);
void presolve(){
	fact[0]=1;
	forn(i,mod)fact[i+1]=fact[i]*(i+1)%mod;
}
void solve(){
	function<ll(ll)> modinv = [&](ll a)->ll{
		return a<=1?a:mod-mod/a*modinv(mod%a)%mod; // mod mod mod mod mod mod mod i'm invincible by ado drums to not sound garbage plzz on spotify
	};
	ll n,x,ans=1;in(n);vector<ll> a(n),b(n);
	in(a,b);
	ll k = __lg(b[0] / a[0]);
    for(ll i=1; i<n; i++)
		k = min(k, __lg(b[i] / a[i]));
    x = k;
    vector<ll> cnt(k), c(n);
    for(ll i=0; i<n; i++){
        for(ll j=0; j<k; j++){
            cnt[j] += b[i] & 1;
            b[i] >>= 1;
        }
        c[i] = b[i] - a[i];
    }
    for(ll j=0; j<k; j++){
        x += cnt[j];
        ans = ans * fact[cnt[j]] % mod;
    }
    ll last = accumulate(c.begin(), c.end(), 0LL);
    ans = (last < mod ? ans * fact[last] % mod : 0);
    for(ll i : c){
        x += i;
        ans = ans * modinv(fact[i]) % mod;
    }
	out(x, ans);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	presolve();
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


