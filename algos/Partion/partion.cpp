#include <iostream>

long long partition(int n, int k);

int main () {
	int n = 0;
	int k = 0;
	std::cin >> n >> k;
	std::cout << partition(n, k);
	return 0;
}

long long partition(int n, int k) {
    long long* dp = new long long[n + 1]{};
    dp[0] = 1;
    for (int i = 1; i <= k; ++i) {
        for (int j = i; j <= n; ++j) {
            dp[j] += dp[j - i];
        }
    }
    long long num = dp[n - k];
    delete[] dp;
    return num;
}
