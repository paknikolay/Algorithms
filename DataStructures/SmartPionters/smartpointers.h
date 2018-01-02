#pragma once
template <typename  T>
class UniquePtr {
public:
    UniquePtr(T *p) {
        ptr = p;
    }

    UniquePtr() {
        ptr = nullptr;
    }

    UniquePtr(const UniquePtr<T> &other) = delete;

    UniquePtr operator=(const UniquePtr<T>& other) = delete;

    UniquePtr& operator=(UniquePtr<T>&& other){
        ptr = other.release();
        return *this;
    }

    ~UniquePtr() {
        remove();
    }

    UniquePtr(UniquePtr<T> &&other) {
        ptr = other.release();
    }

    T *release() {
        T *tmp = ptr;
        ptr = nullptr;
        return tmp;
    }

    void reset(T *p) {
        remove();
        ptr = p;
    }

    void reset() {
        remove();
        ptr = nullptr;
    }

    void swap(UniquePtr<T> &other) {
        std::swap(ptr, other.ptr);
    }

    T *get() {
        return ptr;
    }

    T &operator*() {
        return *ptr;
    }

    T *operator->() {
        return get();
    }

private:
    T *ptr;

    void remove() {
        if (ptr == nullptr)
            return;
        delete ptr;
    }

};


template<typename T>
class WeakPtr;

template<typename T>
class SharedPtr{
public:
    friend WeakPtr<T>;

    SharedPtr(){
        count = new size_t(1);
        weakCount = new size_t(0);
        ptr = nullptr;
    }

    SharedPtr(T* p){
        count = new size_t(1);
        weakCount = new size_t(0);
        ptr = p;
    }


    SharedPtr(const WeakPtr<T>& other){
        count = other.count;
        weakCount = other.weakCount;
        ++*count;
        ptr = other.ptr;
    }

    SharedPtr(const SharedPtr& other){
        count = other.count;
        weakCount = other.weakCount;
        ++*count;
        ptr = other.ptr;
    }

    SharedPtr(SharedPtr&& other){
        count = other.count;
        weakCount = other.weakCount;
        ptr = other.ptr;
        other.weakCount = nullptr;
        other.count = nullptr;
        other.ptr = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other){
        remove();
        count = other.count;
        weakCount = other.weakCount;
        ++*count;
        ptr = other.ptr;
        return *this;
    }

    SharedPtr& operator =(SharedPtr&& other){
        remove();
        count = other.count;
        weakCount = other.weakCount;
        ptr = other.ptr;
        other.weakCount = nullptr;
        other.count = nullptr;
        other.ptr = nullptr;
        return *this;
    }


    ~SharedPtr(){
        remove();
    }

    T* get(){
        return ptr;
    }

    T&operator*(){
        return *ptr;
    }

    T* operator->(){
        return get();
    }

    void reset(T* p){
        remove();
        count = new size_t(1);
        weakCount = new size_t(0);
        ptr = p;
    }

    void reset(){
        remove();
        count = new size_t(1);
        weakCount = new size_t(0);
        ptr = nullptr;
    }

    void swap(SharedPtr& other){
        std::swap(ptr, other.ptr);
        std::swap(count, other.count);
        std::swap(weakCount, other.weakCount);
    }

    size_t use_count(){
        if (count == nullptr)
            return  0;
        return *count;
    }



private:
    size_t* count;
    size_t* weakCount;
    T* ptr;

    void remove() {
        if (count == nullptr)
            return;
        --*count;
        if (*count == 0) {
            if (*weakCount == 0) {
                delete count;
                delete weakCount;
            }
            delete ptr;
        }
    }

};


template<typename T>
class WeakPtr {
public:
    friend SharedPtr<T>;

    WeakPtr(T *p) = delete;

    WeakPtr() {
        count = new size_t(0);
        weakCount = new size_t(1);
        ptr = nullptr;
    }

    WeakPtr(const SharedPtr<T> &sharedPtr) {
        count = sharedPtr.count;
        weakCount = sharedPtr.weakCount;
        ptr = sharedPtr.ptr;
    }


    WeakPtr(const WeakPtr &other) {
        weakCount = other.weakCount;
        count = other.count;
        ptr = other.ptr;
    }

    WeakPtr(WeakPtr &&other) {
        count = other.count;
        ptr = other.ptr;
        weakCount = other.weakCount;
        other.count = nullptr;
        other.ptr = nullptr;
        other.weakCount = nullptr;
    }

    WeakPtr &operator=(const WeakPtr &other) {
        remove();
        count = other.count;
        weakCount = other.weakCount;
        ptr = other.ptr;
        return *this;
    }

    WeakPtr &operator=(const SharedPtr<T> &other) {
        remove();
        count = other.count;
        ptr = other.ptr;
        weakCount = other.weakCount;
        return *this;
    }

    WeakPtr &operator=(WeakPtr &&other) {
        remove();
        count = other.count;
        weakCount = other.weakCount;
        ptr = other.ptr;
        other.weakCount = nullptr;
        other.count = nullptr;
        other.ptr = nullptr;
        return *this;
    }


    ~WeakPtr() {
        --weakCount;
    }


    void reset(T *p) {
        remove();
        ptr = p;
    }

    void reset() {
        remove();
        ptr = nullptr;
    }

    void swap(WeakPtr &other) {
        std::swap(ptr, other.ptr);
        std::swap(count, other.count);
        std::swap(weakCount, other.weakCount);
    }

    long use_count() {
        if (count == nullptr)
            return 0;
        return *count;
    }

    bool expired() {
        return *count == 0;
    }

    SharedPtr<T> lock() {
        return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
    }


private:
    size_t *count;
    size_t *weakCount;
    T *ptr;

    void remove() {
        --*weakCount;
        if (*count == 0 && *weakCount == 0) {
            delete count;
            delete weakCount;
        }
    }
};

