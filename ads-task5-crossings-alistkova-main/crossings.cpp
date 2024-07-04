#include <iostream>
#include <fstream>
#include <stdint.h>

struct Line {
    int x0;
    int x1;
};

void merge(Line* lines, int left, int mid, int right, uint64_t& invCounter, bool(comp)(Line, Line)) {
    int l_size = mid - left + 1;
    int r_size = right - mid;
    Line* leftLines = new Line[l_size];
    Line* rightLines = new Line[r_size];
 
    for (int i = 0; i < l_size; ++i) {
        leftLines[i] = lines[left + i];
    }
    for (int j = 0; j < r_size; ++j) {
        rightLines[j] = lines[mid + 1 + j];
    }
 
    auto i = 0;
    int j = 0;
    int k = left;
 
    while (i < l_size && j < r_size) {
        if (comp(leftLines[i], rightLines[j])) {
            lines[k++] = leftLines[i++];
        }
        else {
            lines[k++] = rightLines[j++];
            invCounter += l_size - i;
        }
    }

    while (i < l_size) {
        lines[k++] = leftLines[i++];
    }
 
    while (j < r_size) {
        lines[k++] = rightLines[j++];
    }

    delete[] leftLines;
    delete[] rightLines;
}
 

void mergeSort(Line* array, int left, int right, uint64_t& invCounter, bool(comp)(Line, Line)) {
    if (left >= right) {
        return;
    }
 
    int mid = left + (right - left) / 2;
    mergeSort(array, left, mid, invCounter, comp);
    mergeSort(array, mid + 1, right, invCounter, comp);
    merge(array, left, mid, right, invCounter, comp);
}

uint64_t getPointCrossings(Line* lines, int n) {
    uint64_t crossings = 0;
    uint64_t inSamePoint = 1;
    for (int i = 1; i < n; ++i) {
        if (lines[i].x0 == lines[i - 1].x0) {
            ++inSamePoint;
        } else {
            crossings += inSamePoint * (inSamePoint - 1) / 2;
            inSamePoint = 1;
        }
    }
    crossings += inSamePoint * (inSamePoint - 1) / 2;
    inSamePoint = 1;
    for (int i = 1; i < n; ++i) {
        if (lines[i].x1 == lines[i - 1].x1) {
            ++inSamePoint;
        } else {
            crossings += inSamePoint * (inSamePoint - 1) / 2;
            inSamePoint = 1;
        }
    }
    crossings += inSamePoint * (inSamePoint - 1) / 2;
    return crossings;
}


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cout << "Not enough arguments given" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cout << "Failed to open input file" << std::endl;
        return 1;
    }

    int n;
    input >> n;
    Line* lines = new Line[n];
    for (int i = 0; i < n; ++i) {
        input >> lines[i].x0;
        input >> lines[i].x1;
    }
    
    uint64_t invCounter = 0;
    mergeSort(lines, 0, n - 1, invCounter, [](Line l, Line r) {return l.x0 <= r.x0;});
    invCounter = 0;
    mergeSort(lines, 0, n - 1, invCounter, [](Line l, Line r) {return l.x1 <= r.x1;});
    invCounter += getPointCrossings(lines, n);
    std::cout << invCounter << std::endl;

    input.close();
    delete[] lines;
    return 0;

}
