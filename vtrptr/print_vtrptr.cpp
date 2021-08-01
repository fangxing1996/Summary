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
