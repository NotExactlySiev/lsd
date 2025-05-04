template <typename T>
class List {
public:
    struct Node {
        Node* next;
        T* data;
    };

    bool Add(T* data);
    void Remove(T* data);
    void Clear();
    static void Iterate(T*& out, Node*& rest);
public:
    Node* m_Head = nullptr;
};