#pragma once

#include "object.h"

Worlds_Num get_opposite_world(Worlds_Num world) { // принимает одну эмоцию а возвращает противоположную

    switch (world) {
    case (SADNESS): return JOY;
    case (JOY): return SADNESS;
    case (FEAR): return CALM;
    case (CALM): return FEAR;
    case (ANGER): return POWER;
    case (POWER): return ANGER;

    }
}

void Check_movement() {

    for (int i = 0; i < 6; i++) {     // Проверяем все эмоции на достижение предела
        Worlds_Num emotion = static_cast<Worlds_Num>(i);
        int emotion_value = Hero.emotions[i];

        if ((emotion_value <= 0 || emotion_value >= 100) && !Worlds[i].is_locked) {// Если эмоция достигла предела и её мир еще не заблокирован

            Worlds[i].is_locked = true; // 1. Блокируем мир этой эмоции

            Worlds_Num opposite_emotion = get_opposite_world(emotion); // 2. Блокируем противоположный мир
            Worlds[static_cast<int>(opposite_emotion)].is_locked = true;

            Hero.emotions[emotion] = 50; // 3. Сбрасываем эмоции
            Hero.emotions[opposite_emotion] = 50;

            if (Hero.current_loc == i || Hero.current_loc == static_cast<int>(opposite_emotion)) { // 4. Если игрок находится в одном из закрываемых миров - перемещаем
                vector<int> available_worlds;
                for (int j = 0; j < 6; j++) {
                    if (!Worlds[j].is_locked) {
                        available_worlds.push_back(j);
                    }
                }

                if (!available_worlds.empty()) {
                    int random_index = rand() % available_worlds.size();
                    Hero.current_loc = available_worlds[random_index];
                    cout << ">> Переход в " << Worlds_Names[Hero.current_loc]
                        << " из-за блокировки " << Worlds_Names[i]
                        << " и " << Worlds_Names[opposite_emotion] << endl;
                }
                else {
                    cout << ">> Все миры закрыты! Игра завершена.\n";
                    // Завершение игры
                }
            }
            else {
                cout << ">> " << Worlds_Names[i] << " и "
                    << Worlds_Names[opposite_emotion] << " закрыты!\n";
            }
        }
    }
}

void Change_emotions(Worlds_Num emotion, char math, int x) { // Изменяет шкалу эмоции, она принимает эмоцию и то число на сколько ее изменить

    // Здесь я решил дополнить функцию возможностью выбирать то, будет шкала уменьшаться или увеличиваться с помощью char math

    if (Worlds[static_cast<int>(emotion)].is_locked) {
        return;
    }

    if (math != '+' && math != '-') return; // проверка на валидацию внутри кода

    int new_value = Hero.emotions[emotion];

    if (math == '+') new_value += x; // записываем новое значение для шкалы в переменную
    else new_value -= x; // записываем новое значение для шкалы в переменную

    if (new_value < 0) new_value = 0; // Блокируем шкалы с новым значением, если шкала перешла за пределы 100 или 0
    if (new_value > 100) new_value = 100;

    Hero.emotions[emotion] = new_value; // а теперь записываем новое значение в эмоции игрока

    Worlds_Num opposite_emotion = get_opposite_world(emotion); // Эмоции как переливающиеся сосуды
    Hero.emotions[opposite_emotion] = 100 - Hero.emotions[emotion];

    if (emotion == Worlds[Hero.current_loc].linked_emotion) {  // Проверка переходов только для эмоции текущего мира
        Check_movement();
    }

}

void Start_dialog() {

    bool start = true; // переменная для завершения цикла диалога 

    while (start) {

        for (int i = 0; i < 6; i++) {

            cout << left << setw(20) << Emotion_Names[i] << "\t" << Hero.emotions[i] << endl; // выводим текущие эмоции игрока

        }

        int choice;

        NPC Ela("Ela");
        Ela.text("Ты тут? (выбери 1 / 2..., 0 - exit) ", 60, 40, 60, 40, 60, 40);
        Ela.info();
        cout << "1) SADNESS (+10)" << endl;
        cout << "2) JOY (-10)" << endl;
        cout << "3) FEAR (+10)" << endl;
        cout << "4) ANGER (-10)" << endl;
        cout << "5) POWER (+10)" << endl;
        cout << "6) CALM (-10)" << endl;
        cin >> choice; // ожидаем ответа игрока на вопрос персонажа

        if (choice >= 0 || choice <= 6) { // проверяем чтобы ввод соответствовал количеству доступных игроку ответов 

            switch (choice) {
            case(1):
                Change_emotions(SADNESS, '+', 10);
                //Change_emotions(JOY, '-', 10); //????
                cout << "Sadness: " << Hero.emotions[SADNESS] << "\n"; // выводим как поменялась шкала из за ответа
                break;
            case(2):
                Change_emotions(JOY, '-', 10);
                //Change_emotions(JOY, '+', 10); //????
                cout << "Joy: " << Hero.emotions[JOY] << "\n"; // выводим как поменялась шкала из за ответа
                break;
            case(3):
                Change_emotions(FEAR, '+', 10);
                //Change_emotions(JOY, '-', 10); //????
                cout << "FEAR: " << Hero.emotions[FEAR] << "\n"; // выводим как поменялась шкала из за ответа
                break;
            case(4):
                Change_emotions(ANGER, '-', 10);
                //Change_emotions(ANGER, '+', 10);
                cout << "ANGER: " << Hero.emotions[ANGER] << "\n"; // выводим как поменялась шкала из за ответа
                break;
            case(5):
                Change_emotions(POWER, '+', 10);
                //Change_emotions(POWER, '-', 10);
                cout << "POWER: " << Hero.emotions[POWER] << "\n"; // выводим как поменялась шкала из за ответа
                break;
            case(6):
                Change_emotions(CALM, '-', 10);
                //Change_emotions(CALM, '+', 10);
                cout << "CALM: " << Hero.emotions[CALM] << "\n"; // выводим как поменялась шкала из за ответа
                break;
            }
        }
        if (choice == 0) { // выход с диалога

            start = false;

        }
    }

}

void Command_Go() {

    for (int i = 0; i < Worlds[Hero.current_loc].portal.size(); i++) {

        cout << i + 1 << ")" << left << setw(20) << Worlds[Hero.current_loc].portal[i].name << "\t"
            << (!Worlds[i].is_locked ? "Мир открыт \n" : "Мир закрыт \n") << endl;

    }

    cout << "В какой мир желаешь переместиться? \n";
    int choice;
    cin >> choice; // ожидаем ввода числового номера из ранее перечисленных миров

    if (choice > 0 || choice <= Worlds[Hero.current_loc].portal.size()) { // проверяем чтобы ввод соответствовал номерам миров

        int target_index = choice - 1;  // нивелирует тот факт что я ввожу 1 а меня перемещает в ноль (тк в коде для демонстрации i + 1) wwwwwe
        Portal_& portal = Worlds[Hero.current_loc].portal[target_index];

        if (portal.open && !Worlds[portal.target].is_locked) {
            Hero.current_loc = portal.target;
            cout << "Ты переместился в " << Worlds_Names[Hero.current_loc] << endl;
        }
        else {
            cout << "Этот мир закрыт!\n";
        }
    }
}

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