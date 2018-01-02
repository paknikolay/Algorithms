#include <iterator>

template <typename T>
class Deque {
public:
    template <typename ValueType, typename  Container>
    class DIterator : public std::iterator<std::random_access_iterator_tag, ValueType> {
    public:
        friend class Deque;
        DIterator(long long ind, long long s, long long begin, Container* deque1) {
            deque = deque1;
            index = ind;
            size = s;
            begin_index = begin;
        }

        DIterator(const DIterator& other) {
            index = other.index;
            size = other.size;
            begin_index = other.begin_index;
            deque = other.deque;
        }

        DIterator& operator= (const DIterator & other) {
            index = other.index;
            size = other.size;
            begin_index = other.begin_index;
            deque = other.deque;
            return *this;
        }

        DIterator& operator++() {
            (*deque).inc(index, 1);
            return *this;
        }

        DIterator& operator--() {
            (*deque).dec(index, 1);
            return *this;
        }

        const DIterator operator++(int) {
            DIterator tmp = *this;
            (*deque).inc(index, 1);
            return tmp;
        }

        const DIterator operator--(int) {
            DIterator tmp = *this;
            (*deque).dec(index, 1);
            return tmp;
        }

        DIterator& operator+=(const long long& n) {
            (*deque).inc(index, n);
            return  *this;
        }
        DIterator& operator-=(const long long& n) {
            (*deque).dec(index, n);
            return  *this;
        }
        DIterator operator+(const long long& n) const {
            DIterator tmp = *this;
            return tmp += n;
        }

        DIterator operator-(const long long& n) const {
            DIterator tmp = *this;
            return tmp -= n;
        }

        long long operator-(const DIterator& other) const {
            if(index == begin_index && other.index == begin_index)
                return  0;
            if(index == begin_index) {
                if (other.index > index)
                    return index - other.index;
                else
                    return index - size - other.index;
            }
            if(other.index == begin_index) {
                if (other.index < index)
                    return index - other.index;
                else
                    return index + size - other.index;
            }
            if ((index < begin_index  && other.index < begin_index ) ||
                (index > begin_index  && other.index > begin_index))
                return index - other.index;
            else
                return index < begin_index  ?
                       index + size - other.index :
                       index - size - other.index;
        }

        bool operator<(const DIterator &other) const {
            return *this - other < 0;
        }

        bool operator>(const DIterator &other) const {
            return *this - other > 0;
        }

        bool operator==(const DIterator &other) const {
            return *this - other == 0;
        }

        bool operator<=(const DIterator &other) const {
            return !operator>(other);
        }

        bool operator>=(const DIterator &other) const {
            return !operator<(other);
        }

        bool operator!=(const DIterator &other) const {
            return !operator==(other);
        }

        ValueType& operator[] (const long long& n) {
            return (*deque).buffer[(*deque).plus(index, n)];
        }
        ValueType& operator* () const{
            return (*deque).buffer[index];
        }
        ValueType* operator-> () const{
            return &**this;
        }

    private:
        Container* deque;
        long long index;
        long long size;
        long long begin_index;

    };


    typedef DIterator<T, const Deque<T>> iterator;
    typedef DIterator<const T, const Deque<T>> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;




    friend class DItertor;








    Deque() {
        buffer_size = DEFAUL_BUFFER_SIZE;
        current_size = 0;
        buffer_begin = buffer_end = 0;
        buffer = new T[buffer_size];
    }

    Deque(const Deque& other) {
        buffer_size = other.buffer_size;
        current_size = other.current_size;
        buffer_begin = 0;
        buffer_end = current_size;
        buffer = new T[buffer_size];
        for (int i = 0; i< current_size; ++i)
            (*this)[i] = other[i];
    }

    ~Deque() {
        delete[] buffer;
    }

    Deque &operator= (const Deque& other) {
        if (this == &other)
            return *this;
        delete[] buffer;
        buffer_size = other.buffer_size;
        current_size = other.current_size;
        buffer_begin = 0;
        buffer_end = current_size;
        buffer = new T[buffer_size];
        for (int i = 0; i< current_size; ++i)
            (*this)[i] = other[i];
        return *this;
    }

    void push_back(const T& value) {
        (*this)[current_size] = value;
        inc(buffer_end, 1);
        ++current_size;
        if (current_size == buffer_size - 2)
            resize(buffer_size * 2);
    }

    void pop_back() {
        if (current_size == 0)
            return;
        --current_size;
        dec(buffer_end, 1);
        if (current_size == buffer_size / 4)
            resize(buffer_size / 2);
    }

    void push_front(const T& value) {
        if (empty()) {
            push_back(value);
            return;
        }
        dec(buffer_begin, 1);
        (*this)[0] = value;
        ++current_size;
        if (current_size == buffer_size - 2)
            resize(buffer_size * 2);
    }

    void pop_front() {
        if (current_size == 0)
            return;
        --current_size;
        inc(buffer_begin, 1);
        if (current_size == buffer_size / 4)
            resize(buffer_size / 2);
    }
    void clear() {
        current_size = 0;
        buffer_end = buffer_begin = 0;
    }
    bool empty() const {
        return current_size == 0;
    }

    long long size() const {
        return current_size;
    }

    T& operator[](long long index) {
        return this->buffer[plus(buffer_begin, index)];
    }

    T& operator[](long long index) const {
        return this->buffer[plus(buffer_begin, index)];
    }

    T& back() {
        return (*this)[current_size - 1];
    }
    T& back()const {
        return (*this)[current_size - 1];
    }

    T& front() {
        return (*this)[0];
    }
    T& front()const {
        return (*this)[0];
    }


    iterator begin() {
        return iterator(buffer_begin, buffer_size, buffer_begin, this);
    }
    const_iterator begin() const {
        return const_iterator(buffer_begin, buffer_size, buffer_begin, this);
    }
    const_iterator cbegin() const {
        return const_iterator(buffer_begin, buffer_size, buffer_begin, this);
    }
    iterator end() {
        return iterator(buffer_end, buffer_size, buffer_begin, this);
    }
    const_iterator end() const {
        return const_iterator(buffer_end, buffer_size, buffer_begin, this);
    }
    const_iterator cend() const {
        return const_iterator(buffer_end, buffer_size, buffer_begin, this);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(cend());
    }
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(cbegin());
    }
    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }



private:
    void resize(long long size) {
        if (size < buffer_size && buffer_size == DEFAUL_BUFFER_SIZE)
            return;
        T* tmpBuffer = new T[size];

        for (long long i = 0; i< current_size; ++i) {
            tmpBuffer[i] = (*this)[i];
        }
        delete[] buffer;
        buffer = tmpBuffer;
        buffer_begin = 0;
        buffer_end = current_size;
        buffer_size = size;
    }
    long long plus(const long long& i, const long long& n) const {
        long long tmp = i;
        inc(tmp, n);
        return tmp;
    }
    long long minus(const long long& i, const long long& n) const {
        return plus(i, -n);

    }
    void inc(long long& i, const long long& n) const {
        i = ((i + n + buffer_size) % buffer_size );
    }
    void dec(long long& i, const long long& n) const {
        inc(i, -n);
    }

    enum {
        DEFAUL_BUFFER_SIZE = 8
    };
    long long buffer_size;
    long long current_size;
    long long buffer_begin;
    long long buffer_end;
    T* buffer;
};