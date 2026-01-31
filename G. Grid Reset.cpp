// Problem: G. Grid Reset
// Judge: Codeforces
// URL: https://codeforces.com/gym/668106/problem/G
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

// Global timer to control execution time across all test cases
static auto start_time = chrono::steady_clock::now();

void solve(){
    int n, m, k, q;
    in(n, m, k, q);
    string s;
    in(s);

    using RowBits = bitset<105>;
    
    // Precompute bitmasks for efficient validity checks
    // k_masks[shift] corresponds to k set bits shifted by 'shift'
    vector<RowBits> k_masks(105);
    RowBits base_mask;
    forn(i, k) base_mask[i] = 1;
    forn(i, 101) k_masks[i] = (base_mask << i);

    vector<pair<int, int>> ans;
    ans.reserve(q);

    // Grid state and buffers
    vector<RowBits> rows(n), cols(m);
    vector<int> r_cnt(n), c_cnt(m);
    vector<ll> orth_gain; 
    orth_gain.reserve(105);

    struct Cand { int r, c; };
    vector<Cand> best_cands;
    best_cands.reserve(n * m);

    // Randomized Greedy with Restarts
    while(true){
        // Check global time limit (1.9s)
        auto curr = chrono::steady_clock::now();
        if(chrono::duration_cast<chrono::milliseconds>(curr - start_time).count() > 1900) break;

        // Reset State
        forn(i, n) { rows[i].reset(); r_cnt[i] = 0; }
        forn(i, m) { cols[i].reset(); c_cnt[i] = 0; }
        ans.clear();
        bool possible = true;

        forn(op_idx, q){
            char type = s[op_idx];
            best_cands.clear();
            ll max_score = -2e18; // Init with very low value

            if(type == 'H'){
                // Optimization: Precompute column gains using prefix sums
                // Gain = (new_count^2 - old_count^2) = 2*count + 1
                orth_gain.assign(m + 1, 0); 
                forn(j, m) {
                    ll val = 2 * c_cnt[j] + 1;
                    if(c_cnt[j] + 1 == n) val += 1000000; // Bonus for completing a column
                    orth_gain[j+1] = orth_gain[j] + val;
                }

                forn(r, n){
                    if(r_cnt[r] > m - k) continue; // Row too full
                    
                    // Base score from row contribution
                    ll row_score = 2LL * r_cnt[r] * k + (ll)k*k; 
                    if(r_cnt[r] + k == m) row_score += 1000000; // Bonus for completing a row

                    // Check all valid column positions
                    for(int c = 0; c <= m - k; ++c){
                        if((rows[r] & k_masks[c]).none()){
                            // Total Score = Row Score + Sum of Column Gains in range [c, c+k)
                            ll score = row_score + (orth_gain[c+k] - orth_gain[c]);
                            
                            if(score > max_score){
                                max_score = score;
                                best_cands.clear();
                                best_cands.pb({r, c});
                            } else if(score == max_score){
                                best_cands.pb({r, c});
                            }
                        }
                    }
                }
            } else { // Vertical Operation
                // Optimization: Precompute row gains
                orth_gain.assign(n + 1, 0);
                forn(i, n) {
                    ll val = 2 * r_cnt[i] + 1;
                    if(r_cnt[i] + 1 == m) val += 1000000;
                    orth_gain[i+1] = orth_gain[i] + val;
                }

                forn(c, m){
                    if(c_cnt[c] > n - k) continue;

                    ll col_score = 2LL * c_cnt[c] * k + (ll)k*k;
                    if(c_cnt[c] + k == n) col_score += 1000000;

                    for(int r = 0; r <= n - k; ++r){
                        if((cols[c] & k_masks[r]).none()){
                            ll score = col_score + (orth_gain[r+k] - orth_gain[r]);
                            
                            if(score > max_score){
                                max_score = score;
                                best_cands.clear();
                                best_cands.pb({r, c});
                            } else if(score == max_score){
                                best_cands.pb({r, c});
                            }
                        }
                    }
                }
            }

            if(best_cands.empty()){
                possible = false;
                break;
            }

            // Randomly select one of the best candidates
            int sel = 0;
            if(best_cands.size() > 1){
                 sel = PIDRNG() % best_cands.size();
            }
            auto [br, bc] = best_cands[sel];
            ans.pb({br + 1, bc + 1}); // Output is 1-based

            // Apply Move
            if(type == 'H'){
                r_cnt[br] += k;
                forn(i, k){
                    rows[br][bc+i] = 1;
                    cols[bc+i][br] = 1;
                    c_cnt[bc+i]++;
                }
            } else {
                c_cnt[bc] += k;
                forn(i, k){
                    rows[br+i][bc] = 1;
                    cols[bc][br+i] = 1;
                    r_cnt[br+i]++;
                }
            }

            // Handle Resets
            static vector<int> full_rows; full_rows.clear();
            static vector<int> full_cols; full_cols.clear();
            
            forn(i, n) if(r_cnt[i] == m) full_rows.pb(i);
            forn(j, m) if(c_cnt[j] == n) full_cols.pb(j);

            if(!full_rows.empty() || !full_cols.empty()){
                RowBits mask_fc;
                for(int c : full_cols) mask_fc[c] = 1;
                RowBits mask_fr;
                for(int r : full_rows) mask_fr[r] = 1;
                
                // Simultaneous reset logic
                forn(i, n){
                    if(mask_fr[i]) rows[i].reset();
                    else rows[i] &= ~mask_fc; // Clear bits intersecting full columns
                }
                forn(j, m){
                    if(mask_fc[j]) cols[j].reset();
                    else cols[j] &= ~mask_fr; // Clear bits intersecting full rows
                }
                
                // Recompute counts after reset
                forn(i, n) r_cnt[i] = (int)rows[i].count();
                forn(j, m) c_cnt[j] = (int)cols[j].count();
            }
        }

        if(possible){
            for(auto& p : ans) out(p);
            return;
        }
    }

    // If time limit exceeded or no solution found after restarts
    out(-1);
}
int main(){
    if(!INTERACTIVE)cin.tie(0)->sync_with_stdio(0);
    #ifndef LOCAL_JUDGE
    #if FILEMODE
    freopen(FILENAME".in","r",stdin);
    freopen(FILENAME".out","w",stdout);
    #endif
    #endif
    int t=1;
    if (MULTITEST) cin>>t;
    forn(i,t)solve();
}
