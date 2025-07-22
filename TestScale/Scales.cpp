#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

enum Worlds_Num { SADNESS, JOY, FEAR, CALM, ANGER, POWER }; // инициализирую миры 
string Emotion_Names[6] = { "Грусть", "Радость", "Страх", "Спокойствие", "Гнев", "Сила" }; // названия эмоций 
string Worlds_Names[6] = { "Мир Грусти", "Мир Радости", "Мир Страха", "Мир Спокойствия", "Мир Гнева", "Мир Силы" }; // названия миров

struct Portal_ { // структура для реализации перемещения между мирами посредством команды Go по порталам
    string name; // у портала есть имя, которое я смогу выводить в консоли для демонстрации выбора перемещения
    int target; // таргет играет ключеву роль, так как у каждого портала он свой и его можно легко приравнивать к текущему положению игрока (менять его положение)
    bool open = true; // это флаг который отвечате за то доступен мир игроку или нет

};

struct info { // вспомогательная структура для следующего класса

    string dialog_Text; // здесь будет хранится реплеки персонажей
    int effect[6]; // этот массив отвечает за демонстрацию (посредством метода в классе) того как могут поменяться шкалы игрока в этом диалоге

};

class NPC { // класс для реализации диалогов от лица персонажей игры 
public:

    string name; // имя персонажа
    vector<info> words; // векторный массив с типом данных нашей вспомогательной структуры

    NPC(string name) : name(name) {}; // конструктор ожидает имя

    void text(string letter, int sadness, int joy, int fear, int calm, int anger, int power) { // в методе ожидаем текст (который будет говорит персонаж)
                                                                                               // Также ожидаем значения для демонстрации того как будут меняться шкалы при том или ином ответе
        words.push_back({ letter, {sadness, joy, fear, calm, anger, power} }); // добавляем это все в words 
        
    }

    void info() { // этот метод нужен чтобы непосредственно уже выводить имя персонажа и его реплику

        cout << name << ": "; // выводим имя персонажа

        for (int i = 0; i < words.size(); i++) {

            cout << words[i].dialog_Text; // выводим текст персонажа

            for (int j = 0; j < 6; j++) {

                cout << "(" << words[i].effect[j] << ") "; // выводим то какой эффект окажут ответные реплики игрока на шкалы

            }

            cout << "\n";
        }
    }
};

struct Player { // Структура для персонажа игрока

    string name; // имя главного героя
    int current_loc = ANGER; // текущая позиция главного героя
    int emotions[6] = { 50, 50, 50, 50, 50, 50 }; // эмоциональные шкалы главного героя (находятся в балансе, все по 50)
    bool life = true;

};

struct Location { // структура для реализации миров по которым игрок будет перемещаться
    string name; // у каждого мира есть свое имя 
    Worlds_Num linked_emotion;  // Какая эмоция связана с этим миром
    bool is_locked = false; // флаг для закрытия мира
    vector<Portal_> portal; // векторный массив с типом данных прошлой структуры для привязки к мирам добавлен именно сюда

};

Player Hero; // Инициализирую персонажа игрока 
Location Worlds[6]; // Массив с нашими мирами

Worlds_Num get_opposite_world(Worlds_Num world) {

    switch (world) {
    case SADNESS: return JOY;
    case JOY: return SADNESS;
    case FEAR: return CALM;
    case CALM: return FEAR;
    case ANGER: return POWER;
    case POWER: return ANGER;
    default: return world; 
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

void Change_emotions(Worlds_Num emotion,char math, int x) { // Изменяет шкалу эмоции, она принимает эмоцию и то число на сколько ее изменить

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

void Init_Game() {

    NPC Ela("PORNO");
    Ela.text("BLADIMIR PUTIN MOLODEC", 100, 50, 100, 99, 90, 99);
    //Ela.info();

    // Создаю миры и порталы для них
    Worlds[SADNESS].name = "Мир Грусти";
    Worlds[SADNESS].portal.push_back({"Мир Радости", JOY });
    Worlds[SADNESS].portal.push_back({ "Мир Страха", FEAR });
    Worlds[SADNESS].portal.push_back({ "Мир Спокойствия", CALM });
    Worlds[SADNESS].portal.push_back({ "Мир Гнева", ANGER });
    Worlds[SADNESS].portal.push_back({ "Мир Силы", POWER });

    Worlds[JOY].name = "Мир Радости";
    Worlds[JOY].portal.push_back({ "Мир Грусти", SADNESS });
    Worlds[JOY].portal.push_back({ "Мир Страха", FEAR });
    Worlds[JOY].portal.push_back({ "Мир Спокойствия", CALM });
    Worlds[JOY].portal.push_back({ "Мир Гнева", ANGER });
    Worlds[JOY].portal.push_back({ "Мир Силы", POWER });

    Worlds[FEAR].name = "Мир Страха";
    Worlds[FEAR].portal.push_back({ "Мир Грусти", SADNESS });
    Worlds[FEAR].portal.push_back({ "Мир Радости", JOY });
    Worlds[FEAR].portal.push_back({ "Мир Спокойствия", CALM });
    Worlds[FEAR].portal.push_back({ "Мир Гнева", ANGER });
    Worlds[FEAR].portal.push_back({ "Мир Силы", POWER });

    Worlds[CALM].name = "Мир Спокойствия";
    Worlds[CALM].portal.push_back({ "Мир Грусти", SADNESS });
    Worlds[CALM].portal.push_back({ "Мир Радости", JOY });
    Worlds[CALM].portal.push_back({ "Мир Страха", FEAR });
    Worlds[CALM].portal.push_back({ "Мир Гнева", ANGER });
    Worlds[CALM].portal.push_back({ "Мир Силы", POWER });

    Worlds[ANGER].name = "Мир Гнева";
    Worlds[ANGER].portal.push_back({ "Мир Грусти", SADNESS });
    Worlds[ANGER].portal.push_back({ "Мир Радости", JOY });
    Worlds[ANGER].portal.push_back({ "Мир Страха", FEAR });
    Worlds[ANGER].portal.push_back({ "Мир Спокойствия", CALM });
    Worlds[ANGER].portal.push_back({ "Мир Силы", POWER });

    Worlds[POWER].name = "Мир Силы";
    Worlds[POWER].portal.push_back({ "Мир Грусти", SADNESS });
    Worlds[POWER].portal.push_back({ "Мир Радости", JOY });
    Worlds[POWER].portal.push_back({ "Мир Страха", FEAR });
    Worlds[POWER].portal.push_back({ "Мир Спокойствия", CALM });
    Worlds[POWER].portal.push_back({ "Мир Гнева", ANGER });
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

int main() {
    setlocale(LC_ALL, "RU");
    srand(static_cast<unsigned int>(time(nullptr)));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Init_Game();
    Start_Game();
    
}