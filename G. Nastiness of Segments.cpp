// Problem: G. Nastiness of Segments
// Judge: Codeforces
// URL: https://codeforces.com/contest/2184/problem/G
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
m1(debug) { cerr<< forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
template<typename T1,typename T2> using hashmap=unordered_map<T1,T2,CHASH>;
template<typename TM> using matrix=vector<vector<TM>>;
template<typename TM> using tensor=vector<matrix<TM>>;
template<typename TM> using hypermatrix=vector<tensor<TM>>;
template<typename TM,typename... Args> auto make(size_t first,Args... args){
	if constexpr(sizeof...(args)==0){
		return vector<TM>(first);
	} else {
		return vector<decltype(make<TM>(args...))>(first,make<TM>(args...));
	}
}	
#define all(x) (x).begin(),(x).end()
#define forn(i,n) for(int i=0;i<(n);++i)
#define MULTITEST true
#define pb push_back
void solve(){
	int n,q;in(n,q);
	vector<int> a(n);in(a);
	vector<int> st(4*n);
	function<void(int,int,int)> build = [&] (int v,int l, int r){
		if(l+1==r){
			st[v]=a[l];
			return;
		}
		int m=(l+r)/2;
		build(2*v+1,l,m);
		build(2*v+2,m,r);
		st[v]=min(st[2*v+1],st[2*v+2]);
	};
	build(0,0,n);
	function<void(int,int,int,int,int)> upd = [&] (int v,int l,int r,int idx,int val){
		if(l+1==r){
			st[v]=val;
			return;
		}
		int m=(l+r)/2;
		if(idx<m)upd(2*v+1,l,m,idx,val);
		else upd(2*v+2,m,r,idx,val);
		st[v]=min(st[2*v+1],st[2*v+2]);
	};
	function<int(int,int,int,int,int)> getmin = [&] (int v,int l,int r,int ql,int qr) -> int {
		if(ql>=r||qr<=l)return INT_MAX;
		if(ql<=l&&qr>=r)return st[v];
		int m=(l+r)/2;
		return min(getmin(2*v+1,l,m,ql,qr),getmin(2*v+2,m,r,ql,qr));
	};
	int ans=0,cmn=0;
	function<void(int,int,int,int,int)> findInter=[&](int v, int l, int r, int lq, int rq)->void {
		if (rq <= l || r <= lq || ans != (int) 1e6) return;
		if (lq <= l && r <= rq && min(cmn, st[v]) > r - lq - 1) {
			cmn = min(cmn, st[v]);
			return;
		}
		if (l + 1 == r) {
			ans = l;
			return;
		}
		int m = (r + l) / 2;
		findInter(v * 2 + 1, l, m, lq, rq);
		findInter(v * 2 + 2, m, r, lq, rq);
	};
	while(q--){
		int idx;in(idx);
		if(idx==1){
			int i,x;in(i,x);
			upd(0,0,n,i-1,x);
		} else {
			int l,r;in(l,r);
			--l;if(getmin(0,0,n,l,r)>r-l){out("0");continue;}
			ans=cmn=INT_MAX;
			findInter(0,0,n,l,r);
			out(ans!=INT_MAX&&getmin(0,0,n,l,ans+1)==ans-l);
		}
	}
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


