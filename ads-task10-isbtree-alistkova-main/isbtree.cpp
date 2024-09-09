#include <stddef.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

constexpr int INT_MIN = std::numeric_limits<int>::min();
constexpr int INT_MAX = std::numeric_limits<int>::max();

template <typename T, typename Comp>
void sort(T* data, int start, int end, Comp comp) {
    if (start >= end) {
        return;
    }

    int i = start;
    int j = end;
    T pivot = data[(i + j) / 2];

    while (i <= j) {
        while (comp(data[i], pivot)) {
            ++i;
        }
        while (comp(pivot, data[j])) {
            --j;
        }
        if (i <= j) {
            std::swap(data[i], data[j]);
            ++i;
            --j;
        }
    }
    sort(data, start, j, comp);
    sort(data, i, end, comp);
}

struct Node {
    bool is_leaf;
    uint index;
    int n_keys;
    int n_children;
    int* keys;
    int* children_indices;
};

class BTree {
   private:
    bool is_valid_ = true;
    int n_;
    int t_;
    uint root_index_;
    Node* nodes_;

    int find(uint index) {
        int l = 0;
        int r = n_ - 1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (nodes_[m].index == index) {
                return m;
            }
            if (nodes_[m].index < index) {
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        return -1;
    }

    bool check_keys(Node& node) {
        for (int i = 1; i < node.n_keys; ++i) {
            if (node.keys[i - 1] > node.keys[i]) {
                return false;
            }
        }
        return true;
    }

    bool check_keys_count(Node& node) {
        if (node.index == root_index_) {
            return 1 <= node.n_keys && node.n_keys <= 2 * t_ - 1;
        }
        return t_ - 1 <= node.n_keys && node.n_keys <= 2 * t_ - 1;
    }

    bool check_children_count(Node& node) {
        if (node.is_leaf) {
            return node.n_children == 0;
        }
        return node.n_children == node.n_keys + 1;
    }

    uint dfs(uint index, int level, int min = INT_MIN, int max = INT_MAX) {
        if (!is_valid_) {
            return level;
        }
        int node_pos = find(index);
        if (node_pos == -1) {
            // std::cerr << "Node not found: " << index << std::endl;
            is_valid_ = false;
            return level;
        }
        Node& node = nodes_[node_pos];
        if (!(check_keys(node) && check_keys_count(node) &&
              check_children_count(node))) {
            // std::cerr << "Node checks failed" << std::endl;
            is_valid_ = false;
            return level;
        }

        if (node.keys[0] < min || node.keys[node.n_keys - 1] > max) {
            // std::cerr << "Wrong borders" << std::endl;
            is_valid_ = false;
            return level;
        }

        if (node.is_leaf) {
            return level;
        }

        int leaf_level_first =
            dfs(node.children_indices[0], level + 1, min, node.keys[0]);

        int leaf_level_last = dfs(node.children_indices[node.n_children - 1],
                                  level + 1, node.keys[node.n_keys - 1], max);

        if (leaf_level_first != leaf_level_last) {
            // std::cerr << "First and last level doesn't match: "
            //   << leaf_level_first << " and " << leaf_level_last
            //   << std::endl;
            is_valid_ = false;
            return level;
        }

        for (int i = 1; i < node.n_children - 1; ++i) {
            int leaf_level = dfs(node.children_indices[i], level + 1,
                                 node.keys[i - 1], node.keys[i]);
            if (leaf_level_first != leaf_level) {
                // std::cerr << "Leaf levels doesnt match at " << i <<
                // std::endl;
                is_valid_ = false;
                return level;
            }
        }

        return leaf_level_first;
    }

   public:
    BTree(int n, int t, uint root_index, Node* nodes)
        : is_valid_(true),
          n_(n),
          t_(t),
          root_index_(root_index),
          nodes_(nodes) {}
    ~BTree() {
        if (nodes_ == nullptr) {
            return;
        }
        for (int i = 0; i < n_; ++i) {
            delete[] nodes_[i].keys;
            delete[] nodes_[i].children_indices;
        }
        delete[] nodes_;
    }

    bool validate() {
        dfs(root_index_, 0);
        return is_valid_;
    }
};

class Parser {
   private:
    std::ifstream input_;

   public:
    Parser(const char* file_name) { input_.open(file_name); }
    ~Parser() {}
    bool is_open() { return input_.is_open(); }
    void close() { input_.close(); }

    BTree parse() {
        int n;
        int t;
        int root_index;
        input_ >> n >> t >> root_index;
        Node* nodes = new Node[n];
        for (int l = 0; l < n; ++l) {
            Node node;

            char type[8];
            input_ >> type;

            input_.ignore(3);
            input_ >> node.index;

            input_.ignore(2);
            input_ >> node.n_keys;

            input_.ignore(1);
            node.keys = new int[node.n_keys];
            for (int i = 0; i < node.n_keys; ++i) {
                input_ >> node.keys[i];
            }

            node.is_leaf = true;
            node.n_children = 0;
            node.children_indices = nullptr;

            if (strcmp(type, "branch:") == 0) {
                node.is_leaf = false;

                input_.ignore(3);
                input_ >> node.n_children;

                input_.ignore(1);
                node.children_indices = new int[node.n_children];
                for (int i = 0; i < node.n_children; ++i) {
                    input_ >> node.children_indices[i];
                }
            }

            input_.ignore(1024, '\n');
            nodes[l] = node;
        }
        sort(nodes, 0, n - 1,
             [](Node& l, Node& r) { return l.index < r.index; });
        return BTree(n, t, root_index, nodes);
    }
};

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cout << "Wrong number of arguments given" << std::endl;
        return 1;
    }
    Parser parser(argv[1]);
    if (!parser.is_open()) {
        std::cout << "Failed to open input file";
        return 1;
    }
    auto tree = parser.parse();
    std::cout << (tree.validate() ? "yes" : "no");

    return 0;
}
