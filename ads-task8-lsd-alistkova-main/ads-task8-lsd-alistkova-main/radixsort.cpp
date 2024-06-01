#include <fstream>
#include <iostream>

void radixsort(char** arr, int r, int c, int ph) {
  char** res = new char*[r + 1];

  for (int i = c - 1; i >= c - ph; i--) {
    int A[256];
    for (int i = 0; i < 256; i++) {
      A[i] = 0;
    }

    for (int j = 0; j < r; j++) {
      A[(int)arr[j][i]]++;
    }

    int score = 0;
    for (int j = 0; j < 256; j++) {
      int tmp = A[j];
      A[j] = score;
      score += tmp;
    }

    for (int j = 0; j < r; j++) {
      char ch = arr[j][i];
      res[A[(int)ch]] = arr[j];
      A[(int)ch]++;
    }

    for (int j = 0; j < r; j++) {
      arr[j] = res[j];
    }
  }

  delete[] res;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Not enough arguments given" << std::endl;
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input.is_open()) {
    std::cout << "Failed to open input file" << std::endl;
    return 1;
  }

  std::ofstream output(argv[2]);
  if (!output.is_open()) {
    std::cout << "Failed to open output file" << std::endl;
    input.close();
    return 1;
  }

  int n, m, k;
  input >> n >> m >> k;
  char** table = new char*[n + 1];

  for (int i = 0; i < n; i++) {
    table[i] = new char(m + 1);
  }
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      input >> table[j][i];
    }
  }

  radixsort(table, n, m, k);

  for (int i = 0; i < n; i++) {
    output << table[i][0];
    delete table[i];
  }

  delete[] table;
  input.close();
  output.close();
  return 0;
}
