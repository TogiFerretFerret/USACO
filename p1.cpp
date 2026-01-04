#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
static const uint64_t CHRONO_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
static const uint64_t PIDRNDP = (uint64_t)(CHRONO_RANDOM)*0xbffbffbffULL;//getpid is often blocked. therefore, use chrono AGAIN
static mt19937_64 PIDRNG(PIDRNDP);
static const uint64_t PIDRND = PIDRNG();
static const uint64_t FIXED_RANDOM = CHRONO_RANDOM ^ PIDRND;
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
	int n;cin>>n;
	matrix<int> m(n,vector<int>(n));
	forn(i,n-1){string s;in(s);firn(j,i+1,n)m[i][j]=s[j-i-1]-'0';}
	matrix<int> d(n,vector<int>(n));
	function<int(int,int)> c=[&](int l,int r)->int{int z=0;firn(j,l,r)z^=m[l][j]*d[j][r];return z;};
	int a=0;
	forn(i,n){firn(j,n-i,n){d[n-i-1][j]=m[n-i-1][j]^c(n-i-1,j);a+=d[n-i-1][j];}}
	out(a);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) in(t);
	forn(i,t)solve();
}


