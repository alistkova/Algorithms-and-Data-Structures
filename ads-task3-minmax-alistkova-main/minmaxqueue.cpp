#include <fstream>
#include <iostream>

struct Node {
    int val;
    int min;
    int max;
};

class Stack {
private:
    Node* data_;
    int size_;
    int pos_;

public:
    Stack(int max_size) : data_(new Node[max_size]{0, 0, 0}), size_(max_size), pos_(0) {}
    ~Stack() {
        delete[] data_;
    }

    void push(int val) {
        if (pos_ > size_) {
            throw std::runtime_error("Stack is full!");
        }
        if (pos_ == 0) {
            data_[pos_] = {val, val, val};
        } else {
            data_[pos_] = {val, std::min(data_[pos_ - 1].min, val), std::max(data_[pos_ - 1].max, val)};
        }
        ++pos_;
    }

    int pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty!");
        }
        int val = data_[pos_ - 1].val;
        --pos_;
        return val;
    }

    int min() {
        if (empty()) {
            throw std::runtime_error("Stack is empty!");
        }
        return data_[pos_ - 1].min;
    }

    int max() {
        if (empty()) {
            throw std::runtime_error("Stack is empty!");
        }
        return data_[pos_ - 1].max;
    }

    bool empty() {
        return pos_ == 0;
    }

};

class Queue {
private:
    Stack l_;
    Stack r_;

public:
    Queue(int max_size = 1e6) : l_(max_size), r_(max_size) {}

    void enqueue(int val) {
        l_.push(val);
    }

    int dequeue() {
        if (l_.empty() && r_.empty()) {
            throw std::runtime_error("Queue is empty!");
        }
        if (r_.empty()) {
            while (!l_.empty()) {
                int val = l_.pop();
                if (l_.empty()) {
                    return val;
                }
                r_.push(val);
            }
        }
        return r_.pop();
    }

    int diff() {
        if (l_.empty() && r_.empty()) {
            throw std::runtime_error("Queue is empty!");
        }
        if (l_.empty()) {
            return r_.max() - r_.min();
        }
        if (r_.empty()) {
            return l_.max() - l_.min();
        }
        return std::max(r_.max(), l_.max()) - std::min(r_.min(), l_.min());
    }
};

int main(int argc, const char* argv[]) {
    
    if (argc < 3) {
        std::cout << "Not enought arguments given" << std::endl;
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cout << "Failed to open input file" << std::endl;
        return 1;
    }

    std::ofstream output(argv[2]);
    if (!output.is_open()) {
        input.close();
        std::cout << "Failed to open output file" << std::endl;
        return 1;
    }


    int n;
    char op;
    int val;
    input >> n;
    Queue queue(n);
    for (int i = 0; i < n; ++i) {
        input >> op;
        switch (op) {
        case '+':
            input >> val;
            queue.enqueue(val);
            break;
        case '-':
            queue.dequeue();
            break;
        case '?':
            output << queue.diff() << '\n';
            break;
        default:
            std::cout << "Unknown operation: " << op;
            return 1;
        }
    }

    input.close();
    output.close();
}
