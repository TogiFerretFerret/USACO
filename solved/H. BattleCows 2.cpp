// Problem: H. BattleCows 2
// Judge: Codeforces
// URL: https://codeforces.com/contest/2185/problem/H
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
	int n;ll k;in(n,k);
	vector<ll> a(n),s(n+1,0),val(n+1),ans(n,0);in(a);
	forn(i,n)s[i+1]=s[i]+a[i];
	vector<int> g(n+2,0),sfb(n+2,0);
	forn(i,n){
		ll diff=a[i]-s[i];
		val[i+1]=diff;
		g[i+1]=(diff>0?1:g[i+1]);
	}
	for(int i=n;i>=1;--i)sfb[i]=sfb[i+1]+g[i];
	function<int(int)> fsb=[&](int lim)->int{
		int l=1,r=n+1;// bee dairy search (i promis i can spell binary sorch when i want to [xddd])
		int res=n+2;
		while(l<=r){
			int mid=l+(r-l)/2;
			if(sfb[mid]<=lim)res=mid,r=mid-1;
			else l=mid+1;
		}
		return res;
	};
	function<int(int,ll)> fpt=[&](int start, ll thresh)->int{
		vector<ll>::iterator it = lower_bound(s.begin()+start,s.end(), thresh);// expanding auto is hot trust
		if(it==s.end())return n+1;
		return (int)(it-s.begin());
	};
	forn(i,n){
		int cp=i+1;ll ai=a[i];
		int idx=fpt(cp+1,2*ai); // ai hoshino from oshi no ko [my brain when writing an oshi no ko fic right before the contest]
		int r1right=min(n,idx-1),r1left=cp+1;
		if(r1left<=r1right){
			int need=fsb(k);
			int start=max(r1left,need-1);
			if(start<=r1right)ans[i]+=(r1right-start+1);
		}
		int r2left=idx,r2right=n;
		if(r2left<=r2right){
			int need=fsb(k-1);
			int start=max(r2left,need-1);
			if(start<=r2right)ans[i]+=(r2right-start+1);
		}
	}
	struct query {ll val; int id;};
	vector<query> queries(n);
	vector<pair<ll,int>> points(n);
	forn(i,n)queries[i]={a[i], i};
	forn(i,n)points[i]={val[i+1],i+1};
	function<bool(const query&,const query&)> compquery=[&](const query& a, const query& b)->bool{
		return a.val>b.val;
	};
	function<bool(const pair<ll,int>&, const pair<ll,int>&)> comppooint=[&](const pair<ll,int>& a, const pair<ll,int>& b)->bool{
		return a.first>b.first;
	};
	sort(all(points),comppooint);
	sort(all(queries),compquery);
	vector<int> bit(n+5,0); // bee dairy index tree
	function<void(int)> bitadd=[&](int idx)->void{
		for(;idx<=n;idx+=idx&-idx)bit[idx]++;
	};
	function<int(int)> bitquery=[&](int idx)->int{
		int s=0;
		for(;idx>0;idx-=idx&-idx)s+=bit[idx];
		return s;
	};
	function<int(int)> bsearch=[&](int target)->int{
		if(target<=0)return 0; // redundancy! see my blog post on redundancy https://riverxia.vercel.app/blog?post=2
		int idx=0,accum=0;
		for(int i=1<<18;i>0;i>>=1){ // i love weird syntax https://riverxia.vercel.app/blog?post=3 (python but whatever)
			if(idx+i<=n && accum+bit[idx+i]<target){
				idx+=i;
				accum+=bit[idx];
			}
		}
		return idx+1;
	};
	int ptr=0;
	for(const query& q:queries){
		int i=q.id,cp1=i+1;
		while(ptr<points.size()&&points[ptr].first>q.val){
			bitadd(points[ptr].second);
			++ptr;
		}
		int sfc=sfb[cp1+1];
		if(sfc>k)continue;
		int allowed=(int)(k-sfc),ita=bitquery(cp1-1),pivot=(int)(lower_bound(all(s),a[i])-s.begin());
		int r0=min(pivot,cp1);
		if(r0>=1){
			int nbv=ita-allowed,limidx=0;
			if(nbv<=0)limidx=0;// redundancy is love redundancy is life
			else limidx=bsearch(nbv);
			int start=max(1,limidx+1);
			if(start<=r0)ans[i]+=(r0-start+1);
		}
		if(allowed-1>=0){
			int l1=pivot+1,r1=cp1;
			if(l1<=r1){
				int nbv=ita-(allowed-1),limidx=0;
				if(nbv<=0)limidx=0;// ok so maybe it's not REALLY redundant but there was another less-redundant way to do it :shrug:
				else limidx=bsearch(nbv);
				int start=max(l1,limidx+1);
				if(start<=r1)ans[i]+=(r1-start+1);
			}
		}
	}
	out(ans);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


