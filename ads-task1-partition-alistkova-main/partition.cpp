#include <iostream>
#include <cstring>

long long partition(int n, int k);

int main () {
	int n = 0;
	int k = 0;
	std::cin >> n >> k;
	std::cout << partition(n, k);
	return 0;
}

long long partition(int n, int k){
  if (k > n) {
    return 0;
  }
  if (k == n) {
    return 1;
  }

  long long curr[501]{0}; // For current k
  long long prev[501]{0}; // For previous k - 1
  for (int curr_k = 1; curr_k <= k; ++curr_k) {
    std::swap(curr, prev);
    memset(curr, 0, (n + 1) * sizeof(long long));

    for (int curr_n = 1; curr_n <= n; ++curr_n) {
      if (curr_k == curr_n || curr_k == 1) {
        curr[curr_n] = 1;
      } else if (curr_k > curr_n || curr_n == 0) {
        curr[curr_n] = 0;
      } else {
        curr[curr_n] = prev[curr_n - 1] + curr[curr_n - curr_k];
      }
    }
  }

  return curr[n];
}
