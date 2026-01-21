// Problem: A. Binary Literature
// Judge: Codeforces
// URL: https://codeforces.com/contest/1508/problem/A
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
	int n;in(n);
	vector<string> s(3);
	in(s);
	
	vector<int> majType(3);
	forn(i, 3) {
		int cnt0 = 0;
		forn(j, 2 * n) {
			if (s[i][j] == '0') cnt0++;
		}
		majType[i] = (cnt0 >= n ? 0 : 1);
	}
	
	int id1 = -1, id2 = -1;
	if (majType[0] == majType[1]) { id1 = 0; id2 = 1; }
	else if (majType[0] == majType[2]) { id1 = 0; id2 = 2; }
	else { id1 = 1; id2 = 2; }
	
	char val = (majType[id1] == 0 ? '0' : '1');
	
	string ans = "";
	int p1 = 0, p2 = 0;
	
	auto can_p1 = function<bool()>( [&]() -> bool { return p1 < 2 * n; } );
	auto can_p2 = function<bool()>( [&]() -> bool { return p2 < 2 * n; } );
	
	while (can_p1() && can_p2()) {
		if (s[id1][p1] != val) {
			ans.pb(s[id1][p1++]);
		} else if (s[id2][p2] != val) {
			ans.pb(s[id2][p2++]);
		} else {
			ans.pb(val);
			p1++; p2++;
		}
	}
	
	while (can_p1()) ans.pb(s[id1][p1++]);
	while (can_p2()) ans.pb(s[id2][p2++]);
	
	out(ans);
}

int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}
