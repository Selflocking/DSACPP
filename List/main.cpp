//
// Created by yunshu on 22-5-27.
//
#include"List.h"
#include<iostream>

List<int> L;

int main() {
    auto p = L.first();
    L.insert(p, 4);
    std::cout << p->succ->data;
    return 0;
}