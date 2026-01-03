#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll mod = 1e9+7;

int k;
int n[50000];
int inf = 1e8;
ll compute_sum(vector<pair<int, int> > li){
	int maxn = 0;
	vector<ll> prefix_prod;
	vector<ll> suffix_prod;
	vector<ll> graphs[k];
	for(int i = 0; i<li.size(); i++){
		graphs[li[i].second].push_back(li[i].first);
	}
	for(int i = 0; i<k; i++){
		vector<ll> cnt(2*n[i]);
		maxn = max(maxn,2*n[i]);
		while(prefix_prod.size()<maxn){
			prefix_prod.push_back(1);
		}
		while(suffix_prod.size()<=maxn){
			suffix_prod.push_back(1);
		}
		for(int j = 0; j<graphs[i].size(); j++){
			cnt[graphs[i][j]]++;
		}
		for(int j = 0; j<2*n[i]; j++){
			if(j>0){
				cnt[j] += cnt[j-1];
			}
			prefix_prod[j] *= cnt[j];
			prefix_prod[j] %= mod;
		}
		suffix_prod[2*n[i]] *= cnt[2*n[i]-1];
		suffix_prod[2*n[i]] %= mod;
	}
	for(int i = 1; i<suffix_prod.size(); i++){
		suffix_prod[i] *= suffix_prod[i-1];
		suffix_prod[i] %= mod;
	}
	ll ans = 0LL;
	for(int i = 1; i<maxn; i++){
		ll cur_num = (prefix_prod[i]*suffix_prod[i])-(prefix_prod[i-1]*suffix_prod[i-1]);
		cur_num %= mod;
		ans += cur_num * (ll)i;
		ans %= mod;
	}
	if(ans<0LL){
		ans += mod;
	}
	return ans;
}
int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	vector<pair<int, int> > evens;
	vector<pair<int, int> > odds;
	vector<pair<int, int> > both;
	cin >> k;
	for(int i = 0; i<k; i++){
		int m;
		cin >> n[i] >> m;
		vector<int> adj[2*n[i]];
		for(int j = 0; j<m; j++){
			int a, b;
			cin >> a >> b;
			a--;
			b--;
			adj[a].push_back(n[i]+b);
			adj[b].push_back(n[i]+a);
			adj[n[i]+a].push_back(b);
			adj[n[i]+b].push_back(a);
		}
		vector<int> dist(2*n[i], inf);
		vector<int> li;
		dist[0] = 0;
		li.push_back(0);
		for(int j = 0; j<li.size(); j++){
			int now = li[j];
			for(int a = 0; a<adj[now].size(); a++){
				int to = adj[now][a];
				if(dist[to]==inf){
					dist[to] = dist[now]+1;
					li.push_back(to);
				}
			}
		}
		for(int j = 0; j<n[i]; j++){
			if(dist[j]<inf){
				evens.push_back(make_pair(dist[j],i));
			}
			if(dist[j+n[i]]<inf){
				odds.push_back(make_pair(dist[j+n[i]],i));
			}
			if(max(dist[j],dist[j+n[i]])<inf){
				both.push_back(make_pair(max(dist[j],dist[j+n[i]]),i));
			}
		}
	}
	ll ans = compute_sum(evens)+compute_sum(odds)-compute_sum(both);
	ans %= mod;
	if(ans<0LL){
		ans += mod;
	}
	cout << ans << "\n";
}
