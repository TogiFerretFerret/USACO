// Problem: D. Swap Pass
// Judge: Codeforces
// URL: https://codeforces.com/contest/1508/problem/D
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
	int n;
    in(n);

    struct Point {
        int id;           
        ll x, y;
        int current_label;
        bool removed;
    };

    vector<Point> points(n);
    vector<int> label_pos(n + 1);
    vector<int> active_indices;

    forn(i, n) {
        points[i].removed = false;
        points[i].id = i + 1;
        in(points[i].x, points[i].y, points[i].current_label);
        label_pos[points[i].current_label] = i; 
        active_indices.pb(i);
    }

    set<pair<int, int>> used_edges;

    auto is_used = [&](int u, int v) -> bool {
        if (u > v) swap(u, v);
        return used_edges.count({u, v});
    };
    auto mark_used = [&](int u, int v) -> void {
        if (u > v) swap(u, v);
        used_edges.insert({u, v});
    };

    auto cross_product = [&](int o, int a, int b) -> ll {
        return (points[a].x - points[o].x) * (points[b].y - points[o].y) - 
               (points[a].y - points[o].y) * (points[b].x - points[o].x);
    };

    struct DSU {
        vector<int> p;
        DSU(int n) {
            p.resize(n);
            iota(all(p), 0);
        }
        int find(int i) {
            if (p[i] == i) return i;
            return p[i] = find(p[i]);
        }
        void unite(int i, int j) {
            int root_i = find(i);
            int root_j = find(j);
            if (root_i != root_j) p[root_i] = root_j;
        }
    };

    vector<pair<int, int>> output_ops;

    while (!active_indices.empty()) {
        int best_idx = -1;
        for (int i : active_indices) {
            if (best_idx == -1) best_idx = i;
            else {
                if (points[i].x < points[best_idx].x || 
                   (points[i].x == points[best_idx].x && points[i].y < points[best_idx].y)) {
                    best_idx = i;
                }
            }
        }
        int o_idx = best_idx;

        if (points[o_idx].current_label == points[o_idx].id) {
            points[o_idx].removed = true;
            vector<int> next_active;
            for(int i : active_indices) if(!points[i].removed) next_active.pb(i);
            active_indices = next_active;
            continue;
        }

        vector<int> others;
        for(int i : active_indices) {
            if(i != o_idx) others.pb(i);
        }

        sort(all(others), [&](int a, int b){
            ll cp = cross_product(o_idx, a, b);
            if (cp != 0) return cp > 0;
            ll da = (points[a].x - points[o_idx].x)*(points[a].x - points[o_idx].x) + (points[a].y - points[o_idx].y)*(points[a].y - points[o_idx].y);
            ll db = (points[b].x - points[o_idx].x)*(points[b].x - points[o_idx].x) + (points[b].y - points[o_idx].y)*(points[b].y - points[o_idx].y);
            return da < db;
        });

        DSU dsu(n);
        vector<int> id_to_index(n + 1);
        for(int idx : active_indices) id_to_index[points[idx].id] = idx;

        for (int i : active_indices) {
            int val_held = points[i].current_label;
            if(val_held > 0 && val_held <= n) {
                int owner_idx = id_to_index[val_held]; 
                dsu.unite(i, owner_idx);
            }
        }

        for (size_t k = 0; k + 1 < others.size(); ++k) {
            int u = others[k];
            int v = others[k+1];
            if (dsu.find(u) != dsu.find(v)) {
                if (!is_used(points[u].id, points[v].id)) {
                    mark_used(points[u].id, points[v].id);
                    output_ops.pb({points[u].id, points[v].id});
                    
                    int lu = points[u].current_label;
                    int lv = points[v].current_label;
                    swap(points[u].current_label, points[v].current_label);
                    label_pos[lu] = v;
                    label_pos[lv] = u;
                    
                    dsu.unite(u, v);
                }
            }
        }

        int needed = points[o_idx].id;
        int holder = label_pos[needed];

        while (holder != o_idx) {
            if (is_used(points[o_idx].id, points[holder].id)) break;
            
            mark_used(points[o_idx].id, points[holder].id);
            output_ops.pb({points[o_idx].id, points[holder].id});

            int lo = points[o_idx].current_label; 
            int lh = points[holder].current_label; 
            swap(points[o_idx].current_label, points[holder].current_label);
            label_pos[lo] = holder;
            label_pos[lh] = o_idx;

            int val_at_o = points[o_idx].current_label;
            if (val_at_o == points[o_idx].id) break;

            int target_node = id_to_index[val_at_o];
            holder = target_node;
        }

        points[o_idx].removed = true;
        
        vector<int> next_active;
        for(int i : active_indices) if(!points[i].removed) next_active.pb(i);
        active_indices = next_active;
    }

    out(output_ops.size());
    for(auto& p : output_ops) out(p.first, p.second);
}
int main(){
	cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}
