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
#include <queue>

using namespace std;

/** 时间复杂度：
 最好：nlogn
 每次数据都平均分成两部分，得到一个完全二叉树，假设二叉树中有n个数据，那么树的深度为log2n+1，此时时间复杂度为nlogn
 最坏：n^2
 这个树仅有左子树或右子树，比较次数为(n-1)+(n-2)+...+1=n^2
 
 空间复杂度：O(n)
 */
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

//获取第K小数，快速排序法，时间复杂度O(n)
void findKth(vector<int> &num, int begin, int end, int K){
    if(begin > end) return;
    int temp = num[begin];
    int i = begin;
    int j = end;
    while (i < j) {
        while (i<j && temp <= num[j]) {
            j--;
        }
        num[i] = num[j];
        num[j] = temp;
        while (i<j && temp >= num[i]) {
            i++;
        }
        num[j] = num[i];
        num[i] = temp;
    }
    //如果碰到K则返回，没必要再排序
    if(j == K) return;
    //如果K在左边，则只排序左边
    if(K < j) findKth(num, begin, j-1, K);
    //如果K在右边，则只排序右边
    if(K > j) findKth(num, j+1, end, K);
}

//获取最K小数，最大堆法，时间复杂度O(nlogn)
int findKth(vector<int> &num, int K){
    priority_queue<int, vector<int>, less<int>> Q;
    for(auto x : num){
        if(Q.size() < K) Q.push(x);
        else{
            if(Q.top() > x){
                Q.pop();
                Q.push(x);
            }
        }
    }
    return Q.top();
}

#endif /* QuickSort_hpp */
