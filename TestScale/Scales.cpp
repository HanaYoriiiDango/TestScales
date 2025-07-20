#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <map>

using namespace std;

// Здесь я начинаю объявлять все глобальные переменные, классы и структуры

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

};

struct Location { // структура для реализации миров по которым игрок будет перемещаться
    string name; // у каждого мира есть свое имя 
    Worlds_Num linked_emotion;  // Какая эмоция связана с этим миром
    bool is_locked = false; // флаг для закрытия мира
    vector<Portal_> portal; // векторный массив с типом данных прошлой структуры для привязки к мирам добавлен именно сюда

};

Player Hero; // Инициализирую персонажа игрока 
Location Worlds[6]; // Массив с нашими мирами
vector<NPC> Characters; // Вектор для персонажей (если честно я уже не помню зачем он создавался)


/*
 просто мои комментарии чтобы ничего не забыть
1) перемещение внутри мира пока что только в формате текста
1.1) все описания также нужно вводить сюда
1.2) Выбор действий никак не меняетсЯ и не ограничивается выбранными репликами
2) Перемещение будет как Go так и по шкалам - срастить
3) Master - готовая версия, всегда рабочая ветка. Test - ветка для тестов  √
4) По мере роста кода раскидать все по файлам, в них стараться сильно не срать
5) Нужно автоатизировать систему с влиянием реплик на шкалы чтобы не прописывать для каждой реплики
6) эти эмоции с персонажей я буду использовать для изменения эмоций шкал
6.1) при достижении одной эмоции своих границ  (0 / 100) мир закрывается и мы пермещаемся в другой
7) мне дают вариант ответов
8) в зависмимости от выбранного ответа я имзменяю шкалу согласно паттернам в таблице
9) Насколько подойдут для проекта ассоциативные контейнеры?

*/

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

    Location& current_world = Worlds[Hero.current_loc];  // теперь сюда по ссылке я будет записываться все что в current_world веедь они теперь одно и тоже
    int emotion_value = Hero.emotions[current_world.linked_emotion]; // значение эмоций игрока в текущем мире

    if (emotion_value <= 0 || emotion_value >= 100) {
    
        if (emotion_value <= 0 || emotion_value >= 100) {
            // Блокируем текущий мир
            current_world.is_locked = true;

            // Находим противоположный мир
            Worlds_Num opposite = get_opposite_world(current_world.linked_emotion);

            // Перемещаем игрока
            Hero.current_loc = opposite;
            cout << "Переход в " << Worlds_Names[opposite] << endl;

            // Сбрасываем эмоции
            Hero.emotions[current_world.linked_emotion] = 50;
            Hero.emotions[opposite] = 50;
        }
    
    }


}

void movement_control() { // эта функция нужна чтобы осуществлять перемещение игрока по шкалам из одного мира в другой

    for (int i = 0; i < Worlds[Hero.current_loc].name.size(); i++) { // перебираю все миры

        if (!Worlds[i].is_locked && (i == Worlds[Hero.current_loc].name.size() - 1)) { // выбираю те миры которые открыты на данный момент и i это последующий элемент

            Hero.current_loc = i; // перемещаю игрока на последующий мир после заблокированного, но может нужно реализовать перемещение в рандомный мир?
            cout << "You transition: " << Worlds_Names[i] << endl; // вывожу куда игрок переместился

        }
    }
}

void block_world() { // эта функция блокирует мир и вызывает функцию для перемещения игрока

    int counter = 0; // счетчик чтобы movement_control() не дублировался при переборе чисел в for

    for (int i = 0; i < 6; i++) { // перебираем согласно размерности массива со шкалами

        if (Hero.emotions[i] == 0 || Hero.emotions[i] == 100) { // Проверка каждой шкалы на то достигли ли они предела 0 или 100

            Worlds[i].is_locked = true; // закрываю флажок у мира
            Worlds[i].portal[i].open = false; // закрываю флажок у порталов в этом мире
            cout << Worlds[i].name << " Cloused \n"; // вывожу информацию что мир теперь закрыт

            if (!Worlds[i].is_locked) counter++; // если есть закрытый мир, то я добавляю 1 в счетчик 

        }
    }
    if (counter == 2) { // когда достигнет 2 то перемещаем игрока

        movement_control();

    }
    if (counter == 6) { // когда достигнет 6 то перемещаем игрока снова

        movement_control();

    }
    if (counter == 14) { // когда достигнет 14 то перемещаем игрока снова (последнее возможное перемещение)

        movement_control();

    }


}

void emotion_control() { // Эта функция нужна чтобы до конца игры фиксировать значения шкалы и оставлять его неизменным если оно достигло 100 или 0

    Hero.emotions[JOY] = 100 - Hero.emotions[SADNESS]; // Эмоции как переливающиеся сосуды
    Hero.emotions[CALM] = 100 - Hero.emotions[ANGER]; // Шкалы обязательно должны быть попарно связаны - задумка автора
    Hero.emotions[POWER] = 100 - Hero.emotions[FEAR]; // возможно стоит лучше продумать метод для их связывания?

    block_world();// вызываем функцию блокирующую миры, чтобы игрок не смог по ним перемещаться

}

