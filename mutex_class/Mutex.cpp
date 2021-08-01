#include "./Mutex.h"
#include <pthread.h>
Mutex:: Mutex() {
        mtxPtr = new pthread_mutex_t;
        pthread_mutex_init(mtxPtr, NULL);
}
Mutex:: ~Mutex(){
        pthread_mutex_destroy(mtxPtr);
}
int Mutex:: Mutex_lock() {
        pthread_mutex_lock(mtxPtr);
        return 0;
}
int Mutex:: Mutex_unlock() {
        pthread_mutex_unlock(mtxPtr);
        return 0;
}
/*Mutex的嵌套类Autolock初始化时调用构造函数抢占锁，
退出程序块时调用析构函数释放线程锁
*/
Mutex::AutoLock:: AutoLock(Mutex& mtxp):refmtx(mtxp) {
    refmtx.Mutex_lock();
}
Mutex::AutoLock::~AutoLock() {
    refmtx.Mutex_unlock();
}