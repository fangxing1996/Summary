#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "./Mutex.h"
using namespace std;
Mutex mut;

void* threadFunc(void* args){//线程函数
    Mutex::AutoLock atlock(mut); 
    cout << "starting thread " << (char*)args << endl;
    for(int i = 0; i < 100; i++){
        for(int j = 0; j <3000; j++)
            ;
        cout << (char*)args;
    }
    cout << endl;
    return NULL;
}

int main(){

    pthread_t ida, idb, idc;
    int x, y, z;
    char pa[2] = "a";
    char pb[2] = "b";
    char pc[2] = "c";
    x = pthread_create(&ida, NULL, threadFunc, (void*)pa);
    if(x != 0){
        cout << "create thread failed" << endl;
        return 0;
    }
    y = pthread_create(&idb, NULL, threadFunc, (void*)pb);
    if(y != 0){
        cout << "create thread failed" << endl;
        return 0;
    }
    z = pthread_create(&idc, NULL, threadFunc, (void*)pc);
    if(z != 0){
        cout << "create thread failed" << endl;
        return 0;
    }
    pthread_join(ida, NULL);
    pthread_join(idb, NULL);
    pthread_join(idc, NULL);
    return 0;
}