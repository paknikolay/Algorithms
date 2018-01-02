#include <iostream>
#include <ctime>
using std::cout;
using std::cin;

class Treap {// left < right равные вправо
public:
    Treap(){
        size = 0;
        root = nullptr;
        srand(0);
    }
    ~Treap(){
        destroyTree(root);
    }

    int&operator[](int index){
        Node** tmp =find(root, index);
        return (*tmp)->value;
    }
    void insert(int value){
        insert(root, size, rand(), value);
        ++size;
    }

    void insert(int value, int index){
        ++size;
        insert(root, index, rand(), value);
    }

    void remove(int index){
        remove(root, index);
    }
    int findKMax(int number){
        return find_k_max(root, number - 1);
    }

    void print(){
        print(root);
        cout<<"\n";
    }

    int getSize(){
        return size;
    }

    void moveToBegin(int indexFirs, int indexSecond) {
        Node *left, *middle, *right;
        split(root, middle, right, indexSecond + 1);
        split(middle, left, middle, indexFirs);
        Node* tmp = merge(left, right);
        root = merge(middle, tmp);
    }


private:
    struct Node{
        Node(int _priority, int _value) {
            left = right = nullptr;
            priority = _priority;
            amount = 1;
            value = _value;
        }
        Node *left, *right;
        int priority;
        int value;
        int amount;

    };
    Node* root;
    int size;

    void destroyTree(Node* current){
        if(current == nullptr)
            return;
        destroyTree(current->right);
        destroyTree(current->left);
        delete current;
    }

    int getAmount(Node* current){
        if(current == nullptr)
            return 0;
        else
            return  current->amount;
    }
    void fixNode(Node*& current){
        if(current == nullptr)
            return;
        current->amount = getAmount(current->right) + getAmount(current->left) + 1;
    }
    void split(Node* current, Node*& left, Node*& right, int index){
        if (current == nullptr) {
            left = nullptr;
            right = nullptr;
            return;
        }
        if (index <= getAmount(current->left)) {
            split(current->left, left, current->left, index);
            right = current;
        }
        else {
            split(current->right, current->right, right, index - 1 - getAmount(current->left));
            left = current;
        }
        fixNode(current);
    }
    Node* merge(Node* left, Node* right){
        if(left == nullptr) {
            fixNode(right);
            return right;
        }
        if(right == nullptr) {
            fixNode(right);
            return left;
        }
        Node* tmp;
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            fixNode(left->right);
            fixNode(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            fixNode(right->left);
            fixNode(right);
            return right;
        }
    }

    void insert (Node*& current, int index, int priority, int value){
        Node* left, *right, *tmp = new Node(priority, value);
        split(current, left, right, index);
        current = merge(merge(left, tmp), right);
    }
    void remove(Node*& current, int index){
        if(current == nullptr)
            return;
        if(index == getAmount(current) - 1 - getAmount(current->right)) {
            Node* tmp= current;
            current = merge(current->left, current->right);
            fixNode(current);
            --size;
            delete(tmp);
            return;
        }
        if(getAmount(current->left) >= index)
            remove(current->left, index);
        else
            remove(current->right, index - getAmount(current->left) - 1);
        fixNode(current);

    }

    void print(Node* current){
        if(current == nullptr)
            return;
        print(current->left);
       cout<<current->value<<" ";
        print(current->right);

    }
    Node** find(Node*& current, int number){
        if(current == nullptr)
            return nullptr ;
        if(number == getAmount(current) - 1 - getAmount(current->right))
            return &current;
        if(getAmount(current->left) > number)
            find(current->left, number);
        else
            find(current->right, number - getAmount(current->left) - 1);
    }

    int find_k_max(Node* current, int number){
        if(current == nullptr)
            return -1;
        if(number == getAmount(current) - 1 - getAmount(current->left))
            return current->value;
        if(getAmount(current->right) > number)
            find_k_max(current->right, number);
        else
            find_k_max(current->left, number - getAmount(current->right) - 1);
    }
};
