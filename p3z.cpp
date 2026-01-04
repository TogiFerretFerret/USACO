#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int N;
	cin >> N;
	vector<int> X(N);
	for (int &x : X) cin >> x;
	sort(begin(X), end(X));
	int Q;
	cin >> Q;
	vector<int64_t> cum{0};
	for (int x : X) cum.push_back(cum.back() + x);
	auto cumsum = [&](int l, int r) { return cum.at(r + 1) - cum.at(l); };
	vector<int> distinct_X{0};  // keep only distinct X[i]
	for (int i = 1; i < N; ++i)
		if (X[i] > X[i - 1]) distinct_X.push_back(i);
	while (Q--) {
		int A, B;
		cin >> A >> B;
		auto eval = [&](int idx) {
			int i = distinct_X.at(idx);
			int64_t ans = B * (cumsum(i, N - 1) - (int64_t)(N - i) * X[i]);
			ans += A * ((int64_t)i * X[i] - cumsum(0, i - 1));
			return ans;
		};
		int lo = 0, hi = size(distinct_X) - 1;
		while (lo < hi) {
			int mid = (lo + hi) / 2;
			if (eval(mid) < eval(mid + 1)) hi = mid;
			else lo = mid + 1;
		}
		cout << eval(lo) << "\n";
	}
}
