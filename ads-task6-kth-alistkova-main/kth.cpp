#include <fstream>
#include <iostream>

void sort(int* arr, int start, int end, int k1, int k2) {
    if (start >= end || start > k2 || end < k1) {
        return;
    }
    
    int i = start;
    int j = end;
    int pivot = arr[(i + j) / 2];

    while (i <= j) {
        while (arr[i] < pivot) {
            ++i;
        }
        while (arr[j] > pivot) {
            --j;
        }
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }
    sort(arr, start, j, k1, k2);
    sort(arr, i, end, k1, k2);
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
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

    int n;
    int k1;
    int k2;
    input >> n >> k1 >> k2;
    int A;
    int B;
    int C;
    int x1;
    int x2;
    input >> A >> B >> C >> x1 >> x2;
    int* arr = new int[n];
    arr[0] = x1;
    arr[1] = x2;
    for (int i = 2; i < n; ++i) {
        arr[i] = A * arr[i - 2] + B * arr[i - 1] + C;
    }
    sort(arr, 0, n - 1, k1 - 1, k2 - 1);
    for (int i = k1 - 1; i <= k2 - 1; ++i) {
        output << arr[i];
        if (i != k2 - 1) {
            output << " ";
        }
    }

    delete[] arr;
    return 0;
}
