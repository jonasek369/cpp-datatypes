#include <iostream>

template <typename T>
struct Node {
    Node(T p_value) {
        value = p_value;
        next = NULL;
    }

    Node() {
        value = NULL;
        next = NULL;
    }

    T value;
    Node<T>* next;
};

template <typename T>
class LinkedList
{
public:
    LinkedList() {}

    // push to back
    void push(T value)
    {
        if (!tail && !next) {
            next = new Node<T>(value);
            tail = next;
        }
        else
        {
            tail->next = new Node<T>(value);
            tail = tail->next;
        }
        size++;
    }

    // pop node at index
    void pop(int index)
    {
        if (!next)
            return;

        if (size == 1)
        {
            delete next;
            next = NULL;
            tail = NULL;
            size--;
            return;
        }

        size--;

        if (index == 0)
        {
            Node<T>* right_node = next->next;
            delete next;
            next = right_node;
            return;
        }

        Node<T>* left_node = get(index - 1);

        if (!left_node->next)
        {
            delete next;
            next = NULL;
            tail = NULL;
            return;
        }
        Node<T>* mid_node = left_node->next;

        if (!(mid_node->next))
        {
            tail = left_node;
            delete mid_node;
            left_node->next = NULL;
            return;
        }

        Node<T>* right_node = mid_node->next;
        delete mid_node;
        right_node->next = left_node;

    }

    //get node at index
    Node<T>* get(int index)
    {
        Node<T>* node = next;
        int counter = 0;
        while (node && counter != index)
        {
            node = node->next;
            counter++;
        }
        if (counter != index || !node)
        {
            return NULL;
        }
        return node;
    }

    void print_chain()
    {
        Node<T>* node = next;
        while (node)
        {
            std::cout << node->value << " ";
            node = node->next;
        }
        std::cout << "\n";
    }

    int size = 0;
private:
    Node<T>* next = NULL;
    Node<T>* tail = next;

};


int main()
{
    std::cout << "starting test" << "\n";
    LinkedList<int> test;
    test.push(12); // adds node with value 12 index: O | size 1
    test.push(43); // adds node with value 43 index: 1 | size 2
    test.pop(0); // removes node with index O | index 1 -> index 0 | size 1
    test.pop(0); // removes node with index 0 | size 1
    test.pop(0); // does nothing beacuse size = 0
    std::cout << test.size << "\n";
}
