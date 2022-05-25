//
// Created by yunshu on 2022/5/17.
//
#include<iostream>
#include"Vector.h"

int main() {
    Vector<int> v;
    for (int i = 20; i >= 1; --i) {
        v.insert(i);
    }
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    v.unsort();
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    v.sort();
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}

