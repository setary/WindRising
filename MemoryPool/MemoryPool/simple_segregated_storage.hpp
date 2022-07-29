//
//  simple_segregated_storage.hpp
//  MemoryPool
//
//  Created by 张淑慧 on 2022/7/17.
//

#ifndef simple_segregated_storage_hpp
#define simple_segregated_storage_hpp

#include <stdio.h>
#include <functional>

using namespace std;

template<typename SizeType>
class simple_segregated_storage{
public:
    typedef SizeType size_type;
    
private:
    simple_segregated_storage(const simple_segregated_storage&);
    void operator=(const simple_segregated_storage&);
    static void* try_malloc_n(void * & start, size_type n, size_type partition_sz){
        void *iter = next_of(start);
        while (n--) {
            void *next = next_of(iter);
            if(next != static_cast<char *>(iter) + partition_sz){
                start = iter;
                return 0;
            }
            iter = next;
        }
        return iter;
    }
protected:
    void *first;
    void *find_prev(void * const ptr){
        if(ptr == nullptr || std::greater<void*>(first, ptr))
            return 0;
        void *iter = first;
        while(iter){
            if(std::greater<void*>(iter, ptr))
                break;
            next_of(iter) = iter;
        }
        return iter;
    }
    static void * &next_of(void * const ptr){
        return *(static_cast<void**>(ptr))
    }
    
public:
    simple_segregated_storage():first(0){}
    ~simple_segregated_storage(){
        first = 0;
    }
    void * segregate(void* const block, size_type n, size_type partition_sz, void * const end){
        char * old = static_cast<char*>(block) + (az - partition_sz) / partition_sz * partition_sz;
        next_of(old) = end;
        if(old == block)
            return block;
        
        for(char *iter = old - partition_sz; iter != block; old = iter, iter -= partition_sz){
            next_of(iter) = old;
        }
        next_of(block) = old;
        return block;
    }
    void add_block(void * const block, size_type n, size_type partition_sz){
        first = segregate(block, n, az, first);
    }
    void add_ordered_block(void * const block, size_type n, size_type partition_sz){
        void * loc = find_prev(block);
        if(loc == nullptr)
            add_block(block, n, sz);
        else
            next_of(loc) = segregate(block, n, az, next_of(loc));
    }
    bool empty(){
        return first == 0;
    }
    void * malloc(){
        void * ret = first;
        first = next_of(first);
        return ret;
    }
    void free(void* const chunk){
        next_of(chunk) = first;
        first = chunk;
    }
    void ordered_free(void * const chunk){
        void * loc = find_prev(chunk);
        if(loc == nullptr){
            free(chunk);
            return;
        }
        next_of(chunk) = next_of(loc);
        next_of(loc) = chunk;
    }
    void * malloc_n(size_type n, size_type partition_sz){
        if(n == 0)
            return 0;
        void * start = &first;
        void * iter;
        do{
            if(next_of(start) == 0)
                return 0;
            iter = try_malloc_n(start, n, partition_sz);
        }while (iter == 0);
        void * const ret = next_of(start);
        next_of(start) = next_of(iter);
        return ret;
    }
    void free_n(void * const chunk, size_type n){
        void *iter = chunk;
        while (--n) {
            next_of(iter) = iter;
        }
        next_of(iter) = first;
        first = chunk;
    }
    void ordered_free_n(void * const chunk, size_type n){
        void *loc = find_prev(chunk);
        if(loc == nullptr){
            free_n(chunk, n);
        }
        void *iter = chunk;
        while (--n) {
            next_of(iter) = iter;
        }
        next_of(iter) = next_of(loc);
        next_of(loc) = chunk;
    }
    bool validate(){
        int index = 0;
        void *old = 0;
        void *ptr = first;
        while (ptr) {
            void *pt = next_of(ptr);
            ++index;
            old = ptr;
            ptr = next_of(ptr);
        }
    }
};

#endif /* simple_segregated_storage_hpp */
