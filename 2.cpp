#include <iostream>
#include <stdlib.h>
#include <string.h>

class bublik
{
    public: 
        int age;
        char name[20];
};

int main()
{
    char k[] = "chingis";
    bublik* bubliki = new bublik[5];
    for(int i = 0; i < 5; i++){
        (bubliki + i)->age = i;
        strcpy((bubliki+i)->name, k);
    }
    std::cout << "Объект записан" << std::endl;
    for(int i = 0; i < 5; i++){
        std::cout << (bubliki + i)->age << std::endl;
        std::cout << (bubliki+i)->name << std::endl;
    }
    delete[] bubliki;
    std::cout << "Массив объектов удален";
    return 0;
}