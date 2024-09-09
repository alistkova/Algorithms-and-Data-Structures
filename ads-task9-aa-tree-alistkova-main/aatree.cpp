#include <fstream>
#include <iostream>

struct Node {
    int val;
    int level;
    Node* left;
    Node* right;
};

class AATree {
   private:
    Node* root_;

    static Node* skew(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->left == nullptr) {
            return node;
        }
        if (node->left->level == node->level) {
            Node* left = node->left;
            node->left = left->right;
            left->right = node;
            return left;
        }
        return node;
    }

    static Node* split(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->right == nullptr || node->right->right == nullptr) {
            return node;
        }
        if (node->level == node->right->right->level) {
            Node* right = node->right;
            node->right = right->left;
            right->left = node;
            ++right->level;
            return right;
        }
        return node;
    }

    static int get_node_level(Node* node) {
        return node == nullptr ? 0 : node->level;
    }

    static Node* fix_level(Node* node) {
        int level =
            std::min(get_node_level(node->left), get_node_level(node->right)) +
            1;
        if (level < node->level) {
            node->level = level;
            if (node->right != nullptr && level < node->right->level) {
                node->right->level = level;
            }
        }
        return node;
    }

    Node* insert_recursive(Node* curr, int val) {
        if (root_ == nullptr) {
            return root_ = new Node{val, 1, nullptr, nullptr};
        }
        if (curr == nullptr) {
            return new Node{val, 1, nullptr, nullptr};
        }
        if (curr->val == val) {
            return curr;
        }
        if (val < curr->val) {
            curr->left = insert_recursive(curr->left, val);
        } else {
            curr->right = insert_recursive(curr->right, val);
        }

        curr = skew(curr);
        curr = split(curr);
        return curr;
    }

    Node* remove_recursive(Node* curr, int val) {
        if (curr == nullptr) {
            return curr;
        }
        if (val < curr->val) {
            curr->left = remove_recursive(curr->left, val);
        } else if (val > curr->val) {
            curr->right = remove_recursive(curr->right, val);
        } else {
            if (curr->left == nullptr && curr->right == nullptr) {
                delete curr;
                return nullptr;
            }

            if (curr->left == nullptr) {
                Node* right = curr->right;
                delete curr;
                return right;
            }

            if (curr->right == nullptr) {
                Node* left = curr->left;
                delete curr;
                return left;
            }

            Node* succ = curr->right;
            while (succ->left != nullptr) {
                succ = succ->left;
            }
            curr->val = succ->val;
            curr->right = remove_recursive(curr->right, succ->val);
        }
        curr = fix_level(curr);
        curr = skew(curr);
        curr->right = skew(curr->right);
        if (curr->right != nullptr) {
            curr->right->right = skew(curr->right->right);
        }
        curr = split(curr);
        curr->right = split(curr->right);
        return curr;
    }

    void delete_nodes(Node* curr) {
        if (curr == nullptr) {
            return;
        }
        delete_nodes(curr->left);
        delete_nodes(curr->right);
        delete curr;
    }

   public:
    AATree() : root_(nullptr) {}
    ~AATree() { delete_nodes(root_); }
    int root_level() { return get_node_level(root_); }
    void insert(int val) { root_ = insert_recursive(root_, val); }
    bool find(int val) {
        Node* curr = root_;
        while (curr != nullptr) {
            if (curr->val == val) {
                return true;
            }
            if (val < curr->val) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return false;
    }
    void remove(int val) { root_ = remove_recursive(root_, val); }
};

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        std::cout << "Wrong number of arguments given" << std::endl;
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

    AATree tree;
    int n;
    input >> n;
    char op;
    int x;
    for (int i = 0; i < n; ++i) {
        input >> op >> x;
        switch (op) {
            case '+':
                tree.insert(x);
                output << tree.root_level() << '\n';
                break;

            case '-':
                tree.remove(x);
                output << tree.root_level() << '\n';
                break;

            case '?':
                output << (tree.find(x) ? "true\n" : "false\n");
                break;
        }
    }

    input.close();
    output.close();
    return 0;
}
