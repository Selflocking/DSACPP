//
// Created by yunshu on 2022/5/17.
//

#ifndef DSACPP_VECTOR_H
#define DSACPP_VECTOR_H

using Rank = int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template<typename T>
class Vector { //向量模板类
protected:
    Rank _size;
    Rank _capacity;
    T *_elem; //规模、容量、数据区
    void copyFrom(T const *A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
    void expand(); //空间不足时扩容
    void shrink(); //装填因子过小时压缩
    bool bubble(Rank lo, Rank hi); //扫描交换
    void bubbleSort(Rank lo, Rank hi); //起泡排序算法
    Rank maxItem(Rank lo, Rank hi); //选取最大元素
    void selectionSort(Rank lo, Rank hi); //选择排序算法
    void merge(Rank lo, Rank mi, Rank hi); //归并算法
    void mergeSort(Rank lo, Rank hi); //归并排序算法
    void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
    Rank partition(Rank lo, Rank hi); //轴点构造算法
    void quickSort(Rank lo, Rank hi); //快速排序算法
    void shellSort(Rank lo, Rank hi); //希尔排序算法
public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    } //s<=c
    Vector(T const *A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
    Vector(T const *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
    Vector(Vector<T> const &V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
    Vector(Vector<T> const &V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
    // 析构函数
    ~Vector() { delete[] _elem; } //释放内部空间
    // 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return !_size; } //判空
    Rank find(T const &e) const { return find(e, 0, _size); } //无序向量整体查找
    Rank find(T const &e, Rank lo, Rank hi) const; //无序向量区间查找
    Rank search(T const &e) const //有序向量整体查找
    { return (0 >= _size) ? -1 : search(e, 0, _size); }

    Rank search(T const &e, Rank lo, Rank hi) const; //有序向量区间查找
    // TODO：为什么有两个 可写访问接口，经过测试发现调用的都是第一个🤔
    T &operator[](Rank r); //重载下标操作符，可以类似于数组形式引用各元素
    const T &operator[](Rank r) const; //仅限于做右值的重载版本
    Vector<T> &operator=(Vector<T> const &); //重载赋值操作符，以便直接克隆向量
    T remove(Rank r); //删除秩为r的元素
    int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
    Rank insert(Rank r, T const &e); //插入元素
    Rank insert(T const &e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank lo, Rank hi); //对[lo, hi)排序
    void sort() { sort(0, _size); } //整体排序
    void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
    void unsort() { unsort(0, _size); } //整体置乱
    Rank deduplicate(); //无序去重
    Rank uniquify(); //有序去重
    // 遍历
    void traverse(void (* )(T &)); //遍历（使用函数指针，只读或局部性修改）
    template<typename VST>
    void traverse(VST &); //遍历（使用函数对象，可全局性修改）
};

template<typename T>
void Vector<T>::copyFrom(const T *A, Rank lo, Rank hi) {
    //预留一倍的空间
    //这是为了让装填因子不致于超过1，也不至于接近0
    //可是为什么要预留一倍空间，而不是0.618倍，3倍等等捏
    // P35,习惯使然吧，这样转移的时间消耗大概是log2
    _capacity = (hi - lo) >> 1;
    _elem = new T[_capacity];
    //注意是左开右闭
    _size = 0;
    while (lo < hi) _elem[_size++] = A[lo++];
}

template<typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
    /*
     * 同样是clang-tidy提示，这样是为了防止自我赋值，太好用辣趴
     * https://clang.llvm.org/extra/clang-tidy/checks/bugprone-unhandled-self-assignment.html
     * 具体原因和解决办法参考上述链接
     *
     */
    if (this == &other) return *this;
    /*
     * if(_elem) delete [] _elem;
     * 本来是这样写的，但是clang-tidy提示
     * Clang-Tidy: 'if' statement is unnecessary; deleting null pointer has no effect
     */
    delete[] _elem;
    copyFrom(other, 0, other.size());
    return *this;
}

template<typename T>
void Vector<T>::expand() {
    //判断一次防止恶意扩充，在其他函数调用expand()时能少写个if
    if (_size < _capacity) return;
    //TODO:这句话不知道有什么用,可能是有时容量小于默认容量？
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
    T *_old_elem = _elem;
    //本来以为自己对左右移已经够熟悉了，没想到还是会用错
    //还是直接用乘除更清晰
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; ++i) {
        _elem[i] = _old_elem[i];
    }
    delete[] _old_elem;
}

template<typename T>
void Vector<T>::shrink() {
    //TODO:这句话还是不知道是什么意思
    if (_capacity < DEFAULT_CAPACITY << 1) return;
    //书上是 if(_size<<2 > _capacity) 感觉不如下面的清楚
    if (_size > _capacity >> 2) return;
    T *_old_elem = _elem;
    _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; ++i) _elem[i] = _old_elem[i];
    delete[] _old_elem;
}

template<typename T>
const T &Vector<T>::operator[](Rank r) const {
    //if(r<0||r>_size) throw
    return _elem[r];
}

template<typename T>
T &Vector<T>::operator[](Rank r) {
    return _elem[r];
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {
    //假设rand()会产生理想的随机数
    srand(time(nullptr));
    T *V = _elem + lo;
    for (Rank i = hi - lo; i > 0; --i) {
        std::swap(V[i - 1], V[rand() % i]);
    }
    /*
     * 感觉不如
     * for(int i = lo;i<hi;++i)
     *     swap(_elem[i],_elem[rand()%(hi-lo)+lo]);
     * 清楚
     */
}

template<typename T>
Rank Vector<T>::find(const T &e, Rank lo, Rank hi) const {
    //如果有多个匹配值，返回其中秩最大的，如果未找到，返回lo-1
    while (lo < hi-- && _elem[hi] != e);
    return hi;
    /*
     * 这样也不错，返回第一个找到的
     * while (++lo < hi && _elem[lo] != e);
     * return lo;
     */
}

template<typename T>
Rank Vector<T>::insert(Rank r, const T &e) {
    expand(); //检查是否需要扩容
    for (int i = _size; i > r; --i) _elem[i] = _elem[i - 1];
    _elem[r] = e;
    ++_size;
    return r;
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
    //这段代码写的很好
    if (hi == lo) return 0;
    while (hi < _size) _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

template<typename T>
T Vector<T>::remove(Rank r) {
    T *t = _elem[r];
    remove(r, r + 1);
    return t;
}

template<typename T>
Rank Vector<T>::deduplicate() {
    //高效删除无序向量中的重复元素
    int _old_size = _size; //记录原规模
    Rank i = 1;
    while (i < _size) {
        //find()查找失败返回的是lo-1，此时lo == 0; 查找失败函数返回值是-1
        if (find(_elem[i], 0, i) < 0) ++i;
        else remove(i);
    }
    return _old_size - _size;
    /*
     * 去重算法的时间消耗主要在find()和remove()
     * 我觉得改成从后向前去重时间效率能进一步提高
     * 改成从后向前后find()的消耗不变，但是remove的消耗会大大减少 (应该吧，
     * TODO: 我不会证明 😭
     * int _old_size = _size;
     * Rank i = _size-1;
     * while(i >= 0){
     *     if(find(_elem[i],0,i) < 0) ++i;
     *     else remove(i)
     * }
     * return _old_size - _size;
     */
}

//TODO: 暂时看不懂
template<typename T>
void Vector<T>::traverse(void (*visit)(T &)) {
    for (int i = 0; i < _size; ++i) visit(_elem[i]);
}

template<typename T>
template<typename VST>
void Vector<T>::traverse(VST &visit) {
    for (int i = 0; i < _size; ++i) visit(_elem[i]);
}

//Vector
#endif //DSACPP_VECTOR_H
