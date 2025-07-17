#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

enum Worlds { SADNESS, JOY, FEAR, CALM, ANGER, POWER }; // инициализирую миры, name
string Emotion_Names[6] = { "Грусть", "Радость", "Страх", "Спокойствие", "Гнев", "Сила" };
string Worlds_Names[6] = { "Мир Грусти", "Мир Радости", "Мир Страха", "Мир Спокойствия", "Мир Гнева", "Мир Силы" };

struct info {
    
    string Dialog_Text;
    int effect[6];

};

class NPC {
public:

    string name;
    vector<info> Words;

    NPC(string name) : name(name){}

    void text( string letter, int sadness, int joy,int fear , int calm, int anger, int power) {

        Words.push_back({ letter, {sadness, joy, fear, calm, anger, power} });
        // наследование классов изучи полезно будет 
    }

    void Info() {

        cout << name << ": " << endl;

        for (int i = 0; i < Words.size(); i++) {
            
            cout << Words[i].Dialog_Text << "\t";
            for (int j = 0; j < 6; j++) {


                cout <<  Words[i].effect[j] << " ";
            }

        }

    }
    
};

struct Player {

	string Name;
	int Current_loc = SADNESS;
    int Emotions[6] = { 50, 50, 50, 50, 50, 50 };

};
 

Player Hero;

//NPC Characters[3] = {
//		   {"Эла", {
//			   {10, -5, 0, 0, 0, 0},   // Реплика 1: сильное влияние
//			   {-15, 5, 0, 0, 0, 0},   // Реплика 2: ещё сильнее
//			   {5, 0, 5, -5, 0, 0}     // Реплика 3: смесь
//		   }},
//		   {"Лоран", {
//			   {5, -2, 0, 0, 0, 0},    // Реплика 1: слабо
//			   {-5, 2, 0, 0, 0, 0},
//			   {0, 0, 5, -5, 0, 0}
//		   }},
//		   {"Ними", {
//			   {0, 0, 0, 0, 5, -5},    // Гнев/Сила
//			   {0, 0, 5, -5, 0, 0},    // Страх/Спокойствие
//			   {0, 0, -5, 5, 0, 0}
//		   }}
//};

/* 
1) перемещение внутри мира пока что только в формате текста 
2) Перемещение будет как Go так и по шкалам - срастить 
3) Master - готовая версия, всегда рабочая ветка. Test - ветка для тестов 
4) По мере роста кода раскидать все по файлам, в них стараться сильно не срать
5) Нужно автоатизировать систему с влиянием реплик на шкалы чтобы не прописывать для каждой реплики 
6) эти эмоции с персонажей я буду использовать для изменения эмоций шкал 
7) мне нужно рандомить девку и пускай она чота должна пиздеть
8) мне дают вариант ответов 
9) в зависмимости от выбранного ответа я имзменяю шкалу согласно паттернам в таблице
10) www


*/


void Init_Game() {

    NPC Ela("PORNO");
    Ela.text("BLADIMIR PUTIN MOLODEC", 100, 50, 100, 99, 90, 99);
    Ela.Info();

}

//void Start_Game() {
//
//    bool start = true;
//	int round = 0;
//	int choice;
//
//	while (start) {
//
//		NPC& c = Characters[round % 3];
//		cout << "\nТы находишься в " << Worlds_Names[Hero.Current_loc] << ". Тебе встречается " << c.Name << ".\n";
//		cout << "Выбери реплику:\n";
//		cout << "1) «Ты ведь всё ещё помнишь?»\n";
//		cout << "2) «Я пытался забыть»\n";
//		cout << "3) «Ты была права»\n";
//		cin >> choice;
//
//        if (choice < 1 || choice > 3) {
//            cout << "Некорректный выбор. Повторите.\n";
//            continue;
//        }
//
//        // Применение эффекта
//        int idx = choice - 1;
//        for (int i = 0; i < 6; i++) {
//            Emotions[i] += c.Effect[idx][i];
//            if (Emotions[i] > 100) Emotions[i] = 100;
//            if (Emotions[i] < 0) Emotions[i] = 0;
//        }
//
//        // Эмоции как переливающиеся сосуды:
//        Emotions[JOY] = 100 - Emotions[SADNESS];
//        Emotions[CALM] = 100 - Emotions[ANGER];
//        Emotions[POWER] = 100 - Emotions[FEAR];
//
//        bool extreme = true;
//        for (int i : {SADNESS, FEAR, ANGER}) {
//            if (Emotions[i] != 0 && Emotions[i] != 100) {
//                extreme = false;
//                break;
//            }
//        }
//        if (extreme) {
//            cout << "\nВсе эмоции достигли крайних пределов. Ты теряешь себя...\nИгра окончена.\n";
//            break;
//        }
//
//        // Перенос в другой мир, если текущая эмоция вышла за пределы
//        if (Emotions[Hero.Current_loc] <= 0 || Emotions[Hero.Current_loc] >= 100) {
//            for (int i = 0; i < 6; i++) {
//                if (i != Hero.Current_loc && Emotions[i] > 0 && Emotions[i] < 100) {
//                    Hero.Current_loc = i;
//                    cout << ">> Ты переместился в " << Worlds_Names[Hero.Current_loc] << " из-за нестабильности.\n";
//                    break;
//                }
//            }
//        }
//
//        // Вывод эмоций
//        cout << "\nТекущие эмоции:\n";
//        for (int i = 0; i < 6; i++) {
//            cout << Emotion_Names[i] << ": " << Emotions[i] << "\n";
//        }
//
//        round++;
//	}
//}

int main() {
	setlocale(LC_ALL, "RU");
	SetConsoleCP(1251); 
	SetConsoleOutputCP(1251); 
	Init_Game();
	//Start_Game();

}