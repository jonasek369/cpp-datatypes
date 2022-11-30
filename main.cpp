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
        // if the list is empty we set next to a node
        if (!tail && !next) {
            next = new Node<T>(value);
            tail = next;
        }
        // if its not empty we add node to tail and set it as new tail
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

    T operator[](int key)
    {
        if(key <= size || key >= size)
        {
            return -1;
        }
        return get(key)->value;
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

    void insert(int index, T value)
    {
		if (index > size++)
		{
			return;
		}
        if(index == 0)
        {
            Node<T>* old_node = next;
            next = new Node<T>(value);
            if(old_node == tail)
                tail = next;
            next->next = old_node;
			
        }else{
            Node<T>* right_node = get(index-1);
            Node<T>* mid_node = right_node->next;
            Node<T>* new_node = new Node<T>(value);
            right_node->next = new_node;
            new_node->next = mid_node;
        }
        size++;
    }
	
	void fill(T value, int size)
    {
		for(int i = 0; i < size; i++)
		{
			push(value);
		}
	}
	
	
	void fill(T* array, int size)
    {
		for(int i = 0; i < size; i++)
		{
			push(array[i]);
		}
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
    test.push(2);
    test.push(4);
    test.insert(0, 1);
    int arr[5] = {8, 16, 32, 64};
    test.fill(arr, 4);
    test.push(128);
    test.print_chain();
}
