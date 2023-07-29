#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#include <cassert>


#ifdef DEFAULT_LIST_ALLOC_ELEMENTS
#else
    #define DEFAULT_LIST_ALLOC_ELEMENTS 128
#endif

#ifdef ALLOW_LIST_RESIZE_ON_COPY
#else
    #define ALLOW_LIST_RESIZE_ON_COPY 1
#endif

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

    ~Node(){
        delete next;
    }

    T value;
    Node<T>* next;
};

template <typename T>
class LinkedList
{
public:
    LinkedList() {}

    ~LinkedList(){
        delete next;
    }

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
    Node<T>* get(size_t index)
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

    T operator[](size_t key)
    {
        if(key >= size)
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
		if (index > size+1)
		{
			return;
		}
        if(index == 0)
        {
            Node<T>* insertedNode = new Node<T>(value);
            if(!next){
                next = insertedNode;
                tail = insertedNode;
            }else{
                Node<T>* oldNode = next;
                next = insertedNode;
                next->next = oldNode;
            }
        }else{
            Node<T>* left_node = get(index-1);
            Node<T>* mid_node = left_node->next;
            Node<T>* new_node = new Node<T>(value);
            left_node->next = new_node;
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

template <typename T>
struct List
{
    List(size_t elementCount){
        listRealloc(elementCount);
    }

    List(){
        listRealloc(DEFAULT_LIST_ALLOC_ELEMENTS);
    }

    ~List(){
        delete data;
    }

    void push(T value){
        if(size+1 <= capacity){
            data[size] = value;
            size++;
        }else{
            listRealloc((capacity+1)*3);
            push(value);
        }
    }

    T& get(size_t index)
    {
        if(index >= size){
            throw std::out_of_range("index" + std::to_string(index) +" out of range. List size is " + std::to_string(size));
        }
        return data[index];
    }

    void insert(size_t index, T value){
        if(size+1 <= capacity){
            if(index == size){
                data[size] = value;
                size++;
            }else if(index < size){
                memmove(data+(index+1), data+index, (size-index)*sizeof(T));
                data[index] = value;
                size++;
            }
            else if(index > size){
                throw std::out_of_range("you cannot insert over the size that would create empty spaces in array");
            }
        }else{
            listRealloc((capacity+1)*3);
            insert(index, value);
        }
    }

    T pop(size_t index){
        if(index >= size){
            throw std::out_of_range("index" + std::to_string(index) +" out of range. List size is " + std::to_string(size));
        }
        T tmp = data[index];
        memcpy(data + index, data + index + 1, (size - index - 1) * sizeof(T));
        size--;
        return tmp;
    }
    
    T& operator[](size_t index)
    {
        return get(index);
    }

    void resize(size_t elementCount){
        listRealloc(elementCount);
    }

    List<T> sortedCopy(){
        List<T> sortedList = List<T>(capacity);
        copyToList(&sortedList);
        sortedList.sort();
        return sortedList;
    }

    List<T> copy(){
        List<T> copyList = List<T>(capacity);
        copyToList(&copyList);
        return copyList;
    }

    void sort(){
        this->quickSort(0, size-1);
    }

    void copyToList(List<T>* dst){
        if(dst->capacity < size){
            if(ALLOW_LIST_RESIZE_ON_COPY == 1){
                dst->resize(size);
                copyToList(dst);
                return;
            }else{
                throw std::out_of_range("destination array is smaller than array your copying from");
            }
        }
        memcpy(dst->data, data, size*sizeof(T));
        dst->size = size;
    }

    size_t capacity = 0;
    size_t size = 0;
private:
    void listRealloc(size_t elementCount) {
        if(data != nullptr){ 
            T* newData = (T*) malloc(elementCount*sizeof(T));
            size_t dataCopy = 0;
            if(elementCount*sizeof(T) < size*sizeof(T)){
                dataCopy = elementCount*sizeof(T);
                size = elementCount;
            }else{
                dataCopy = size*sizeof(T);
            }
            memcpy(newData, data, dataCopy);
            delete data;
            data = newData;
            capacity = elementCount;
        }else{
            data = (T*) malloc(elementCount*sizeof(T));
            capacity = elementCount;
        }
    }

    int partition(int low, int high)
    {
        int pivot = data[high];
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (data[j] < pivot) {
                i++;
                std::swap(data[i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        return (i + 1);
    }

    // taken from https://www.geeksforgeeks.org/quick-sort/ and sligthly modified
    void quickSort(int low, int high)
    {
        if (low < high) {
            int pi = partition(low, high);
    
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
        }
    }

    T* data = nullptr;
};

void test_ll(){
    std::cout << "\nStarting Linked List tests\n\n";
    LinkedList<int> ll;

    ll.push(2);
    assert(ll[0] == 2 || ll.get(0)->value == 2);
    std::cout << "push, operator[] and get passed" << std::endl;

    ll.insert(0, 1);
    assert(ll[0] == 1);
    std::cout << "insert passed" << std::endl;

    int arr[5] = {4, 8, 16, 32, 64};
    ll.fill(arr, 5);

    assert(ll[2] == 4 && ll[6] == 64);
    std::cout << "array fill passed" << std::endl;

    ll.push(128);
    assert(ll[7] == 128);
    std::cout << "final push passed" << std::endl;

    std::cout << "\nFinished Linked List tests\n\n";
}

void test_list(){
    std::cout << "Starting List tests\n\n";
    List<int> list = List<int>(2);

    list.push(2);
    assert(list[0] == 2);
    std::cout << "push and get passed" << std::endl;

    list.insert(0, 1);
    assert(list[0] == 1);
    std::cout << "insert passed" << std::endl;

    // list will have to resize here
    list.push(4);
    assert(list[2] == 4 && list.capacity > 2);
    std::cout << "resize after reaching capacity passed" << std::endl;

    assert(list[0] == 1 && list[1] == 2);
    std::cout << "List keeping its values after resize passed" << std::endl;

    list.insert(0, 21);
    list.insert(0, 43);
    list.sort();
    assert(list[0] == 1 && list[3] == 21 && list[4] == 43);
    std::cout << "List sorting passed" << std::endl;

    std::cout << "\nFinished List tests\n\n";
}


int main(){
    std::cout << "starting test" << "\n";
    test_ll();
    test_list();
    std::cout << "all tests passed" << "\n";
}