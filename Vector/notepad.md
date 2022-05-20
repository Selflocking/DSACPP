# Vector

## 查找

考虑这样的代码

```c++
//二分查找A
template<typename T>
static Rank binSearch(T *A, const T &e, Rank lo, Rank hi) {
while (lo < hi) {
Rank mi = lo + (hi - lo) / 2; //防止爆int
if (e < A[mi]) hi = mi;
else if (e > A[mi]) lo = mi + 1;
else return mi;
}
return -1;
} 
```

发现问题没有，

1. 当传进来的向量中有多个重复的数时，返回的秩时“随机”的
2. 需要1次比较跳到左半边，需要2次比较跳到右半边，所以其实左右比较长度不一样长

这个二分算法的时间复杂度为O($1.5log_2n$),有一个常系数1.5

下面的二分就完美的解决了这两个问题

```c++
template<typename T>
static Rank binSearch(T *A, const T &e, Rank lo, Rank hi) {
    while (lo < hi) {
        Rank mi = lo + (hi - lo) / 2;
        if (e < A[mi]) hi = mi;
        else lo = mi + 1;
    }
    return lo - 1;
}
```
