// Problem: Round Trip II
// Judge: CSES
// URL: https://cses.fi/problemset/task/1678
// Memory Limit: 512 MB
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
#define m2(x) (int[]){(x forward<U>(b),0)...}
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << "\n"; }
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
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
#define MULTITEST false
#define pb push_back
void solve(){
	int n,m;in(n,m);
	matrix<int> graph = matrix<int>(n);
	forn(i,m){
		int a,b;in(a,b);
		graph[a-1].pb(b-1);
	}
	vector<int> cycle;
	vector<bool> visited(n);
	vector<bool> onStack(n);
	function<bool(int)> dfs = [&](int node)->bool{
		visited[node]=onStack[node]=true;
		for(int next:graph[node]){
			if(onStack[next]){
				cycle.pb(node);
				onStack[node]=onStack[next]=false;
				return true;
			} else if (!visited[next]){
				if(dfs(next)){
					if(onStack[node]){
						cycle.pb(node);
						onStack[node]=false;
						return true;
					} else {
						cycle.pb(node);
						return false;
					}
				}
				if(!cycle.empty())return false;
			}
		}
		onStack[node]=false;
		return false;
	};
	for(int i=0;cycle.empty()&&i<n;++i)dfs(i);
	if(cycle.empty()){
		out("IMPOSSIBLE");
		return;
	}
	reverse(all(cycle));
	out(cycle.size()+1);
	for(int x:cycle)cout<<x+1<<" ";
	out(cycle[0]+1);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


