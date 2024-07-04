#include <iostream>
#include <fstream>

const int MAX_STEP = 1e6;

class Queue {
private:
    char* buffer_;
    int capacity_;
    int size_;
    int front_;
    int back_;

public:
    Queue(int capacity = 100) : buffer_(new char[capacity]{}), capacity_(capacity), size_(0), front_(0), back_(0) {}

    void push(char card) {
        if (size_ == capacity_) {
            return;
        }
        ++size_;
        buffer_[back_] = card;
        back_ = (back_ + 1) % capacity_;
    }

    char pop() {
        if (size_ == 0) {
            throw "Error: empty queue pop";
        }
        --size_;
        char card = buffer_[front_];
        front_ = (front_ + 1) % capacity_;
        return card;
    }

    bool is_empty() const {
        return size_ == 0;
    }

    ~Queue() {
        delete[] buffer_;
    }
    
};

char card_to_value(std::string& card) {
    if (card == "2") {
        return 2;
    }
    if (card == "3") {
        return 3;
    }
    if (card == "4") {
        return 4;
    }
    if (card == "5") {
        return 5;
    }
    if (card == "6") {
        return 6;
    }
    if (card == "7") {
        return 7;
    }
    if (card == "8") {
        return 8;
    }
    if (card == "9") {
        return 9;
    }
    if (card == "10") {
        return 10;
    }
    if (card == "J") {
        return 11;
    }
    if (card == "Q") {
        return 12;
    }
    if (card == "K") {
        return 13;
    }
    if (card == "A") {
        return 14;
    }
    return 0;
}

bool compare_cards(char card1, char card2) {
    if (card1 == 2 && card2 == 14) {
        return true;
    }
    if (card1 == 14 && card2 == 2) {
        return false;
    }
    return card1 > card2;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Not enough arguments" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cout << "Can not open input file" << std::endl;
        return 1;
    }

    Queue deck1;
    Queue deck2;
    Queue table;

    std::string card;
    for (int i = 0; i < 26; ++i) {
        input >> card;
        deck1.push(card_to_value(card));
        input >> card;
    }

    for (int i = 0; i < 26; ++i) {
        input >> card;
        deck2.push(card_to_value(card));
        input >> card;
    }

    int step = 0;
    while (step < MAX_STEP && !deck1.is_empty() && !deck2.is_empty()) {
        ++step;

        char card1 = deck1.pop();
        char card2 = deck2.pop();
        table.push(card1);
        table.push(card2);

        if (card1 == card2) {
            continue;
        } 
        if (compare_cards(card1, card2)) {
            while (!table.is_empty()) {
                deck1.push(table.pop());
            }
        } else {
            while (!table.is_empty()) {
                deck2.push(table.pop());
            }
        }
    }

    if (step >= MAX_STEP) {
        std::cout << "unknown" << std::endl;
    } else if (deck1.is_empty() && deck2.is_empty()) {
        std::cout << "draw" << std::endl;
    } else if (deck1.is_empty()) {
        std::cout << "second" << std::endl;
    } else {
        std::cout << "first" << std::endl;
    }

    input.close();
    return 0;
}
