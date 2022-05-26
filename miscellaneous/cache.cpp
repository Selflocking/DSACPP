//
// Created by yunshu on 22-5-26.
//
/**
* @brief 常访问的数据可能会进cache，加快程序，所以有些复杂度高的反而可能更快
*/

/*
 * 考虑这样一个问题，一个数组中有n个数，是否只用O(1)的辅助空间就能讲整个数组像前移动k位
 */
#include<iostream>

int nums[20];

//暴力，每次挪一位，O(kn)
void shift(int *A, int n, int k);

//讲每一位向前移动k位
void jumpShift(int *A, int n, int k);

//翻转移动，很巧妙
void reverseShift(int *A, int n, int k);

void reverse(int *A, int n);

int main() {
    for (int i = 0; i < 20; ++i) nums[i] = i + 1;
//    shift(nums, 20, 6);
//    jumpShift(nums, 20, 6);
    reverseShift(nums, 20, 6);
    for (auto i: nums) std::cout << i << " ";
    return 0;
}

void shift(int *A, int n, int k) {
    for (int i = 0; i < k; ++i) {
        int t = A[0];
        for (int j = 1; j < n; ++j) A[j - 1] = A[j];
        A[n - 1] = t;
    }
}

void jumpShift(int *A, int n, int k) {

}

void reverseShift(int *A, int n, int k) {
    reverse(A, k);
    reverse(A + k, n - k);
    reverse(A, n);
}

void reverse(int *A, int n) {
    for (int i = 0; i < n / 2; ++i) {
        std::swap(A[i], A[n - i - 1]);
    }
}
