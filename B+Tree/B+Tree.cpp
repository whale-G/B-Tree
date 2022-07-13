#include <iostream>
#include <queue>
#include "D:\code\cpp_workspace\cpp_test\B+Tree\B_Plus_Tree.h"

using namespace std;

int BPTree::insertVal(int x,Node* cursor){
    int i=0;
    //找到比需要插入的键值小的键值的位置（key数组中）
    while(x > cursor->key[i] && i < cursor->size) i++;
    //将key数组key[i]位置空出，其余向后挪一位
    for(int j = cursor->size; j>i; j--)
        cursor->key[j] = cursor->key[j-1];
    //key[i]处赋值为需要插入的结点的key值
    cursor->key[i] = x;
    //结点中key个数加1
    cursor->size++;

    //返回需要插入的位置
    return i;
}

void BPTree::insert(int x){
    //1、根结点为空时，创建一个根结点
    if(root == NULL){
        root = new Node;
        root->key[0] = x;
        root->IS_LEAF = true;
        root->size = 1;
        root->parent = NULL;
    }
    //2、根结点不为空时，找到需要插入的位置，同时记录插入结点的父结点
    else {
        Node* cursor = root;
        Node* parent = NULL; //此处的父结点初始化为NULL可以吗？2022-7-12

        //到叶子结点才停止，当根结点不是叶子结点时
        while(cursor->IS_LEAF == false){
            parent = cursor;
            for(int i=0 ; i < cursor->size ; i++){
                if(x < cursor->key[i]){
                    //parent->parent = cursor;
                    cursor = cursor->ptr[i];        //小于第一个key值，访问左子结点
                    break;
                }
                if(i == cursor->size - 1){
                    //parent->parent = cursor;
                    cursor = cursor->ptr[i+1];      //大于最后一个key值，访问右子结点
                    break;
                }
            }
        }
        //3、如果插入后结点满足key个数<MAX,可以直接插入
        if(cursor->size < MAX){
            insertVal(x,cursor);
            cursor->parent = parent;
            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size -1] = NULL;
        }
        //4、否则需要split()
        else split(x,parent,cursor);
    }
}

void BPTree::split(int x,Node* parent,Node* cursor){
    Node* LLeaf = new Node;
    Node* RLeaf = new Node;
    //插入key
    insertVal(x,cursor);
    //初始化左右叶子结点
    LLeaf->IS_LEAF = RLeaf->IS_LEAF = true;
    LLeaf->size = (MAX+1)/2;
    RLeaf->size = (MAX+1)-(MAX+1)/2;
    for(int i=0 ; i < MAX+1 ; i++) LLeaf->ptr[i] = cursor->ptr[i];
    //顺序访问指针,双向指针？ 2022-7-12
    LLeaf->ptr[LLeaf->size] = RLeaf;
    RLeaf->ptr[RLeaf->size] = LLeaf->ptr[MAX];
    LLeaf->ptr[MAX] = NULL;
    //给叶子结点插入key值    
    for(int i=0 ; i < LLeaf->size ; i++) 
        LLeaf->key[i]=cursor->key[i];
    for(int i=0,j=LLeaf->size ; i < RLeaf->size ; i++,j++) {
        RLeaf->key[i]=cursor->key[j];
    }
    //叶子结点拆分后，提上去为根结点
    if(cursor == root){
        Node* newRoot = new Node;
        newRoot->key[0] = RLeaf->key[0];
        newRoot->ptr[0] = LLeaf;
        newRoot->ptr[1] = RLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;             //根结点变为新的结点!  2022-7-11
        //delete cursor;              //释放掉之前的旧结点   2022-7-12
        LLeaf->parent = RLeaf->parent = newRoot;
    }
    //提上去不是根结点
    else insertInternal(RLeaf->key[0],parent,LLeaf,RLeaf);
}

void BPTree::insertInternal(int x,Node* cursor,Node* LLeaf,Node* RLeaf){
    //父结点的key值个数小于MAX,直接插入key
    if(cursor->size < MAX){
        int i = insertVal(x,cursor);        //为什么用auto? 2022-7-11
        for(int j=cursor->size ; j>i+1 ;j--)
            cursor->ptr[j] = cursor->ptr[j-1];
        cursor->ptr[i] = LLeaf;
        cursor->ptr[i+1] = RLeaf;
        LLeaf->parent = RLeaf->parent = cursor;     //新加的 2022-7-12
    }
    //父结点key值个数大于MAX,再次拆分
    else{
        //建立新左右子结点
        Node* newLchild = new Node;
        Node* newRchild = new Node;
        //这个是什么？  2022-7-12
        Node* virtualPtr[MAX+2];
        for(int i=0; i < MAX+1 ;i++)
            virtualPtr[i] = cursor->ptr[i];
        int i=insertVal(x,cursor);

        for(int j=MAX+2; j>i+1 ;j--)
            virtualPtr[j-1] = virtualPtr[j-2];
        virtualPtr[i] = LLeaf;
        virtualPtr[i+1] = RLeaf;
        newLchild->IS_LEAF = newRchild->IS_LEAF = false;
        //注意和叶子结点有区别
        newLchild->size = (MAX+1)/2;
        newRchild->size = MAX-(MAX+1)/2;
        //key值插入
        for(int i=0; i < newLchild->size ;i++)
            newLchild->key[i] = cursor->key[i];
        for(int i=0,j=newLchild->size+1; i < newRchild->size ;i++,j++)//j=newLchild->size+1,,中间的数提上去了
            newRchild->key[i] = cursor->key[j];
        //结点分配
        for(int i=0; i < LLeaf->size+1 ; i++)
            newLchild->ptr[i] = virtualPtr[i];
        for(int i=0,j=LLeaf->size+1; i < RLeaf->size+1 ;i++,j++)
            newRchild->ptr[i] = virtualPtr[j];
        
        //再次拆分提到根结点
        if(cursor == root){
            Node* newRoot = new Node;
            newRoot->key[0] = cursor->key[newLchild->size];                                  
            newRoot->ptr[0] = newLchild;
            newRoot->ptr[1] = newRchild;
            newRoot->IS_LEAF = false;
            newRoot->size = 1;
            root = newRoot;
            newLchild->parent = newRchild->parent = newRoot;
            LLeaf->parent = newLchild;
            RLeaf->parent = newRchild;
            }
        //再次拆分没有到根结点，递归调用insertInternal()
        else {
            insertInternal(cursor->key[newLchild->size],cursor->parent,newLchild,newRchild);
        }
    }
}

void BPTree::show(){
    queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        int size_t = q.size();
        while(size_t--){
            auto t=q.front();
            for(int i=0; i < t->size+1 ;i++){
                if(!t->IS_LEAF)
                    q.push(t->ptr[i]);
            }
            for(int i=0; i < t->size ;i++)
                cout<<t->key[i]<<",";
            cout<<"   ";
            q.pop();
        }
        cout<<endl;
    }
}