#pragma once
#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    typedef T value_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::const_pointer const_pointer;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::const_reference const_reference;
    typedef size_t size_type;
    typedef long long difference_type;
    typedef Allocator allocator_type;

    class Node {
    public:
        friend class List<value_type, Allocator>;
        Node() {
            next = previous = nullptr;
        }

        Node(value_type val) {
            value = val;
            next = previous = nullptr;
        }
        value_type getValue(){
            return value;
        }

        ~Node() {
        };
    private:
        Node *next;
        Node *previous;
        value_type value;

    };


    explicit List(const Allocator &alloc = Allocator()) {
        currentSize = 0;
        begin = end = nullptr;
    }

    List(size_type count, const T &value = T(), const Allocator &alloc = Allocator()) {
        currentSize = 0;
        begin = end = nullptr;
        for (int i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    List(const List<value_type, Allocator> & other) {
        currentSize = 0;
        begin = end = nullptr;
        Node *i = other.begin;
        while (i != other.end) {
            push_back(i->getValue());
            i = i->next;
        }
        push_back(other.end->getValue());
    }

    List(List<value_type , Allocator>&& other) {
        currentSize = other.currentSize;
        begin = other.begin;
        end = other.end;
        other.begin = other.end = nullptr;
        other.currentSize = 0;
    }

    ~List(){
        while(currentSize != 0) {
            pop_back();
        }
    }

    List operator = (const List<value_type , Allocator> & other){
        currentSize = 0;
        begin = end = nullptr;
        Node *i = other.begin;
        while (i != other.end) {
            push_back(i->getValue());
            i = i->next;
        }
        push_back(other.end->getValue());
    }

    List operator= (List<value_type , Allocator>&& other) {
        currentSize = other.currentSize;
        begin = other.begin;
        end = other.end;
        other.begin = other.end = nullptr;
        other.currentSize = 0;
    }

    size_type size() {
        return currentSize;
    }

    template<typename Type>
    void push_back(Type &&value) {
        if (currentSize == 0) {
            ++currentSize;
            begin = allocatorNode.allocate(1);
            allocatorType.construct(&(begin->value), std::forward<Type>(value));
            begin->next = begin->previous = nullptr;
            end = begin;
            return;
        }
        ++currentSize;
        end->next = allocatorNode.allocate(1);
        allocatorType.construct(&(end->next->value), std::forward<Type>(value));
        end->next->previous = end;
        end->next->next = nullptr;
        end = end->next;
    }

    template<typename Type>
    void push_front(Type &&value) {
        if (currentSize == 0) {
            ++currentSize;
            begin = allocatorNode.allocate(1);
            allocatorType.construct(&(begin->value), std::forward<Type>(value));
            begin->previous = begin->next = nullptr;
            end = begin;
            return;
        }
        ++currentSize;
        begin->previous = allocatorNode.allocate(1);
        allocatorType.construct(&(begin->previous->value), std::forward<Type>(value));
        begin->previous->previous = nullptr;
        begin->previous->next = begin;
        begin = begin->previous;
    }


    void pop_back() {
        if (currentSize == 0)
            return;
        --currentSize;
        Node* tmp = end->previous;
        allocatorType.destroy(&end->value);
        allocatorNode.deallocate(end, 1);
        end = tmp;
    }

    void pop_front() {
        if (currentSize == 0)
            return;
        --currentSize;
        Node* tmp = begin->next;
        allocatorType.destroy(&begin->value);
        allocatorNode.deallocate(begin, 1);
        begin = tmp;
    }


    template<typename Type>
    void insert_before(Node* node, Type &&value) {
        ++currentSize;
        Node* tmp = allocatorNode.allocate(1);
        allocatorType.construct(&(tmp->value), std::forward<Type>(value));
        tmp->previous = node->previous;
        tmp->next = node;
        node->previous = tmp;
        if(node == begin)
            begin = tmp;
        else
            tmp->previous->next = tmp;

    }

    template<typename Type>
    void insert_afer(Node* node, Type &&value) {
        ++currentSize;
        Node* tmp = allocatorNode.allocate(1);
        allocatorType.construct(&(tmp->value), std::forward<Type>(value));
        tmp->next = node->next;
        tmp->previous = node;
        node->next = tmp;
        if(node == end)
            end = tmp;
        else
            tmp->next->previous = tmp;
    }

    void erase(Node * node){
        node->previous ->next = node ->next;
        node->next->previous = node ->previous;
        allocatorType.destroy(&node->value);
        allocatorNode.deallocate(node, 1);
    }

    Node *front() {
        return begin;
    }

    Node *back() {
        return end;
    }

    bool empty(){
        return currentSize==0;
    }

private:
    typename Allocator::template rebind<Node>::other allocatorNode;
    typename Allocator::template rebind<T>::other allocatorType;
    size_type currentSize;
    Node *begin;
    Node *end;
};