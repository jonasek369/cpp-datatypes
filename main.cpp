#include <iostream>

template <typename T>
struct Node{
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
    LinkedList(){}

    // push to back
    void push(T value)
    {
        if (!tail && !next) {
            next = new Node<T>(value);
            tail = next;
        }else
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
        if (index == 0)
        {
            Node<T>* temp = next->next;
            delete next;
            next = temp;
        } else if(get(index) == tail)
        { 
            Node<T>* pnode = get(index - 1);
            delete pnode->next;
            pnode->next = NULL;
        }
        else {
            Node<T>* left_node = get(index - 1);
            Node<T>* mid_node = left_node->next;
            Node<T>* right_node = mid_node->next;
            delete mid_node;
            left_node->next = right_node;
        }
        size--;
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
    LinkedList<int> test;
    test.push(12);
    test.push(32);
    test.push(82);
    test.push(14);
    test.pop(0);
    test.pop(0);
    test.print_chain();
    std::cout << test.size << "\n";
}
