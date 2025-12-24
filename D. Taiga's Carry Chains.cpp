/*

*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
void solve(){
    ll n, k;
    cin >> n >> k;
    if(k >= 32){
        cout << __builtin_popcountll(n) + k - 1 << "\n";
        return;
    }
    int m = k;
    int dp[65][35][2];
    for(int i = 0; i <= 64; i++){
        for(int j = 0; j <= m; j++){
            dp[i][j][0] = dp[i][j][1] = 1e9;
        }
    }
    dp[0][0][0] = 0;
    for(int i = 0; i < 64; i++){
        int bit = (n >> i) & 1;
        for(int j = 0; j <= m; j++){
            for(int c = 0; c < 2; c++){
                if(dp[i][j][c] > 1e8) continue;
                // Option 1: Do not select this bit
                int s = bit + c;
                dp[i+1][j][s/2] = min(dp[i+1][j][s/2], dp[i][j][c] + s%2);
                // Option 2: Select this bit
                if(j < m){
                    s = bit + c + 1;
                    dp[i+1][j+1][s/2] = min(dp[i+1][j+1][s/2], dp[i][j][c] + s%2);
                }
            }
        }
    }
    int min_pop = min(dp[64][m][0], dp[64][m][1] + 1);
    cout << __builtin_popcountll(n) + k - min_pop << "\n";
}
int main(){cin.tie(0)->sync_with_stdio(0);int t;cin>>t;while(t--)solve();}

