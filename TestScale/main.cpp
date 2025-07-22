#include "Command.h"

int main() {
    setlocale(LC_ALL, "RU");
    srand(static_cast<unsigned int>(time(nullptr)));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Init_Game();
    Start_Game();
    
}