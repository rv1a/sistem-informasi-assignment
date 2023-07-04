#ifndef SRC_LIB_STACK2
#define SRC_LIB_STACK2

#include <iostream>

template <typename T> class Stack2 {
private:
  static const int SIZE = 30;
  T storage[SIZE];
  int top_index = -1;

public:
  bool isEmpty() { return top_index < 0; }

  bool isFull() { return top_index == (SIZE - 1); }

  // Return a copy of the top item
  T top() {
    if (isEmpty()) {
      return NULL;
    }

    T top_item = storage[top_index];
    return top_item;
  }

  T peek() { return this->top(); }

  // Get elemen ke [pos] dari atas stack.
  // Elemen yang ada diatas stack memiliki [pos] 1.
  T peek(int pos) {
    int index = pos - 1;
    if (isEmpty() || index < 0) {
      return NULL;
    }

    index = top_index - index;
    if (index < 0) {
      throw std::runtime_error("Posisi peek melebihi jumlah elemen.");
    }

    return storage[index];
  }

  int count() { return top_index + 1; }

  void push(T data) {
    if (this->isFull()) {
      throw std::runtime_error("Stack is full");
    }
    top_index++;
    storage[top_index] = data;
  }

  T pop() {
    if (this->isEmpty()) {
      throw std::runtime_error("Stack is empty");
    }

    T top = storage[top_index];
    top_index--;
    return top;
  }

  void destroy() {
    while (!isEmpty()) {
      this->pop();
    }
  }

  // Replace elemen yang berada di [pos] dari atas stack.
  // Elemen yang ada di atas stack memiliki pos 1.
  void replaceAt(int pos, T newData) {
    int index = pos - 1;
    if (index < 0 || index > top_index) {
      std::cout << "Posisi replace out of bounds." << std::endl;
      return;
    }

    index = top_index - index;
    storage[index] = newData;
  }

  // Traverse dari element yang paling atas / top of stack sampai bawah
  void traverse(std::function<void(T)> fn) {
    for (int i = top_index; i >= 0; i--) {
      fn(storage[i]);
    }
  }

  void traverseMut(std::function<void(T &)> fn) {
    for (int i = top_index; i >= 0; i--) {
      fn(storage[i]);
    }
  }

  void print() {
    this->traverse([](T elem) { std::cout << elem << std::endl; });
  }
};

#endif /* SRC_LIB_STACK2 */
