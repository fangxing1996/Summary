# 打印虚函数表首地址

## 实验

​	`print_vtrptr.cpp`代码如下：

```c++
//print_vtrptr.cpp
#include<iostream>
#include<stdio.h>
using namespace std;

class Base {
    public:
        virtual int printText(){
            cout << "Base class!" << endl;
            return 0;
        }
};

class Deriver: public Base {
    public :
        virtual int printAge(){
            cout << "Deriver class!" << endl;
            return 0;
        }
};

int main(){
    Base be1;
    Base be2;
    cout << "Base class virtual table address: " << *((int*)&be1) << " " << *((int*)&be2) << endl;
    Deriver de1;
    Deriver de2;
    cout << "Deriver class virtual table address: " << *((int*)&de1) << " " << *((int*)&de2) << endl;
    return 0;
}
```

​	输入`g++ main.cpp -o main`编译源文件，生成可执行程序`main`，输入`./main`执行程序，输入结果如下：

![Image text](http://r.photo.store.qq.com/psc?/V52JW28g11pzg042wNGX2SRazK149khx/45NBuzDIW489QBoVep5mcbs93NZHfRxcSV43ABQ9jZaxR2yWPYg*KJVuWHnmRC9Wcu5lkWpiSt1Rh3U.940opaVT7MkxSe4zfYkqU0CXhuY!/r)

​	由输出结果可以观察到，基类`Base`实例化的对象的虚函数表的首地址均为4215484，派生类`Deriver`实例化的对象的虚函数表的首地址均为4215496。