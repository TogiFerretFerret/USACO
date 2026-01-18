#include<bits/stdc++.h>
#define int long long
using namespace std;
vector<int> e[300005],v[300005],g[300005];
int n,d[300005],t[300005],id[300005],son[300005],snum[300005],fa[300005];
bool flag[300005];
void dfs(int pos,int fa)
{
	::fa[pos]=fa;
	d[pos]=d[fa]+1;
	t[d[pos]]++;
	g[d[pos]].push_back(pos);
	for(int x:e[pos])
	{
		if(x==fa)
			continue;
		snum[pos]++;
		dfs(x,pos);
	}
}
void test()
{
	cin>>n;
	for(int i=1;i<=n;i++)
		e[i].clear(),g[i].clear(),v[i].clear(),d[i]=t[i]=id[i]=snum[i]=0;
	for(int i=1;i<n;i++)
	{
		int u,v;
		cin>>u>>v;
		e[u].push_back(v);
		e[v].push_back(u);
	}
	dfs(1,0);
	int mx=0,mt=0;
	for(int i=1;i<=n;i++)
		if(t[i]>=mx)
			mx=t[i],mt=i;
//	for(int i=1;i<=n;i++)
//		cerr<<i<<" "<<fa[i]<<" "<<d[i]<<"\n";
	int ans=mx;
	for(int i=1;i<=mx;i++)
		id[g[mt][i-1]]=i,v[i].push_back(g[mt][i-1]);
	for(int i=mt+1;i<=n;i++)
	{
		vector<int> vfa;
		for(int j:g[i-1])
		{
			son[j]=0;
			if(snum[j])
				flag[id[j]]=1;
		}
		int pos=1;
		for(int j:g[i])
		{
			if(!son[fa[j]])
				son[fa[j]]=j,vfa.push_back(fa[j]);
			else
			{
				while(flag[pos])
					pos++;
				assert(pos<=mx);
				id[j]=pos,v[pos].push_back(j),pos++;
			}
		}
		while(flag[pos])
			pos++;
		assert(pos<=mx);
		int lst=pos;
		for(int x:vfa)
		{
			id[son[x]]=lst;
			v[lst].push_back(son[x]);
			lst=id[x];
		}
		for(int j:g[i-1])
			flag[id[j]]=0;
	}
	for(int i=mt-1;i>=1;i--)
	{
		vector<int> vfa;
		for(int j:g[i])
			son[j]=0;
		for(int j:g[i+1])
		{
			flag[id[j]]=1;
			if(!son[fa[j]])
				son[fa[j]]=j,vfa.push_back(fa[j]);
		}
		assert(!vfa.empty());
		if(vfa.size()==1)
		{
			if(ans==t[i+1])
				ans++,id[vfa[0]]=ans,v[ans].push_back(vfa[0]);
			else
			{
				int pos=1;
				while(flag[pos])
					pos++;
				assert(pos<=ans);
				id[vfa[0]]=pos,v[pos].push_back(vfa[0]);
			}
		}
		else
		{
			for(int i=0;i<vfa.size();i++)
				id[vfa[i]]=id[son[vfa[(i+1)%vfa.size()]]],v[id[vfa[i]]].push_back(vfa[i]);
		}
		for(int j:g[i+1])
			flag[id[j]]=0;
		for(int x:vfa)
			flag[id[x]]=1;
		int pos=1;
		for(int x:g[i])
			if(!son[x])
			{
				while(flag[pos])
					pos++;
				assert(pos<=ans);
				id[x]=pos,v[pos].push_back(x);
				pos++;
			}
		for(int x:vfa)
			flag[id[x]]=0;
	}
	cout<<ans<<"\n";
	for(int i=1;i<=ans;i++)
	{
		cout<<v[i].size()<<" ";
		for(int x:v[i])
			cout<<x<<" ";
		cout<<"\n";
	}
}
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin>>t;
    for(int i=1;i<=t;i++)
    	test();
}
