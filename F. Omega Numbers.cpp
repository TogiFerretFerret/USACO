// Problem: F. Omega Numbers
// Judge: Codeforces
// URL: https://codeforces.com/contest/2176/problem/F
// Memory Limit: 512 MB
// Time Limit: 3000 ms
// Global constants

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
#define m2(x) (int[]){(x std::forward<U>(b),0)...}
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
#define f0rn(v,s,e) for(int v=(s);v>(e);--v)
#define fOrn(v,s,e) for(int v=(s);v<(e);++v)
#define INTERACTIVE false
#ifndef LOCAL_JUDGE
#define FILEMODE false
#define FILENAME "pname"
#endif
#if INTERACTIVE
m1(out) { cout << std::forward<T>(a);  m2(cout << " " <<); cout << endl; }//softmod for interactive
m1(debug) { cerr << std::forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> std::forward<T>(a); m2(cin >>); }
#else
m1(out) { cout << std::forward<T>(a);  m2(cout << " " <<); cout << "\n"; }//softmod for interactive
m1(debug) { cerr << std::forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> std::forward<T>(a); m2(cin >>); }
#endif
#define MULTITEST true
#define pb push_back
const int MAXN = 200005;
const int MOD = 998244353;

// Global Sieve Arrays
int min_prime[MAXN];
int mu[MAXN];
int omega[MAXN];
int core[MAXN];

// Global Solver Arrays
int freqs[MAXN];
int DSum[7][MAXN];
int Coeff[MAXN][13];
ll POW[14];


// Initialization function called once
void init() {
    fill(min_prime, min_prime + MAXN, 0);
    mu[1] = 1;
    omega[1] = 0;
    core[1] = 1;
    vector<int> primes;
    for (int i = 2; i < MAXN; ++i) {
        if (min_prime[i] == 0) {
            min_prime[i] = i;
            primes.pb(i);
            mu[i] = -1;
            omega[i] = 1;
            core[i] = i;
        }
        for (int p : primes) {
            if (p > min_prime[i] || (ll)i * p >= MAXN) break;
            int m = i * p;
            min_prime[m] = p;
            if (min_prime[i] == p) {
                mu[m] = 0;
                omega[m] = omega[i];
                core[m] = core[i];
            } else {
                mu[m] = -mu[i];
                omega[m] = omega[i] + 1;
                core[m] = core[i] * p;
            }
        }
    }
}

// Modular Exponentiation
ll power(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

void solve() {
    int n;
    ll k;
    if(!(cin >> n >> k)) return;

    int max_val = n;

    // Reset relevant parts of global arrays
    // Note: We only iterate up to max_val, so cleaning 0..max_val is sufficient
    for(int i = 0; i <= max_val; ++i) freqs[i] = 0;
    for(int w = 0; w < 7; ++w) {
        for(int i = 0; i <= max_val; ++i) DSum[w][i] = 0;
    }
    for(int i = 0; i <= max_val; ++i) {
        for(int s = 0; s < 13; ++s) Coeff[i][s] = 0;
    }

    // Precompute powers for (S - w(g))^k
    for(int i = 0; i <= 13; ++i) POW[i] = power(i, k);

    // Read input
    forn(i, n) {
        int val;
        in(val);
        freqs[core[val]]++;
    }

    // Compute DSum: sum of frequencies of multiples
    // DSum[w][d] count of numbers with omega value w divisible by d
    for(int i = 1; i <= max_val; ++i) {
        for (int j = i; j <= max_val; j += i) {
            if (freqs[j] > 0) {
                int w = omega[j];
                DSum[w][i] += freqs[j];
                if (DSum[w][i] >= MOD) DSum[w][i] -= MOD;
            }
        }
    }

    // Compute Coeff matrix using Dirichlet logic
    for(int g = 1; g <= max_val; ++g) {
        int w_g = omega[g];
        // Iterate multiples T of g
        for (int m = 1; g * m <= max_val; ++m) {
            if (mu[m] == 0) continue;
            int T = g * m;
            int term_sign = mu[m]; // 1 or -1
            
            // Calculate contributions for all possible sums S = wa + wb
            for(int s = 0; s <= 12; ++s) {
                // We sum terms (S - omega(g))^k
                int base = s - w_g;
                ll val;
                // If base < 0, it means S < omega(g). In valid pairs, S >= omega(g) holds.
                // Algebraically treating as 0 fits the exclusion logic for impossible states.
                if (base < 0) val = 0;
                else val = POW[base];

                if (term_sign == 1) {
                    Coeff[T][s] = (Coeff[T][s] + val) % MOD;
                } else {
                    Coeff[T][s] = (Coeff[T][s] - val) % MOD;
                    if (Coeff[T][s] < 0) Coeff[T][s] += MOD;
                }
            }
        }
    }

    ll total_ans = 0;
    
    // Iterate over all pairs of omega counts (wa, wb)
    for(int wa = 0; wa <= 6; ++wa) {
        for(int wb = wa; wb <= 6; ++wb) {
            int S = wa + wb;
            ll pair_sum = 0;
            
            // Sum over GCD multiples T
            for(int T = 1; T <= max_val; ++T) {
                // Count pairs (A, B) such that T|A and T|B
                ll prod = (ll)DSum[wa][T] * DSum[wb][T] % MOD;
                if (prod == 0) continue;
                
                // Add weighted by the precomputed inclusion-exclusion coefficient
                pair_sum = (pair_sum + prod * Coeff[T][S]) % MOD;
            }

            if (wa != wb) {
                total_ans = (total_ans + 2 * pair_sum) % MOD;
            } else {
                total_ans = (total_ans + pair_sum) % MOD;
            }
        }
    }

    // Remove diagonal terms (i == j)
    ll diag_sum = 0;
    for(int i = 1; i <= max_val; ++i) {
        if (freqs[i] > 0) {
            // Term is w(a_i)^k
            ll term = POW[omega[i]];
            ll contrib = (term * freqs[i]) % MOD;
            diag_sum = (diag_sum + contrib) % MOD;
        }
    }

    // Final result is (Total - Diagonal) / 2
    ll res = (total_ans - diag_sum + MOD) % MOD;
    ll inv2 = (MOD + 1) / 2;
    res = (res * inv2) % MOD;

    out(res);
}

int main(){
    cin.tie(0)->sync_with_stdio(0);
    init();
    int t = 1;
    cin >> t;
    while(t--) solve();
}
