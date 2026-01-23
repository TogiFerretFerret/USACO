// Problem: PolandBall and Gifts
// Judge: Codeforces
// URL: https://codeforces.com/contest/755/problem/F
// Memory Limit: 256 MB
// Time Limit: 2000 ms
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
#define MULTITEST false
#define pb push_back

void solve(){
    int n, k;
    if(!(cin >> n >> k)) return;
    vector<int> p(n);
    in(p);
    forn(i, n) p[i]--;

    vector<int> lens;
    {
        vector<bool> vis(n, false);
        forn(i, n) {
            if (!vis[i]) {
                int curr = i;
                int length = 0;
                while (!vis[curr]) {
                    vis[curr] = true;
                    curr = p[curr];
                    length++;
                }
                lens.pb(length);
            }
        }
    }

    auto get_min = [&]() -> int {
        if (k == 0) return 0;
        if (k == n) return n;
        map<int, int> counts;
        for (int l : lens) counts[l]++;
        
        static bitset<1000001> can;
        can.reset();
        can[0] = 1;
        for (auto const& [val, count] : counts) {
            int rem = count;
            for (ll j = 1; j <= rem; j <<= 1) {
                ll shift = (ll)val * j;
                if (shift <= k) can |= (can << (size_t)shift);
                rem -= (int)j;
            }
            if (rem > 0) {
                ll shift = (ll)val * rem;
                if (shift <= k) can |= (can << (size_t)shift);
            }
            if (can[k]) return k;
        }
        return can[k] ? k : k + 1;
    };

    auto get_max = [&]() -> int {
        int pairs = 0;
        for (int l : lens) pairs += l / 2;
        if (k <= pairs) return 2 * k;
        return 2 * pairs + min(k - pairs, n - 2 * pairs);
    };

    out(get_min(), get_max());
}

int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}
