// Problem: J. Two Binary Strings Problem
// Judge: ICPC Shanghai 2021
// URL: https://codeforces.com/gym/103446/problem/J
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
    template <typename T>
    size_t operator()(const T& x) const {
        return hash<T>{}(x) ^ FIXED_RANDOM;
    }
    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2>& x) const {
        return (*this)(x.first) ^ ((*this)(x.second) + 0x9e3779b9 + (x.first << 6) + (x.first >> 2));
    }
};
template<class T, class U>
istream& operator>>(istream& i, pair<T, U>& p) { return i >> p.first >> p.second; }
template<class T, class U>
ostream& operator<<(ostream& o, const pair<T, U>& p) { return o << p.first << " " << p.second; }
template<class T>
istream& operator>>(istream& i, vector<T>& v) { for(auto& x : v) i >> x; return i; }
template<class T>
ostream& operator<<(ostream& o, const vector<T>& v) { for(int i=0; i<v.size(); ++i) o << v[i] << (i==v.size()-1?"":" "); return o; }
#define m1(x) template<class T, class... U> void x(T&& a, U&&... b)
#define m2(x) (int[]){(x forward<U>(b),0)...}
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << "\n"; }
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
template<typename T1,typename T2>
using hashmap=unordered_map<T1,T2,CHASH>;
template<typename TM>
using matrix=vector<vector<TM>>;
using graph=matrix<int>;
template<typename TM>
using tensor=vector<matrix<TM>>;
template<typename TM>
using hypermatrix=vector<tensor<TM>>;
template<typename TM, TM Val = TM(), typename... Args>
auto make(size_t first, Args... args){
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
    int n;
    in(n);
    string A, B;
    in(A, B);

    vector<int> S(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        S[i + 1] = S[i] + (A[i] == '1' ? 1 : -1);
    }

    int limit_k = n + 1;
    for (int i = 1; i <= n; ++i) {
        bool more_ones = (S[i] > 0);
        int expected = (more_ones ? 1 : 0);
        int actual = (B[i - 1] - '0');
        if (expected != actual) {
            limit_k = min(limit_k, i + 1);
        }
    }

    vector<int> p(n + 1);
    iota(all(p), 0);
    sort(all(p), [&](int i, int j) {
        return S[i] < S[j];
    });

    static bitset<50005> rev_less;
    static bitset<50005> rev_geq;
    static bitset<50005> ans;
    static bitset<50005> mask_all;
    
    ans.set();
    mask_all.set();
    rev_geq.set();
    rev_less.reset();

    for (int k = 0; k <= n; ) {
        int r = k;
        while (r <= n && S[p[r]] == S[p[k]]) {
            r++;
        }
        
        for (int idx = k; idx < r; ++idx) {
            int i = p[idx]; 
            if (i == 0) continue; 
            
            int b_val = B[i - 1] - '0';
            
            if (b_val == 1) {
                ans &= (rev_less >> (n - i)) | (mask_all << (i + 1));
            } else {
                ans &= (rev_geq >> (n - i)) | (mask_all << (i + 1));
            }
        }
        
        for (int idx = k; idx < r; ++idx) {
            int j = p[idx];
            rev_less[n - j] = 1;
            rev_geq[n - j] = 0;
        }
        
        k = r;
    }

    string res = "";
    for (int k = 1; k <= n; ++k) {
        if (k >= limit_k) res += '0';
        else res += (ans[k] ? '1' : '0');
    }
    out(res);
}

int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}
