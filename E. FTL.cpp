// Problem: E. FTL
// Judge: Codeforces
// URL: https://codeforces.com/problemset/problem/1743/E
// Memory Limit: 512 MB
// Time Limit: 4000 ms
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
// CHASH -> Crazy Hellish Asseninely Stupid Hash 
static const uint64_t CHRONO_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
static const uint64_t PIDRNDP = (uint64_t)(getpid())*0xbffbffbffULL;//getpid is often blocked. therefore, use chrono AGAIN
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
// Check if the compiler supports __int128 before defining the alias
using longer_long = __int128; // <- meme, never has any CP problem ever required this
template<typename T1,typename T2>
using hashmap=unordered_map<T1,T2,CHASH>;
template<typename TM>
using matrix=vector<vector<TM>>;
template<typename TM>
using tensor=vector<matrix<TM>>;
#define all(x) (x).begin(),(x).end()
#define forn(i,n) for(int i=0;i<(n);++i)
#define forab(i,a,b) for(int i=(a);i<=(b);++i)
#define f0r(i,n) for(int i=(n)-1;i>=0;--i)
#define f0rab(i,a,b) for(int i=(b);i>=(a);--i)
#define KIMIKAYOKIMINANKAYO "hikarunara"
// ^ meme bc funny; if i ever see a Your Lie in April reference in CP problem IM GOING TO USE IT AND LOSE IT
#define MULTITEST false

void solve(){
	function<void(const ll)> main_ed=[&](ll INF64)->void{ // *sighs in lambda*
		vector<int> ps(2);
		vector<long long> ts(2);
		int h, s;
		function<void()> read=[&](){// **sighs in lambda**
			forn(i, 2) scanf("%d%lld", &ps[i], &ts[i]);
			scanf("%d%d", &h, &s);
		};
		function<void(vector<ll>&)> deepee=[&](vector<ll>& dp)->void{// ***SIGHING INTENSIFIES***
			dp[0] = 0;
			forn(i, h) for (int j = 1; j <= h - i; ++j) forn(k, 2){
				int ni = min((ll)h, i + j * (ps[k] - s) + j * ts[k] / ts[k ^ 1] * (ps[k ^ 1] - s));
				if (ni == h)
					INF64 = min(INF64, dp[i] + j * ts[k]);
				if (j * ts[k] >= ts[k ^ 1]){
					ni = min((long long)h, i + (j - 1) * (ps[k] - s) + (j * ts[k] / ts[k ^ 1] - 1) * (ps[k ^ 1] - s) + (ps[0] + ps[1] - s));
					dp[ni] = min(dp[ni], dp[i] + j * ts[k]);
				}
			}
		};
		read();
		vector<ll> dp(h + 1, *&INF64); // dereference AND reference to prevent overwrite issues
												 // ****STARTS WILDLY SIGHING IN LAMBDA****
		deepee(dp);
		function<ll(ll,ll)> minf=[&](ll a,ll b)->ll{
			return a<b?a:b;
		};
		// INF64 = min(INF64, dp[h]);
		INF64 = minf(INF64, dp[h]); // ******LAMBDA INTENSIFIES*******
		printf("%lld\n", INF64);
	};
	main_ed(1e18); 
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


