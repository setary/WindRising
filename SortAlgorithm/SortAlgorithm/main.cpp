//
//  main.cpp
//  SortAlgorithm
//
//  Created by 张淑慧 on 2022/7/29.
//

#include <iostream>
#include "QuickSort.hpp"

using namespace std;

void test_quick_sort(){
    vector<int> vec{6, 1, 2, 7, 9, 3, 4, 5, 10, 8};
    quick_sort(vec, 0, (int)vec.size()-1);
    for(int i = 0; i < vec.size(); i++){
        cout<<vec[i]<<' ';
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    test_quick_sort();
    std::cout << "Hello, World!\n";
    return 0;
}
