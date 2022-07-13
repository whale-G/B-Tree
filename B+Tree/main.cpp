#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include "D:\code\cpp_workspace\cpp_test\B+Tree\B_Plus_Tree.h"
#include "D:\code\cpp_workspace\cpp_test\B+Tree\B+Tree.cpp"

using namespace std;

int main(){
    BPTree B;
    string str;
    //int num;
    
    int num=0;
    while(num != -1){
        cout<<"请输入一个数字，输入-1结束输入："<<endl;
        cin>>num;
        if(num == -1) break;
        B.insert(num);
        cout<<"当前B+树为："<<endl;
        B.show();
    }
    
    /*
    int i=0;
    ifstream inFile;
    inFile.open("C:\\Users\\29822\\OneDrive\\桌面\\test_csv.csv");
    while(getline(inFile,str)){
        num = atoi(str.c_str());
        B.insert(num);
        cout<<i++<<endl;
        //B.show();
    }
    //B.show();
    cout<<"OK"<<endl;
    */

    system("pause");
    return 0;
}