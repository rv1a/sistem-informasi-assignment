#ifndef SRC_LIB_DOUBLYLINKEDLIST
#define SRC_LIB_DOUBLYLINKEDLIST

#include <iostream>

template <typename T> class Node {
public:
  T data;
  Node *prev;
  Node *next;
};

template <typename T> class DoublyLinkedList {
  using NodeT = Node<T>;

public:
  NodeT *head{nullptr};
  NodeT *tail{nullptr};

  int length() {
    int counter = 0;
    NodeT *temp = head;
    while (temp != nullptr) {
      counter++;
      temp = temp->next;
    }
    return counter;
  }

  NodeT *get(int index) {
    if (index > length() - 1) {
      std::cout << "Index melebihi jumlah elemen pada linkedlist!" << std::endl;
      return nullptr;
    }

    if (index == 0) {
      return head;
    }

    NodeT *temp = head;
    for (int i = 0; i < index; i++) {
      temp = temp->next;
    }

    return temp;
  }

  void insertAt(T data, int posisi) {
    if (posisi < 1 || posisi > length()) {
      std::cout << "ERROR: Posisi insert tidak valid" << std::endl;
      return;
    }

    if (posisi == 1) {
      return insertDepan(data);
    }

    NodeT *nextNode = this->get(posisi - 1);
    NodeT *prevNode = nextNode->prev;

    NodeT *newNode = new NodeT();
    newNode->data = data;
    newNode->next = nextNode;
    newNode->prev = nextNode->prev;

    prevNode->next = newNode;
    nextNode->prev = newNode;
  }

  NodeT *insertDepan(T data) {
    NodeT *newNode = new NodeT();
    newNode->data = data;
    newNode->prev = nullptr;
    newNode->next = nullptr;

    if (head == nullptr) {
      head = newNode;
      tail = newNode;
      return newNode;
    }

    head->prev = newNode;
    newNode->next = head;
    head = newNode;
    return newNode;
  }

  NodeT *insertBelakang(T data) {
    NodeT *newNode = new NodeT();
    newNode->data = data;
    newNode->prev = nullptr;
    newNode->next = nullptr;

    if (head == nullptr) {
      head = newNode;
      tail = newNode;
      return newNode;
    }

    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    return newNode;
  }

  bool hapusDepan() {
    if (head == nullptr) {
      return false;
    }

    NodeT *temp = head;
    NodeT *afterHead = head->next;
    if (afterHead) {
      afterHead->prev = nullptr;
    }
    head = afterHead;
    delete temp;
    return true;
  }

  bool hapusBelakang() {
    if (tail == nullptr) {
      return false;
    }

    NodeT *temp = tail;
    NodeT *beforeTail = tail->prev;
    if (beforeTail) {
      beforeTail->next = nullptr;
    }
    tail = beforeTail;
    delete temp;
    return true;
  }

  bool hapusTengah(int posisi) {
    if (posisi < 1 || posisi > length()) {
      std::cout << "ERROR: Posisi hapus tidak valid" << std::endl;
      return false;
    }

    if (posisi == 1) {
      return hapusDepan();
    }

    NodeT *temp = this->get(posisi - 1);
    NodeT *next = temp->next;
    NodeT *prev = temp->prev;
    prev->next = next;
    next->prev = prev;
    delete temp;
    return true;
  }

  bool hapusNode(NodeT *node) {
    if (node == head) {
      return hapusDepan();
    }

    if (node == tail) {
      return hapusBelakang();
    }
    auto *next = node->next;
    auto *prev = node->prev;
    if (next) {
      next->prev = prev;
    }
    if (prev) {
      prev->next = next;
    }

    delete node;
    return true;
  }

  bool updateDepan(T newData) {
    if (head == nullptr) {
      return false;
    }

    head->data = newData;
    return true;
  }

  bool updateBelakang(T newData) {
    if (tail == nullptr) {
      return false;
    }

    tail->data = newData;
    return true;
  }

  bool updateTengah(T newData, int posisi) {
    NodeT *toUpdate = this->get(posisi - 1);
    if (toUpdate != nullptr) {
      toUpdate->data = newData;
      return true;
    }
    return false;
  };

  void deleteAll() {
    NodeT *current = head;
    while (current != nullptr) {
      NodeT *temp = current;
      current = current->next;
      delete temp;
    }
    head = nullptr;
    tail = nullptr;
  }
};

#endif /* SRC_LIB_DOUBLYLINKEDLIST */
