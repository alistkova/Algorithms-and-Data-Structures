#include <fstream>
#include <iostream>

int hashfunc(int key, int m) { return (std::abs(key) % m); }

struct Node {
  int key;
  Node* left;
  Node* right;
};

class HashSet {
 public:
  Node** array;
  int m;

  HashSet(int m1) : m(m1), array(new Node*[m1]) {
    for (int i = 0; i < m1; i++) {
      array[i] = nullptr;
    }
  }
  Node* find(int key) {
    Node* cur = array[hashfunc(key, m)];
    while (cur != nullptr) {
      if (cur->key == key) {
        return cur;
      }
      cur = cur->right;
    }
    return nullptr;
  }
  void remove(int key) {
    Node* cur = find(key);
    if (cur == nullptr)
      return;
    else {
      if (cur->left != nullptr) {
        cur->left->right = cur->right;
        if (cur->right != nullptr) {
          cur->right->left = cur->left;
        }
      } else {
        array[hashfunc(key, m)] = cur->right;
        if (cur->right != nullptr) {
          cur->right->left = nullptr;
        }
      }
      delete cur;
    }
  }
  void insert(int key) {
    Node* cur = find(key);
    if (cur != nullptr)
      return;
    else {
      int h = hashfunc(key, m);
      Node* newNode = array[h];
      if (newNode == nullptr) {
        newNode = new Node;
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        array[h] = newNode;
      } else {
        Node* temp = newNode;
        newNode = new Node;
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = temp;
        temp->left = newNode;
        array[h] = newNode;
      }
    }
  }
  ~HashSet() {
    for (int i = 0; i < m; ++i) {
      Node* cur = array[i];
      while (cur != nullptr) {
        Node* temp = cur;
        cur = cur->right;
        delete temp;
      }
    }
    delete[] array;
  }
};

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

  int n;
  input >> n;

  HashSet T = HashSet(n / 6);
  for (int i = 1; i <= n; ++i) {
    char command;
    int num;
    input >> command >> num;
    switch (command) {
      case '+':
        T.insert(num);
        break;
      case '-':
        T.remove(num);
        break;
      case '?':
        output << (T.find(num) ? "true" : "false") << '\n';
        break;
      default:
        std::cerr << "Undefined command: '" << command << "'\n";
    }
  }
  input.close();
  output.close();
  return 0;
}