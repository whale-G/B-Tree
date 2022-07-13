#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_

#include <iostream>
#include <queue>

using namespace std;
int MAX = 2;                    //单个结点最大出度 2022-7-11

//BP node
class Node{
private:
    bool IS_LEAF;               //IS_LEAF,是否为叶子结点
    int *key;                   //key,关键字（键值） 
    int size;                   //size,单个结点中key个数？2022-7-11,为啥没有初始化
    Node** ptr;                 //ptr,结点
    Node* parent;               //split时需要维护一个父结点指针

    friend class BPTree;
public:
    Node(): key(new int[MAX+1]),ptr(new Node* [MAX+1]),parent(NULL) {}      //以数组存key值和ptr
    ~Node();
};

//BP tree
class BPTree{
private:
    Node* root;                 //根结点

    void insertInternal(int,Node*,Node*,Node*);     //二次或多次拆分
    void split(int,Node*,Node*);                    //第一次拆分
    int insertVal(int,Node*);                       //插入key值并返回插入位置
public:
    BPTree(): root(NULL){}
    void insert(int x);                             //B+树插入操作
    void show();                                    //打印当前B+树
};

#endif