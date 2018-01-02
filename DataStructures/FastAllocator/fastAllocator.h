#include <set>
#include <vector>
#include "fixedAllocator.h"
using std::set;
using std::vector;
struct SegmentForGettingTheBigger{
    void * ptr;
    size_t size;
    SegmentForGettingTheBigger(){
        ptr = nullptr;
        size = 0;
    }

    SegmentForGettingTheBigger(void* p, size_t s){
        ptr = p;
        size = s;
    }
};

struct SegmentForMerging{
    void * ptr;
    size_t size;
    SegmentForMerging(){
        ptr = nullptr;
        size = 0;
    }

    SegmentForMerging(void* p, size_t s){
        ptr = p;
        size = s;
    }
};
bool operator<(SegmentForGettingTheBigger f, SegmentForGettingTheBigger l){
    return f.size == l.size ? f.ptr < l.ptr : f.size > l.size;
}

bool operator<(SegmentForMerging f, SegmentForMerging l){
    return f.ptr < l.ptr;
}

template <typename T>
class FastAllocator{
public:
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T        value_type;

    FastAllocator(){
        value_size = sizeof(value_type);
        size = DEFAULTSIZE * value_size;
    }

    ~FastAllocator(){
        while(!allBlocks.empty()){
            void *ptr = allBlocks.back();
            allBlocks.pop_back();
            free(ptr);
        }
    }

    FastAllocator(const FastAllocator& other){}

    template<typename X, typename ... Args>
    void construct(X *ptr, Args &&... args) {
        *ptr = X(std::forward<Args>(args)...);
    }

    template<typename X>
    void destroy(X *ptr) {
        ptr->~X();
    }

    pointer allocate(size_t amount){
        size_t requiredSize = amount * sizeof(value_type);
        switch (requiredSize) {
            case 0:
                return nullptr;
            case sizeof(value_type):
                return static_cast<pointer >(allocator1.allocate());
            case 2*sizeof(value_type):
                return static_cast<pointer >(allocator2.allocate());
            case 4*sizeof(value_type):
                return static_cast<pointer >(allocator4.allocate());
            case 8*sizeof(value_type):
                return static_cast<pointer >(allocator8.allocate());
            default:
                return getBlock(requiredSize);
        }
    }

    void deallocate(T *ptr, size_t amount) {
        size_t requiredSize = amount * value_size;
        switch (requiredSize) {
            case 0:
                return;
            case sizeof(value_type):
                allocator1.deallocate(ptr);
                break;
            case 2*sizeof(value_type):
                allocator2.deallocate(ptr);
            case 4*sizeof(value_type):
                allocator4.deallocate(ptr);
                break;
            case 8*sizeof(value_type):
                allocator8.deallocate(ptr);
                break;
            default:
                deleteBlock(ptr, requiredSize);
                break;
        }
    }

private:
    enum{
        DEFAULTSIZE = 16
    };
   size_t value_size;
    FixedAllocator<sizeof(value_type)> allocator1;
    FixedAllocator<2*sizeof(value_type)> allocator2;
    FixedAllocator<4*sizeof(value_type)> allocator4;
    FixedAllocator<8*sizeof(value_type)> allocator8;

    set<SegmentForMerging> freeBlocksForMerging;
    set<SegmentForGettingTheBigger> freeBlockForGetting;
    vector<void*> allBlocks;
    size_t size;


    void fillWithBlocks(size_t requiredSize){
        if(size < requiredSize)
            size = requiredSize;

        size*=2;
        void* ptr = malloc(size);
        allBlocks.push_back(ptr);
        freeBlocksForMerging.insert(SegmentForMerging(ptr, size));
        freeBlockForGetting.insert(SegmentForGettingTheBigger(ptr, size));
    }

    void checkAndFillIfNeeded(size_t requiredSize){
        if(freeBlockForGetting.empty() || freeBlockForGetting.begin()->size < requiredSize)
            fillWithBlocks(requiredSize);
    }

    pointer getBlock(size_t requiredSize) {
        checkAndFillIfNeeded(requiredSize);
        set<SegmentForGettingTheBigger>::iterator i = freeBlockForGetting.begin();
        freeBlockForGetting.erase(*i);
        freeBlocksForMerging.erase(SegmentForMerging(i->ptr, i->size));
        void *ptr = i->ptr;
        size_t diff = i->size - requiredSize;
        switch (diff) {
            case 0:
                break;
            case 1:
                allocator1.deallocate(ptr + requiredSize);//add ptr to allocator1
                break;
            case 2 * sizeof(value_type):
                allocator2.deallocate(ptr);
            case 4 * sizeof(value_type):
                allocator1.deallocate(ptr + requiredSize);
                break;
            case 8 * sizeof(value_type):
                allocator1.deallocate(ptr + requiredSize);
                break;
            default:
                freeBlockForGetting.insert(SegmentForGettingTheBigger(ptr + requiredSize, diff));
                freeBlocksForMerging.insert(SegmentForMerging(ptr + requiredSize, diff));
                break;
        }

        return static_cast<pointer >(ptr);
    }

    void deleteBlock(pointer ptr, size_t requiredSize) {
        SegmentForGettingTheBigger s(ptr, requiredSize);
        set<SegmentForMerging>::iterator i = freeBlocksForMerging.find(SegmentForMerging((void*)ptr, requiredSize));
        if (i != freeBlocksForMerging.end()) {
            freeBlocksForMerging.erase(*i);
            freeBlockForGetting.erase(SegmentForGettingTheBigger(i->ptr, i->size));
            s.size += i->size;
            s.ptr = i->ptr;
       }

        i = freeBlocksForMerging.find(SegmentForMerging((void*)(ptr + requiredSize), requiredSize));
        if (i != freeBlocksForMerging.end() && (i->ptr == (void*)ptr)) {
            freeBlocksForMerging.erase(*i);
            freeBlockForGetting.erase(SegmentForGettingTheBigger(i->ptr, i->size));
            s.size += i->size;
        }

        freeBlockForGetting.insert(s);
        freeBlocksForMerging.insert(SegmentForMerging(s.ptr,s.size));

    }

};