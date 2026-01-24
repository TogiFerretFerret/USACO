// Problem: G. Xor-MST
// Judge: Codeforces
// URL: https://codeforces.com/contest/888/problem/G
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
#if INTERACTIVE
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << endl; }//softmod for interactive
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
#else
m1(out) { cout << forward<T>(a);  m2(cout << " " <<); cout << "\n"; }//softmod for interactive
m1(debug) { cerr << forward<T>(a);  m2(cerr << " " <<); cerr << "\n"; }
m1(in) { cin >> forward<T>(a); m2(cin >>); }
#endif
#define MULTITEST false
#define pb push_back
// We have two approaches:
//// 1. Prim's Algorithm with a trie + Meet in the Middle (recursive)
//// 2. Modified Boruvka's Algorithm with a trie <- better approach
void solve(){
	int n; in(n);
	vector<int> a(n); in(a);
	if(n <= 1){ out(0); return; }
	const int B = 30; // bit depth for ai <= 1e9

	// Trie storage: children, count, and leaf id lists
	vector<array<int,2>> ch; ch.reserve(n*(B+1));
	vector<int> cnt; cnt.reserve(n*(B+1));
	vector<vector<int>> leaf_ids; leaf_ids.reserve(n*(B+1));
	function<int()> new_node = [&]()->int {
		ch.push_back({-1,-1});
		cnt.push_back(0);
		leaf_ids.emplace_back();
		return (int)ch.size() - 1;
	};
	int root = new_node();

	vector<int> posInLeaf(n, -1), whereLeaf(n, -1);
	function<void(int,int)> trie_insert = [&](int val, int id)->void {
		int cur = root; cnt[cur]++;
		f0rn(b, B-1, -1){
			int bit = (val >> b) & 1;
			if(ch[cur][bit] == -1) ch[cur][bit] = new_node();
			cur = ch[cur][bit]; cnt[cur]++;
		}
		whereLeaf[id] = cur;
		posInLeaf[id] = (int)leaf_ids[cur].size();
		leaf_ids[cur].push_back(id);
	};
	function<void(int,int)> trie_erase = [&](int val, int id)->void {
		int cur = root; cnt[cur]--;
		f0rn(b, B-1, -1){
			int bit = (val >> b) & 1;
			int nxt = ch[cur][bit];
			cur = nxt; cnt[cur]--;
		}
		int leaf = cur;
		int pos = posInLeaf[id];
		int last = leaf_ids[leaf].back();
		leaf_ids[leaf][pos] = last;
		posInLeaf[last] = pos;
		leaf_ids[leaf].pop_back();
		posInLeaf[id] = -1;
		whereLeaf[id] = -1;
	};
	function<int(int)> trie_query = [&](int val)->int {
		if(cnt[root] == 0) return -1;
		int cur = root;
		f0rn(b, B-1, -1){
			int bit = (val >> b) & 1;
			int prefer = ch[cur][bit];
			if(prefer != -1 && cnt[prefer] > 0) cur = prefer;
			else cur = ch[cur][1-bit];
		}
		if(leaf_ids[cur].empty()) return -1;
		return leaf_ids[cur].back();
	};
	function<bool()> trie_empty = [&]()->bool { return cnt[root] == 0; };

	// DSU with members list per root
	vector<int> parent(n);
	vector<vector<int>> members(n);
	forn(i,n){ parent[i] = i; members[i].push_back(i); }
	function<int(int)> find = [&](int x)->int { return parent[x] == x ? x : parent[x] = find(parent[x]); };
	function<bool(int,int)> unite = [&](int x, int y)->bool {
		x = find(x); y = find(y); if(x == y) return false;
		// union by size (members size)
		if(members[x].size() < members[y].size()) swap(x,y);
		parent[y] = x;
		forn(ii, (int)members[y].size()) members[x].push_back(members[y][ii]);
		members[y].clear();
		return true;
	};

	// build initial trie
	forn(i,n) trie_insert(a[i], i);

	ll ans = 0;
	int components = n;
	while(components > 1){
		vector<int> roots;
		forn(i,n) if(find(i) == i) roots.push_back(i);
		vector<ll> bestCost(n, (1LL<<60));
		vector<int> bestU(n, -1), bestV(n, -1);

		forn(ri, (int)roots.size()){
			int r = roots[ri];
			// remove all members of component r
			forn(ii, (int)members[r].size()){
				int v = members[r][ii]; trie_erase(a[v], v);
			}
			// query best outgoing edge
			forn(ii, (int)members[r].size()){
				int u = members[r][ii];
				if(trie_empty()) break;
				int id = trie_query(a[u]);
				if(id == -1) continue;
				ll cost = (ll)(a[u] ^ a[id]);
				if(cost < bestCost[r]){ bestCost[r] = cost; bestU[r] = u; bestV[r] = id; }
			}
			// reinsert members
			forn(ii, (int)members[r].size()){
				int v = members[r][ii]; trie_insert(a[v], v);
			}
		}

		bool any_merge = false;
		forn(ri, (int)roots.size()){
			int r = roots[ri];
			if(bestU[r] != -1){
				if(unite(bestU[r], bestV[r])){ ans += bestCost[r]; components--; any_merge = true; }
			}
		}
		if(!any_merge) break; // safety: shouldn't happen
	}
	out(ans);
}
int main(){
	if(!INTERACTIVE)cin.tie(0)->sync_with_stdio(0);
	int t=1;
	if (MULTITEST) cin>>t;
	forn(i,t)solve();
}