void Change_emotions_plus(Worlds_Num emotion, int x) { // Изменяет шкалу эмоции, она принимает эмоцию и то число на сколько ее изменить

    if (Worlds[Hero.current_loc].linked_emotion == emotion && // Это защита для кода от закрытых миров
        Worlds[Hero.current_loc].is_locked) { // потому что мы не будем ничего менять если мир уже закрыт
        return;
    }

    int new_value = Hero.emotions[emotion] + x; // записываем новое значение для шкалы

    if (new_value < 0) new_value = 0; // Блокируем шкалы для изменений до конца игры, если шкала перешла за пределы 100 или 0
    if (new_value > 100) new_value = 100;

    Hero.emotions[emotion] = new_value; 

    Check_movement(); // Проверяем переходы

}

void Change_emotions_minus(Worlds_Num emotion, int x) { // Изменяет шкалу эмоции, она принимает эмоцию и то число на сколько ее изменить

    if (Worlds[Hero.current_loc].linked_emotion == emotion && // Это защита для кода от закрытых миров
        Worlds[Hero.current_loc].is_locked) { // потому что мы не будем ничего менять если мир уже закрыт
        return;
    }

    int new_value = Hero.emotions[emotion] + x; // записываем новое значение для шкалы

    if (new_value < 0) new_value = 0; // Блокируем шкалы для изменений до конца игры, если шкала перешла за пределы 100 или 0
    if (new_value > 100) new_value = 100;

    Hero.emotions[emotion] = new_value;

    Check_movement(); // Проверяем переходы

}


// функция для инициализации и добавления важных для игры элементов 

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

// функция для начала игры, здесь я буду прописывать весь ход игры

void Start_Game() {

    bool start = true; // переменная для завершения цикла (может стоить заменить на Hero.life? ), так как логически так игра будет длится до тех пор пока игрок жив
    string temp; // переменная для ввода команд 

    while (start) {


        cout << "тут странности какие то происходят, все в огне \n";
        cout << "Go - для перемещения \n";
        cout << "Loc - вывести текущее положение игрока \n";
        cout << "Start - начать диалог с персонажем \n";
        cin >> temp; // ожидаем ввода одной из команд команду

        if (temp == "Go") { // выводит все доступные миры для игрока в соответствии с его текущим положение и статусом портала (открыт / закрыт)   

            for (int i = 0; i < Worlds[Hero.current_loc].portal.size(); i++) {

                cout << i + 1 << ")" << left << setw(20) << Worlds[Hero.current_loc].portal[i].name << "\t"
                    << (Worlds[Hero.current_loc].portal[i].open ? "Мир открыт \n" : "Мир закрыт \n") << endl;

            }

            cout << "В какой мир желаешь переместиться? \n";
            int choice; 
            cin >> choice; // ожидаем ввода числового номера из ранее перечисленных миров

            if (choice > 0 || choice < 6) { // проверяем чтобы ввод соответствовал номерам миров

                for (int i = 0; i < Worlds[Hero.current_loc].portal.size(); i++) { // перебераем доступные порталы

                    if (!Worlds[i].is_locked) {

                        if (choice == Worlds[Hero.current_loc].portal[i].target) { //сравниваем введенное число с таргетом портала

                            Hero.current_loc = Worlds[Hero.current_loc].portal[i].target; // меняем значение в текущем положении игрока на таргет портала 
                            cout << "Current loc: " << Worlds_Names[i] << endl; // так как таргет всегда соответствует определенному миру мы перемещаем игрока и выводим ему его новое положение

                        }

                    }

                }
            }
        }
        if (temp == "Loc") {

            cout << "Current loc: " << Hero.current_loc << endl; // выводим игроку его текущее положение

        }
        if (temp == "Start") { // начинаем диалог 


            bool start = true; // переменная для завершения цикла диалога 

            while (start) {

                for (int i = 0; i < 6; i++) {

                    cout << left << setw(20) << Emotion_Names[i] << "\t" << Hero.emotions[i] << endl; // выводим текущие эмоции игрока

                }

                int choice;

                NPC Ela("Ela");
                Ela.text("Ты тут? (выбери 1 или 2) ", 60, 40, 50, 50, 50, 50); // в логику вопросов и ответов можно не вдумываться, так как они здесь для галочки
                Ela.info();
                cout << "1) нет ()" << endl;
                cout << "2) да ()" << endl;
                cin >> choice; // ожидаем ответа игрока на вопрос персонажа

                if (choice > 0 || choice < 2) { // проверяем чтобы ввод соответствовал количеству доступных игроку ответов 

                    switch (choice) {
                    case(1):
                        Change_emotions_plus(SADNESS, 10);
                        cout << "Sadness: " << Hero.emotions[SADNESS] << "\n"; // выводим как поменялась шкала из за ответа
                        break;
                    case(2):
                        Change_emotions_minus(JOY, 10);
                        cout << "Joy: " << Hero.emotions[JOY] << "\n"; // выводим как поменялась шкала из за ответа
                        break;
                    }
                }
                if (choice == 3) {

                    start = false;

                }
            }
        }
    }
}

// Основная функция в которой все хранится 
int main() {
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Init_Game();
    Start_Game();

}