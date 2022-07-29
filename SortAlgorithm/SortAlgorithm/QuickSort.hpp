//
//  QuickSort.hpp
//  SortAlgorithm
//
//  Created by 张淑慧 on 2022/7/29.
//

#ifndef QuickSort_hpp
#define QuickSort_hpp

#include <stdio.h>
#include <vector>

using namespace std;

void quick_sort(vector<int> &nums, int begin, int end){
    if(end < begin) return;
    //取第一个作为基准数
    int temp = nums[begin];
    int i = begin;
    int j = end;
    while (i < j) {
        //每次先移动j，小于基准数的与基准数交换
        while(i < j && temp <= nums[j]){
            j--;
        }
        nums[i] = nums[j];
        nums[j] = temp;
        //再移动i，大于基准数的与基准数交换
        while (i < j && temp >= nums[i]) {
            i++;
        }
        nums[j] = nums[i];
        nums[i] = temp;
    }
    quick_sort(nums, begin, j-1);
    quick_sort(nums, j+1, end);
}

#endif /* QuickSort_hpp */
