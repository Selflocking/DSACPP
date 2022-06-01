//
// Created by yunshu on 22-5-25.
//

#ifndef DSACPP_LISTNODE_H
#define DSACPP_LISTNODE_H


using Rank = int; //秩
template<typename T>
struct ListNode;
template<typename T> using ListNodePosi = ListNode<T> *; //列表节点位置
template<typename T>
struct ListNode { //列表节点模板类（以双向链表形式实现）
    // 成员
    T data;
    ListNodePosi<T> pred;
    ListNodePosi<T> succ; //数值、前驱、后继
    // 构造函数
    ListNode() {} //针对header和trailer的构造
    ListNode(T e, ListNodePosi<T> p = nullptr, ListNodePosi<T> s = nullptr)
            : data(e), pred(p), succ(s) {} //默认构造器
    // 操作接口
    ListNodePosi<T> insertAsPred(T const &e); //紧靠当前节点之前插入新节点
    ListNodePosi<T> insertAsSucc(T const &e); //紧随当前节点之后插入新节点
};

template<typename T>
ListNodePosi<T> ListNode<T>::insertAsPred(const T &e) {
    //构建一个新节点.
    auto t = new ListNode(e, pred, this);
    //将此节点的前一个的后继设为t
    pred->succ = t;
    //将此节点的前指针指向t
    pred = t;
    return t;
}

template<typename T>
ListNodePosi<T> ListNode<T>::insertAsSucc(const T &e) {
    auto t = new ListNode(e, this, succ);
    t->pred = this;
    this->succ = t;
    return t;
}

#endif //DSACPP_LISTNODE_H
