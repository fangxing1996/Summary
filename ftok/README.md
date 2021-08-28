# ftok()函数产生键值的原理

## 概要

```c++
#include <sys/types.h>
#include <sys/ipc.h>

key_t ftok(const char *pathname, int proj_id);
```

## 实验

​	`main.cpp`代码如下：

```c++
//main.cpp
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {

    struct stat stat1;
    if ( argc != 2 ){

        printf("usage: ftok < pathname >" );
        exit(1);
    }
    stat(argv[1], &stat1);
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x587 ));
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x118 ));
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x22));
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x33));
    /*
        由输出结果可以观察到，函数ftok返回的32bit的key值由3个部分先后组成：proj_id的低8bits，
        st_dev的低8bits以及st_ino的低位16bits。
    */
    exit(0);
}
```

​	在同一目录下创建`demo.txt`文本文件，内容无所谓，可以写也可以不写，输入`g++ main.cpp -o main`编译源文件，生成可执行程序`main`，输入`./main demo.txt`执行程序，输入结果如下：

![Image text](http://r.photo.store.qq.com/psc?/V52JW28g11pzg042wNGX2SRazK149khx/45NBuzDIW489QBoVep5mcbs93NZHfRxcSV43ABQ9jZZiydmQfrWtCIJwUxPRZlRaFKNCqQBn1VdbWUcja.TpdAyFE9h1L1svNAAzf.WpSXk!/r)

​	由输出结果可以观察到，函数`ftok`返回的32bit的key值由3个部分先后组成：proj_id的低8bits， st_dev的低位8bits以及st_ino的低位16bits。