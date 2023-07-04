#ifndef SRC_LIB_QUEUE
#define SRC_LIB_QUEUE

#include "DoublyLinkedList.hpp"

// Class implementasi queue menggunakan linked list.
template <typename T>
class Queue {
 private:
  // Penyimpanan aslinya linked list.
  DoublyLinkedList<T> _container;

 public:
  bool isEmpty() { return _container.length() == 0; }

  int size() { return _container.length(); }

  void enqueue(T data) { _container.insertBelakang(data); }

  void dequeue() { _container.hapusDepan(); }

  T& front() { return _container.head->data; }

  T& back() { return _container.tail->data; }
};

#endif /* SRC_LIB_QUEUE */
