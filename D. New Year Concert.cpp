#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int my_gcd(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

void solve(){
    int n;
    if (!(cin >> n)) return;
    vector<int> a(n);
    for(int& x : a) cin >> x;

    int ans = 0;
    // Stores pairs of {gcd_value, start_index}
    vector<pair<int, int>> cands;

    for (int i = 0; i < n; ++i) {
        for (auto& p : cands) {
            p.first = my_gcd(p.first, a[i]);
        }
        cands.push_back({a[i], i});

        vector<pair<int, int>> next_cands;
        for (auto& p : cands) {
            if (next_cands.empty() || next_cands.back().first != p.first) {
                next_cands.push_back(p);
            }
        }
        cands = next_cands;

        bool bad_found = false;
        for (size_t j = 0; j < cands.size(); ++j) {
            int g = cands[j].first;
            int l_start = cands[j].second;
            int l_end = (j + 1 < cands.size()) ? cands[j+1].second - 1 : i;

            int target_l = i + 1 - g;
            if (target_l >= l_start && target_l <= l_end) {
                bad_found = true;
                break;
            }
        }

        if (bad_found) {
            ans++;
            cands.clear();
        }

        cout << ans << (i == n - 1 ? "" : " ");
    }
    cout << "\n";
}

int main(){
    cin.tie(0)->sync_with_stdio(0);
    solve();
    return 0;
}
