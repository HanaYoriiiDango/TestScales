#include "Command.h"

void Start_Game() {

    string temp; // переменная для ввода команд 

    while (Hero.life) {

        cout << "тут странности какие то происходят, все в огне \n";
        cout << "Go - для перемещения \n";
        cout << "Loc - вывести текущее положение игрока \n";
        cout << "Start - начать диалог с персонажем \n";
        cin >> temp; // ожидаем ввода одной из команд 

        if (temp == "Go") Command_Go(); // выводит все доступные миры для игрока и позволяет перемещаться по тем мирам что доступы игроку  
        if (temp == "Loc") cout << "Current loc: " << Hero.current_loc << endl; // выводим игроку его текущее положение
        if (temp == "Start") Start_dialog(); // начинаем диалог с персонажем
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    srand(static_cast<unsigned int>(time(nullptr)));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Init_Game();
    Start_Game();
    
}