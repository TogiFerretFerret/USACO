#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
static const uint64_t CHRONO_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
static const uint64_t PIDRNDP = (uint64_t)(CHRONO_RANDOM)*0xdeadbeefULL;//the beef! it's dead!
static mt19937_64 PIDRNG(PIDRNDP);
static const uint64_t PIDRND = PIDRNG();
static const uint64_t FIXED_RANDOM = CHRONO_RANDOM^PIDRND;
struct CHASH {
    template <typename T>
    size_t operator()(const T& x) const {
        return hash<T>{}(x) ^ FIXED_RANDOM;
    }
    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2>& x) const {
        return (*this)(x.first) ^ ((*this)(x.second) + 0x9e3779b9 + (x.first << 6) + (x.first >> 2));
    }
};
template<typename T1,typename T2>
using hashmap=unordered_map<T1,T2,CHASH>;
template<typename TM>
using matrix=vector<vector<TM>>;
template<typename TM>
using tensor=vector<matrix<TM>>;
#define all(x) (x).begin(),(x).end()
#define forn(i,n) for(int i=0;i<(n);++i)
#define MULTITEST false
#define in(x) cin>>x
#define out(s) cout<<s<<'\n';
#define firn(i,s,n) for(int i=(s);i<(n);++i)
void solve(){
	int n,m;in(n>>m);
	tensor<int> adj(n+1);
	tensor<ll> cand(n+1);
	vector<int> deg(n),rnk(n),len(n);
	vector<ll> sum(n);
	forn(i,m){int u,v,l;in(u>>v>>l);adj[v].push_back({u,l});++deg[u];}
	vector<int> nodes;
	firn(i,1,n+1)if(!deg[i])nodes.push_back(i);
	while(nodes.size()){
		vector<int> n2;
		for(int u:nodes){for(vector<int> e:adj[u]){
			int v=e[0],l=e[1];
            cand[v].push_back({-len[u], l, rnk[u], sum[u]+l});
			--deg[v];if(!deg[v]){len[v]=len[u]+1;n2.push_back(v);}
		}}
		swap(nodes,n2);
		matrix<ll> v;
		forn(i,(int)nodes.size()){
			int x=nodes[i];sort(all(cand[x]));
			vector<ll> e=cand[x][0];sum[x]=e[3];v.push_back({e[1],e[2],x});
		}
		sort(all(v));
		forn(i,(int)v.size())rnk[v[i][2]]=i;
	}
	firn(i,1,n+1)out(len[i]<<' '<<sum[i]);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) in(t);
	forn(i,t)solve();
}


