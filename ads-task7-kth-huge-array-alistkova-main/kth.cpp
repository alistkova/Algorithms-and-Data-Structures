#include <fstream>
#include <iostream>

class Heap {
   private:
    int* data_;
    size_t capacity_;
    size_t size_;

    size_t parent(int child) {
        if (child % 2 == 0) {
            return (child / 2) - 1;
        } else {
            return (child / 2);
        }
    }

    void shift_up(size_t i) {
        if (i == 0) {
            return;
        }

        size_t parent_index = parent(i);
        if (data_[parent_index] < data_[i]) {
            std::swap(data_[i], data_[parent_index]);
            shift_up(parent_index);
        }
    }

    void shift_down(size_t i) {
        size_t l = 2 * i + 1;
        size_t r = 2 * i + 2;

        if (l >= size_) {
            return;
        }

        size_t max_index = i;
        if (data_[l] > data_[i]) {
            max_index = l;
        }

        if (r < size_ && (data_[r] > data_[max_index])) {
            max_index = r;
        }

        if (max_index != i) {
            std::swap(data_[i], data_[max_index]);
            shift_down(max_index);
        }
    }

   public:
    Heap(int capacity)
        : capacity_(capacity), size_(0), data_(new int[capacity]) {}

    ~Heap() { delete[] data_; }

    void push(int x) {
        if (size_ == capacity_) {
            throw std::runtime_error("Heap is full");
        }
        data_[size_] = x;
        shift_up(size_++);
    }

    int top() {
        if (size_ == 0) {
            throw std::runtime_error("Heap is empty");
        }
        return data_[0];
    }

    void pop() {
        if (size_ == 0) {
            throw std::runtime_error("Heap is empty");
        }
        data_[0] = data_[--size_];
        shift_down(0);
    }

    size_t size() { return size_; }
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

    Heap heap(k2);
    heap.push(x1);
    heap.push(x2);

    for (int i = 2; i < n; ++i) {
        int x = A * x1 + B * x2 + C;

        if (i < k2) {
            heap.push(x);
        } else if (x < heap.top()) {
            heap.pop();
            heap.push(x);
        }
        x1 = x2;
        x2 = x;
    }

    int res_size = k2 - k1 + 1;
    int* res = new int[res_size];
    for (int i = res_size - 1; i >= 0; --i) {
        res[i] = heap.top();
        heap.pop();
    }

    for (int i = 0; i < res_size; ++i) {
        output << res[i];
        if (i != res_size - 1) {
            output << ' ';
        }
    }

    delete[] res;

    input.close();
    output.close();
    return 0;
}
