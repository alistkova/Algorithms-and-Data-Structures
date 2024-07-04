#include <fstream>
#include <iostream>

struct Student {
    int id;
    double grade;
};

void sort(Student* arr, int start, int end) {
    if (start >= end) {
        return;
    }
    
    int i = start;
    int j = end;
    Student pivot = arr[(i + j) / 2];

    while (i <= j) {
        while (arr[i].grade < pivot.grade) {
            ++i;
        }
        while (arr[j].grade > pivot.grade) {
            --j;
        }
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }
    sort(arr, start, j);
    sort(arr, i, end);
}


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cout << "Not enough arguments given" << std::endl;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cout << "Failed to open input file" << std::endl;
    }

    int n;
    input >> n;
    Student students[99999];
    for(int i = 0; i < n; ++i) {
        students[i] = Student{i + 1, 0.0};
        input >> students[i].grade;
    }
    sort(students, 0, n - 1);
    std::cout << students[0].id << ' ' << students[n / 2].id << ' ' << students[n - 1].id << std::endl;
}
