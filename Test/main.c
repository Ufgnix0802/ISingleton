#include "Aclass.h"
int main111() {
    //Aclass a;  err
    Aclass::Ins().a = 5;
    std::cout << Aclass::Ins().a << std::endl;
    return 0;
}