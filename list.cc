#include "list.hh"

template<typename T>
bool List<T>::Add(T* data) {
    Node* oldHead = m_Head;
    Node* newHead = new Node;
    if (newHead == nullptr)
        return false;

    newHead->data = data;
    newHead->next = oldHead;
    m_Head = newHead;
    return true;
}

template<typename T>
void List<T>::Remove(T* data) {
    // linear search aaaah
    Node* node;
    Node* prev = nullptr;
    Node* next = m_Head;
    while (true) {
        node = next;
        if (node == nullptr)
            return;
        if (node->data == data) break;
        next = node->next;
        prev = node;
    }
    if (prev == nullptr) {
        m_Head = node->next;
    } else {
        prev->next = node->next;
    }
    delete node;
}

template<typename T>
void List<T>::Clear() {
    Node* node = m_Head;
    while (node != nullptr) {
        Node* next = node->next;
        delete node;
        node = next;
    }
    m_Head = nullptr;
}

template<typename T>
void List<T>::Iterate(T*& out, Node*& rest) {
    if (rest == nullptr) {
        out = nullptr;
    } else {
        out = rest->data;
        rest = rest->next;
    }
}
