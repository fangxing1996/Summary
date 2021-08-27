#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {

    struct stat stat1 ;
    if ( argc != 2 ){

        printf("usage: ftok < pathname >" ) ;
        exit(1) ;
    }
    stat(argv[1], &stat1) ;
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x587 )) ;
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x118 )) ;
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x22)) ;
    printf("st_dev:%lx, st_ino:%lx, key:%x\n",  \
        (unsigned long)stat1.st_dev, (unsigned long)stat1.st_ino , ftok(argv[1], 0x33)) ;
    /*
        由输出结果可以观察到，函数ftok返回的32bit的key值由3个部分先后组成：proj_id的低8bits，
        st_dev的低8bits以及st_ino的低位16bits。
    */
    exit(0) ;
}
