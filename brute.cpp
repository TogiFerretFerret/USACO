#include <bits/stdc++.h>
using namespace std;
using ll = long long;
struct DSU {
    vector<int> p;
    DSU(int n): p(n) { iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x ? x : p[x]=find(p[x]); }
    bool unite(int a, int b){ a=find(a); b=find(b); if(a==b) return false; p[b] = a; return true; }
};
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin>>n)) return 0;
    vector<int> a(n);
    for(int i=0;i<n;++i) cin>>a[i];
    if(n <= 1){ cout << 0 << '\n'; return 0; }
    vector<tuple<int,int,int>> edges;
    for(int i=0;i<n;++i) for(int j=i+1;j<n;++j) edges.emplace_back(a[i]^a[j], i, j);
    sort(edges.begin(), edges.end());
    DSU d(n);
    ll ans = 0; int cnt = 0;
    for(auto &e : edges){
        int w,i,j; tie(w,i,j) = e;
        if(d.unite(i,j)){ ans += w; ++cnt; if(cnt == n-1) break; }
    }
    cout << ans << '\n';
    return 0;
}

