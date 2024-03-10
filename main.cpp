#include <bits/stdc++.h>

using namespace std;
int n, m;
int main()
{
    freopen("./data/musk.csv", "r", stdin);
    freopen("./data/musk2.csv", "w", stdout);
    cin >> n >> m;
    cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; i++) {
        string s, t;
        cin >> s >> t;
        int x;
        for (int j = 1; j <= m + 1; j++) {
            cin >> x;
            cout << x << ' ';
        }
        cout << '\n';
    }
}
