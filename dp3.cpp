#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;
const ll INV2 = 500000004; // Modular inverse of 2
const ll INV6 = 166666668; // Modular inverse of 6

// --- Trie Memory Pool ---
// Max 2N nodes * 30 bits -> ~1.2M. Use 2M for safety.
const int MAX_NODES = 2000000;
int ch[MAX_NODES][2];
ll sz[MAX_NODES]; // Count of items in subtree
bool is_full[MAX_NODES]; // Optimization: node represents complete binary interval
int node_ptr = 1;

int new_node() {
    int u = node_ptr++;
    ch[u][0] = ch[u][1] = 0;
    sz[u] = 0;
    is_full[u] = false;
    return u;
}

ll K;

// Combinatorics
inline ll nC3(ll n) {
    if (n < 3) return 0;
    ll ret = n % MOD;
    ret = ret * ((n - 1) % MOD) % MOD;
    ret = ret * ((n - 2) % MOD) % MOD;
    return ret * INV6 % MOD;
}
inline ll nC2(ll n) {
    if (n < 2) return 0;
    ll ret = n % MOD;
    ret = ret * ((n - 1) % MOD) % MOD;
    return ret * INV2 % MOD;
}

// Helpers
inline ll get_sz(int u, int bit) {
    if (u == -1) { 
        ll v = (1LL << (bit + 1)); 
        return (bit >= 30) ? (v % MOD) : v;
    }
    return u ? sz[u] : 0;
}

void insert(int u, ll L, ll R, ll qL, ll qR, int bit) {
    ll overlapL = max(L, qL);
    ll overlapR = min(R, qR);
    if (overlapL > overlapR) return;
    if (overlapL == L && overlapR == R) {
        is_full[u] = true;
        sz[u] = (R - L + 1) % MOD;
        return;
    }
    if (bit < 0) return;
    ll mid = L + (1LL << bit) - 1;
    if (qL <= mid) {
        if (!ch[u][0]) ch[u][0] = new_node();
        insert(ch[u][0], L, mid, qL, qR, bit - 1);
    }
    if (qR > mid) {
        if (!ch[u][1]) ch[u][1] = new_node();
        insert(ch[u][1], mid + 1, R, qL, qR, bit - 1);
    }
    sz[u] = (get_sz(ch[u][0], bit-1) + get_sz(ch[u][1], bit-1)) % MOD;
}

// --- Logic ---

struct Stats {
    ll sum_deg;
    ll sum_sq_deg;
};

// Hash table for Stats get_stats(bit, u, v)
// u = Source set, v = Target set. Calculating sum of degrees in v for nodes in u.
const int HSIZE = 3000019;
struct Entry { int bit, u, v; Stats val; bool active; } memo[HSIZE];

inline int hash_fn(int bit, int u, int v) {
    size_t k = (size_t)u * 31337 + v;
    k = k * 31 + bit;
    return k % HSIZE;
}

