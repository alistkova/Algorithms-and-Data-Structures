#include <fstream>
#include <iostream>

struct Node {
  int key;
  int pos;
};

class priority_queue {
 private:
  void min_heapify(int i) {
    int lowest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    while (true) {
      if (l < size && arr[l].key < arr[lowest].key) lowest = l;
      if (r < size && arr[r].key < arr[lowest].key) lowest = r;

      if (lowest == i) break;

      Node temp = arr[i];
      arr[i] = arr[lowest];
      arr[lowest] = temp;
      pos[arr[i].pos] = i;
      pos[arr[lowest].pos] = lowest;

      i = lowest;
      l = 2 * i + 1;
      r = 2 * i + 2;
    }
  }

 public:
  Node* arr;
  int* pos;
  int size;
  int capacity;
  priority_queue() {
    size = 0;
    capacity = 0;
  }
  ~priority_queue() {
    if (capacity > 0) {
      delete[] arr;
    }
  }
  void decrease_key(int i, int key) {
    arr[i].key = key;
    int parent = (i - 1) / 2;
    while (i > 0 && arr[parent].key > arr[i].key) {
      Node temp = arr[i];
      arr[i] = arr[parent];
      arr[parent] = temp;
      pos[arr[i].pos] = i;
      pos[arr[parent].pos] = parent;
      i = parent;
      parent = (i - 1) / 2;
    }
  }
  void push(int key, int pos1, int k) {
    if (capacity == 0) {
      arr = new Node[1];
      arr[0].key = key;
      pos = new int[k + 1];
      arr[0].pos = pos1;
      pos[pos1] = 0;
      capacity = 1;
      size = 1;
    } else {
      if (capacity == size) {
        capacity = capacity * 2;
        Node* temp = new Node[capacity];
        for (int i = 0; i < size; ++i) {
          temp[i] = arr[i];
        }
        delete[] arr;
        arr = temp;
      }
      size++;
      arr[size - 1].key = key;
      arr[size - 1].pos = pos1;
      pos[pos1] = size - 1;
      int i = size - 1;
      int parent = (i - 1) / 2;
      while (i > 0 && arr[parent].key > arr[i].key) {
        Node temp = arr[i];
        arr[i] = arr[parent];
        arr[parent] = temp;
        pos[arr[i].pos] = i;
        pos[arr[parent].pos] = parent;
        i = parent;
        parent = (i - 1) / 2;
      }
    }
  }
  void extract_min(std::ofstream& output) {
    if (size < 1) {
      output << "*\n";
      return;
    }

    int min = arr[0].key;
    arr[0] = arr[size - 1];
    pos[arr[0].pos] = 0;
    size -= 1;
    min_heapify(0);

    output << min << "\n";
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

  priority_queue pq;

  input.close();
  output.close();
  return 0;
}