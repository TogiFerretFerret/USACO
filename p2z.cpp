#include<bits/stdc++.h>
using namespace std;
 
const int N=2e5+1;
vector<vector<int>> radj[N];
vector<vector<long long>> cand[N];
int deg[N], rnk[N], len[N];
long long sum[N];
 
int main(){
    int n, m;
    cin >> n >> m;
    for(int i=0; i<m; i++){
        int u,v,l; cin >> u >> v >> l;
        radj[v].push_back({u, l});
        deg[u]++;
    }
    vector<int> nodes;
    for(int i=1; i<=n; i++){
        if(!deg[i]) nodes.push_back(i);
    }
 
    // bfs from sinks
    while(int(nodes.size())){
        vector<int> nodes2;
 
        // update predecessors' deg, len, and cand
        for(int u:nodes){
            for(auto e:radj[u]){
                int v=e[0], l=e[1];
                cand[v].push_back({-len[u], l, rnk[u], sum[u]+l});
                deg[v]--;
                if(deg[v]==0){
                    len[v]=len[u]+1;
                    nodes2.push_back(v);
                }
            }
        }
        // next round's nodes to process
        swap(nodes, nodes2);
 
        // find the lex. min. path
        vector<vector<long long>> v;
        for(int i=0; i<nodes.size(); i++){
            int x=nodes[i];
            sort(cand[x].begin(), cand[x].end());
            vector<long long> use=cand[x][0];
            sum[x]=use[3];
            v.push_back({use[1], use[2], x});
        }
 
        // rank this level's nodes
        sort(v.begin(), v.end());
        for(int i=0; i<v.size(); i++){
            rnk[v[i][2]]=i;
        }
    }
 
    for(int i=1; i<=n; i++) cout << len[i] << " " << sum[i] << '\n';
}