Stats get_stats(int bit, int u, int v) {
    if (u == 0 || v == 0) return {0, 0};
    
    // Virtual full nodes handled by specific ID check logic
    // u=-1 or v=-1 represents full subtrees.
    // Logic below handles is_full via node flags or manual index -1 checks.
    
    // Optimization: If v is full, deg(x) is constant for all x in u
    // Const deg = number of elements in v satisfying condition with x.
    // If v is Full, it covers [0..2^{bit+1}-1].
    // Condition x ^ y <= K means y must match x for bits > limit? No.
    // If v is Full, then for ANY x, the count of valid neighbors in v is just
    // "Count of numbers y < 2^{bit+1} such that y <= K' (relative)". 
    // This assumes matching prefixes, but x and y here are diffed relative to this tree level.
    // The query implies prefixes matched down to here (relative to original XOR split).
    // Basically: y <= K mod (2^{bit+1}). The range 0..Max is full.
    // So Count = (K & ((1<<bit+1)-1)) + 1.
    bool u_full = (u == -1 || is_full[u]);
    bool v_full = (v == -1 || is_full[v]);

    if (v_full) {
        ll m = (1LL << (bit + 1)) - 1;
        ll count_per_x = ((K & m) + 1) % MOD;
        ll size_u = get_sz(u, bit);
        
        Stats ret;
        ret.sum_deg = (size_u * count_per_x) % MOD;
        ret.sum_sq_deg = (ret.sum_deg * count_per_x) % MOD;
        return ret;
    }
    
    // Base case: Leaf
    if (bit < 0) {
        // bit -1 means leaf level. ranges [x, x]. diff 0 <= K always true.
        ll su = get_sz(u, -1);
        ll sv = get_sz(v, -1);
        // deg is sv. Sum over u: su * sv. Sq: su * sv^2.
        return { su * sv % MOD, su * sv % MOD * sv % MOD };
    }
    
    // Memo check (only if not doing fast logic)
    int h = hash_fn(bit, u, v);
    while (memo[h].active) {
        if (memo[h].bit == bit && memo[h].u == u && memo[h].v == v) return memo[h].val;
        if (++h == HSIZE) h = 0;
    }
    
    int kb = (K >> bit) & 1;
    Stats res = {0, 0};
    
    // Children. -1 implies child is also virtual full.
    int u0 = u_full ? -1 : ch[u][0];
    int u1 = u_full ? -1 : ch[u][1];
    int v0 = v_full ? -1 : ch[v][0];
    int v1 = v_full ? -1 : ch[v][1];

    if (kb == 0) {
        // Can only match 0-0 and 1-1. Strict subset.
        Stats s0 = get_stats(bit - 1, u0, v0);
        Stats s1 = get_stats(bit - 1, u1, v1);
        res.sum_deg = (s0.sum_deg + s1.sum_deg) % MOD;
        res.sum_sq_deg = (s0.sum_sq_deg + s1.sum_sq_deg) % MOD;
    } else {
        // kb=1. Match 0-0 (ALL), 1-1 (ALL), 0-1 (Cond), 1-0 (Cond).
        
        // 1. x in u0. Can match all y in v0. And cond y in v1.
        //    deg(x) = |v0| + partial(x, v1).
        // 2. x in u1. Can match all y in v1. And cond y in v0.
        //    deg(x) = |v1| + partial(x, v0).
        
        // Get Sub-stats
        // For u0 -> v1
        Stats st01 = get_stats(bit - 1, u0, v1);
        // For u1 -> v0
        Stats st10 = get_stats(bit - 1, u1, v0);
        
        ll sv0 = get_sz(v0, bit - 1);
        ll sv1 = get_sz(v1, bit - 1);
        ll su0 = get_sz(u0, bit - 1);
        ll su1 = get_sz(u1, bit - 1);
        
        // Accumulate u0 side:
        // Sum = sum(|v0| + d) = |u0||v0| + Sum(d)
        ll term1_sum = (su0 * sv0 + st01.sum_deg) % MOD;
        // Sq = sum((|v0| + d)^2) = |u0||v0|^2 + 2|v0|Sum(d) + Sum(d^2)
        ll term1_sq = (su0 * sv0 % MOD * sv0) % MOD; // constant part
        term1_sq = (term1_sq + 2 * sv0 % MOD * st01.sum_deg) % MOD;
        term1_sq = (term1_sq + st01.sum_sq_deg) % MOD;
        
        // Accumulate u1 side:
        ll term2_sum = (su1 * sv1 + st10.sum_deg) % MOD;
        ll term2_sq = (su1 * sv1 % MOD * sv1) % MOD;
        term2_sq = (term2_sq + 2 * sv1 % MOD * st10.sum_deg) % MOD;
        term2_sq = (term2_sq + st10.sum_sq_deg) % MOD;
        
        res.sum_deg = (term1_sum + term2_sum) % MOD;
        res.sum_sq_deg = (term1_sq + term2_sq) % MOD;
    }
    
    // Store memo
    h = hash_fn(bit, u, v); // reset h
    while (memo[h].active) {
        if (memo[h].bit == bit && memo[h].u == u && memo[h].v == v) break;
        if (++h == HSIZE) h = 0;
    }
    memo[h] = {bit, u, v, res, true};
    
    return res;
}

ll solve(int bit, int u) {
    if (u == 0) return 0;
    if (bit < 0) return nC3(sz[u]);
    
    // Check if full node logic needed? Not really, root recursion drives flow.
    // If clique formed (kb=1), then logic stops descending.
    
    int kb = (K >> bit) & 1;
    bool fu = (u == -1 || is_full[u]);
    int u0 = fu ? -1 : ch[u][0];
    int u1 = fu ? -1 : ch[u][1];
    
    if (kb == 0) {
        // Split, no cross interaction allowed at all.
        // Even the clique property doesn't apply across boundaries.
        return (solve(bit - 1, u0) + solve(bit - 1, u1)) % MOD;
    } else {
        // S0 and S1 are effectively cliques globally
        // Ans = Local(S0) + Local(S1) + Cross terms.
        ll ans = (nC3(get_sz(u0, bit - 1)) + nC3(get_sz(u1, bit - 1))) % MOD;
        
        // Cross: 2 from S0, 1 from S1
        Stats st01 = get_stats(bit - 1, u0, u1); 
        // term = Sum(nC2(deg)) = (Sum(d^2) - Sum(d))/2
        ll val = (st01.sum_sq_deg - st01.sum_deg + MOD) % MOD * INV2 % MOD;
        ans = (ans + val) % MOD;
        
        // Cross: 2 from S1, 1 from S0
        Stats st10 = get_stats(bit - 1, u1, u0);
        val = (st10.sum_sq_deg - st10.sum_deg + MOD) % MOD * INV2 % MOD;
        ans = (ans + val) % MOD;
        
        return ans;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N >> K)) return 0;

    struct Range { ll l, r; };
    vector<Range> in(N);
    ll max_limit = K;
    for(int i=0; i<N; i++) {
        cin >> in[i].l >> in[i].r;
        max_limit = max(max_limit, in[i].r);
    }
    
    int sbit = 0;
    while ((1LL << (sbit+1)) <= max_limit) sbit++;
    // Must handle bits up to ~29/30 as typical int max
    if (sbit < 29 && (max_limit > (1LL<<sbit))) sbit = 29;
    
    int root = new_node();
    ll end_cov = (1LL << (sbit + 1)) - 1;
    for (auto &r : in) insert(root, 0, end_cov, r.l, r.r, sbit);
    
    cout << solve(sbit, root) << endl;

    return 0;
}
