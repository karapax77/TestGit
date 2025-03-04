#include <iostream>
<<<<<<< HEAD
#include <stdlib.h>
#include <string.h>

class bublik
{
    public: 
        int age;
        char name[20];
=======

class bublik
{
    int age;
    char name[20];
>>>>>>> 0c1117198684b72fb9c90a25c71c951b3c3c4e0d
};

int main()
{
    char k[] = "chingis";
<<<<<<< HEAD
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
=======
    
>>>>>>> 0c1117198684b72fb9c90a25c71c951b3c3c4e0d
    return 0;
}