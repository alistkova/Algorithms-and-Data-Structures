#include <cstring>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    if (argc != 3) {
        std::cout << "Wrong number of arguments given" << '\n';
    }

    char* pattern = argv[1];
    int pattern_length = strlen(pattern);
    std::ifstream input(argv[2]);

    if (!input.is_open()) {
        std::cout << "Failed to open input file" << '\n';
        return 1;
    }

    char str[1025];
    int result = 0;
    int pi[1024];
    pi[0] = 0;
    for (int l = 1; l < pattern_length; ++l) {
        int j = pi[l - 1];
        while ((j > 0) && (pattern[l] != pattern[j])) {
            j = pi[j - 1];
        }
        if (pattern[l] == pattern[j]) {
            ++j;
        }
        pi[l] = j;
    }
    int j = 0;
    while (input.getline(str, 1024, '\n')) {
        for (int i = 0; str[i] != '\0'; ++i) {
            while ((j > 0) && (str[i] != pattern[j])) {
                j = pi[j - 1];
            }
            if (str[i] == pattern[j]) {
                ++j;
            }
            if (j == pattern_length) {
                ++result;
                j = pi[j - 1];
            }
        }
    }
    std::cout << result << '\n';
    input.close();
    return 0;
}
