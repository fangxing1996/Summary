# linux线程互斥量pthread_mutex_t使用简介

## 为什么使用线程锁？

​		在多线程应用程序中，当多个线程共享相同的内存时，如同时访问一个变量时，需要确保每个线程看到一致的数据视图，即保证所有线程对数据的修改是一致的。

​		当然，如下两种情况不存在不一致的问题：

`1、每个线程使用的变量都是其他线程不会读取和修改的`

`2、变量是只读的`

​		当一个线程在修改变量的值时，其他线程在读取这个变量时可能会得到一个不一致的值。一个典型的例子是，在一个多线程程序中，两个或者两个以上的的线程对同一个变量i执行i++操作，结果得到的值并不如顺序执行所预期的那样。这就是线程间不同步的一个例子，所谓线程间不同步，可粗略理解为`多个线程的执行顺序不确定`。

​		可以用程序修改变量值所经历的3个步骤解释这个现象：

`1、从内存单元读入寄存器`

`2、在寄存器中操作变量（加/减）`

`3、把新值写回到内存单元`

​		不能预期以上3个步骤在一个总线周期内完成，所以也就不能指望多线程程序如预期那样运行。

## 实验

`main.cpp`代码如下：

```c++
//main.cpp
#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

int tickets = 20;//共享资源，票的数量为20


void* threadFunc(void* arg){
    int tmp = *((int*)arg);
    cout << tmp << "th" << endl;

    while(tickets > 0){
        cout << tickets << endl;
        tickets--;
        sleep(0.5);
    }
    return NULL;
}


int main(){
    int num = 3;
    pthread_t pid[num];
    int arg[num];
    for(int i = 0; i < num; i++){
        arg[i] = i;
        pthread_create(&pid[i], NULL, threadFunc, (void*)&arg[i]);//创建3个线程卖票
    }
    sleep(5);
    for(int i = 0; i < num; i++){
        pthread_join(pid[i], NULL);
    }
    return 0;
}
```

​	输入`g++ main.cpp -lpthread -o main`编译源程序，输入`./main`执行，输出结果如下：

![Image text](http://r.photo.store.qq.com/psc?/V52JW28g11pzg042wNGX2SRazK149khx/45NBuzDIW489QBoVep5mcbmPsNby*32MnAZdBDgq17KOw2l99aokkaGD4IwxQ4E4kyUhN0AA1xNgvqOuYcJ*oqgZt0wTJ5avtSX2y0P2s5Y!/r)

​	由于用于卖票的3个线程对共享资源`tickets`的访问没有实现互斥，导致第20张票卖出去3次，重复卖票，20张票，卖出去22张，这显示是错误的。

## 开始使用线程锁

### 互斥量

​		多线程程序中可能存在数据不一致的情况，那么如何保证数据一致呢？可以考虑同意时间只有一个线程访问数据。互斥量（mutex）就是一把锁。

​		多个线程只有一把锁和一个钥匙，谁上的锁就只有谁能开锁。当一个线程要访问一个共享变量时，先用锁把变量锁住，然后再操作，操作完了之后再释放掉锁，完成。

​		当另外一个线程也要访问这个变量时，发现这个变量被锁住了，无法访问，它就会一直等待，直到锁没了，它再给这个变量上个锁，然后使用，使用完了释放锁，以此类推。这样，即使有多个线程同时访问这个变量，也好像对这个变量的操作是顺序进行的。

​		互斥量使用特定的数据类型为`pthread_mutex_t`，是一个结构体。

​		**1、创建互斥量**

```
pthread_mutex_t mtx;//创建互斥量mtx
```

​		**2、在使用互斥量之前，先要将互斥量初始化**

​		有2种方法进行初始化，动态方式和静态方式：

​		（1）动态方式——调用`pthread_mutex_init`进行初始化

```c++
#include <pthread.h>//头文件

int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);//初始化互斥量
int pthread_mutex_destroy(pthread_mutex_t *mutex);//销毁互斥量
```

​		`pthread_mutexattr_t`为互斥锁属性类型，一般设置为`NULL`。在使用完互斥锁后，用`pthread_mutex_destory()`销毁互斥锁。

​		（2）静态方式——利用将宏`PTHREAD_MUTEX_INITIALIZER`赋给互斥量

```c++
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER
```

​		**3、锁操作**

​		锁操作包括`加锁pthread_mutex_lock()、解锁pthread_mutex_unlock()、尝试加锁pthread_mutex_trylock`。

```c++
int pthread_mutex_lock(pthread_mutex_t *mutex);//加锁

int pthread_mutex_unlock(pthread_mutex_t *mutex);//解锁

int pthread_mutex_trylock(pthread_mutex_t *mutex);//尝试加锁
```

## 子进程对互斥锁状态的继承

`main.cpp`代码如下：

```c++
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
using namespace std;
int main(){
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);
    pid_t pt;
    pthread_mutex_lock(&mtx);
    pt = fork();
    if(pt > 0){
        cout << "Father process" << endl;
        sleep(4);
        pthread_mutex_unlock(&mtx);
        sleep(4);
        pthread_mutex_destroy(&mtx);
        
    }
    else if(pt == 0){
        cout << "Child process" << endl;
        pthread_mutex_lock(&mtx);//子进程继承了锁locked状态，陷入死锁
        cout << "lock successful" << endl;
    }
    else{
        perror("fork error");
    }
    return 0;
}
```

​	子进程继承了互斥锁`mtx`被锁住的状态，尝试对`mtx`加锁时陷入死锁，输出结果如下：

![Image text](http://r.photo.store.qq.com/psc?/V52JW28g11pzg042wNGX2SRazK149khx/45NBuzDIW489QBoVep5mcSB23ir7TGn6iszNIJ8CN6autZ14DE8JxSVdtcAiVsdj6AaQZdPjQQrT0JEtM7k8Dphl.lnpnpqLaLgzjO.2r6k!/r)

​	子进程陷入死锁，即使在父进程睡眠4秒后释放锁，子进程还是不能进行加锁操作，因为fork之后子进程对父进程的动作是不可见的，因此子进程陷入了永远的阻塞状态。
