//
//  main.cpp
//  MemoryPool
//
//  Created by 张淑慧 on 2022/7/17.
//

#include <iostream>
#include "simple_segregated_storage.hpp"

void test_simple_segregated_storage()
{
    simple_segregated_storage<int> ss;
    void *block = new int(8);
    ss.add_block(block, 8, 4);
    void *ptr = block;
    while (ptr) {
        void *pt = ss.next_of(ptr);
        cout<<pt<<endl;;
        pt = ptr;
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
