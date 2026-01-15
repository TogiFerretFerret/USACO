#include <bits/stdc++.h>
 
using namespace std;
 
const int MAXN = 2e5 + 7;

int a[MAXN], st[4 * MAXN];

void build(int v, int l, int r) {
    if (l + 1 == r) {
        st[v] = a[l];
        return;
    }
    int m = (r + l) / 2;
    build(v * 2 + 1, l, m);
    build(v * 2 + 2, m, r);
    st[v] = min(st[v * 2 + 1], st[v * 2 + 2]);
}

void upd(int v, int l, int r, int i, int x) {
    if (i < l || r <= i) return;
    if (l + 1 == r) {
        a[i] = x;
        st[v] = x;
        return;
    }
    int m = (r + l) / 2;
    upd(v * 2 + 1, l, m, i, x);
    upd(v * 2 + 2, m, r, i, x);
    st[v] = min(st[v * 2 + 1], st[v * 2 + 2]);
}

int getMin(int v, int l, int r, int lq, int rq) {
    if (rq <= l || r <= lq) return (int) 1e6;
    if (lq <= l && r <= rq) return st[v];
    int m = (r + l) / 2;
    return min(getMin(v * 2 + 1, l, m, lq, rq), getMin(v * 2 + 2, m, r, lq, rq));
}

int ans, currMn;

void findInter(int v, int l, int r, int lq, int rq) {
    if (rq <= l || r <= lq || ans != (int) 1e6) return;
    if (lq <= l && r <= rq && min(currMn, st[v]) > r - lq - 1) {
        currMn = min(currMn, st[v]);
        return;
    }
    if (l + 1 == r) {
        ans = l;
        return;
    }
    int m = (r + l) / 2;
    findInter(v * 2 + 1, l, m, lq, rq);
    findInter(v * 2 + 2, m, r, lq, rq);
}


int main() {
    int t;
    cin >> t;

    while (t--) {
        int n, q;
        cin >> n >> q;
        for (int i = 0; i < n; ++i) cin >> a[i];

        build(0, 0, n);
        while (q--) {
            int idx;
            cin >> idx;

            if (idx == 1) {
                int i, x;
                cin >> i >> x;
                upd(0, 0, n, i - 1, x);
            } else {
                int l, r;
                cin >> l >> r;
                l--;

                if (getMin(0, 0, n, l, r) > r - l) {
                    cout << "0\n";
                    continue;
                }

                ans = currMn = (int) 1e6;
                findInter(0, 0, n, l, r);
                cout << (ans != (int) 1e6 && getMin(0, 0, n, l, ans + 1) == ans - l) << "\n";
            }
        }
    }

    return 0;
}
