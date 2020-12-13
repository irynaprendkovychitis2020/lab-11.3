// lab 11.3 B.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h> 
#include <fstream>

using namespace std;

enum Specialty { KN, IT, software, VP, CA };

string strSpecialty[] = { "KN", "IT", "software", " VP", " CA" };

struct Student
{
    char prizv[64];
    Specialty specialty;
    short unsigned kurs;
    short unsigned physics;
    short unsigned math;
    short unsigned third_mark;
    union
    {
        short unsigned programming;
        short unsigned methods;
        short unsigned pedagogika;
    };
};

void Create(char* FileName);
void load_print(char* FileName);
void SortBin(char* FileName);
double favgMark(fstream& f, const int i);
int fMark(fstream& f, const int i);
char fPrizv(fstream& f, const int i);
void fChange(fstream& f, const int i, const int j);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char FileName[81];
    int menu;
    do {
        cout << "[0] - Create a list of students " << endl;
        cout << "[1] - List students" << endl;
        cout << "[2] - Arrange" << endl;
        cout << "[3] - Exit the program" << endl;

        cout << "Menu item: "; cin >> menu;
        switch (menu)
        {
        case 0:
            cin.get(); cin.sync();
            cout << "Enter a file name:";
            cin.getline(FileName, sizeof(FileName));

            Create(FileName);
            break;
        case 1:
            cin.get(); cin.sync();
            cout << "Enter a file name: ";
            cin.getline(FileName, sizeof(FileName));
            load_print(FileName);
            break;
        case 2:
            cin.get(); cin.sync();
            cout << "Enter a file name: ";
            cin.getline(FileName, sizeof(FileName));
            SortBin(FileName);
            break;
        case 3:
            exit(1);
        default:
            cout << "You have entered an incorrect value" << endl;
            break;
        }
    } while (menu != 3);
}

void Create(char* FileName)
{
    ofstream f(FileName, ios::binary);
    if (!f)
    {
        cerr << "Error opening file '" << FileName << "'" << endl;
        exit(1);
    }

    Student s;
    int specialty;
    char ch;
    int i = 0;
    do {
        cout << endl;
        cout << "Student # " << i + 1 << ":" << endl;
        ++i;
        cin.sync();
        cout << "  Name: ";  cin >> s.prizv;
        cout << "  Course: "; cin >> s.kurs;
        do
        {
            cout << " specialty (0 - KN, 1 - IT, 2 - software, 3 - VP, 4 - CA): "; cin >> specialty;
        } 
        while (specialty != 0 && specialty != 1 && specialty != 2 && specialty != 3 && specialty != 4);
        s.specialty = (Specialty)specialty;
        cout << "  grade in physics: "; cin >> s.physics;
        cout << "  grade in mathematics: "; cin >> s.math;
        switch (specialty)
        {
        case 0:
            cout << "  grade in programming: "; cin >> s.programming;
            break;
        case 3:
            cout << "  grade in methods: "; cin >> s.methods;
            break;
        case 1:
        case 2:
        case 4:
            cout << "  grade in pedagogik: "; cin >> s.pedagogika;
            break;
        }
        if (!f.write((char*)&s, sizeof(Student)))
        {
            cerr << "Write error" << endl;
            exit(1);
        }
        cout << "Continue? (Y / N): "; cin >> ch;

    } while (ch == 'Y' || ch == 'y');

}
void load_print(char* FileName)
{
    ifstream f(FileName, ios::binary);
    if (!f)
    {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    Student s;
    int i = 0;
    cout << "================================================================================================================="
        << endl;
    cout << "| # | Last name | Course | Specialty | Physics |Mathematics |Programming| Numerical methods| Pedagogy |"
        << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
    while (f.read((char*)&s, sizeof(Student)))
    {
        cout << "| " << setw(2) << right << i + 1 << " "
            << "| " << setw(13) << left << s.prizv
            << "| " << setw(3) << right << s.kurs << "  "
            << "| " << setw(8) << right << strSpecialty[s.specialty] << "      "
            << "| " << setw(4) << right << s.physics << "   "
            << "| " << setw(6) << right << s.math << "     ";
        switch (s.specialty)
        {
        case 0:
            cout << "| " << setw(7) << right << s.programming << "       |" << setw(18) << "|" << setw(13) << "|" << endl;
            break;
        case 3:
            cout << "| " << setw(15) << "|" << setw(10) << right << s.methods << "       |" << setw(13) << "|" << endl;
            break;
        case 1:
        case 2:
        case 4:
            cout << "| " << setw(15) << "|" << setw(18) << "|" << right << setw(7) << s.pedagogika << "     |" << endl;
            break;
        }
        ++i;
    }
    cout << "=================================================================================================================" << endl;
}
void SortBin(char* fname)
{
    fstream f(fname, ios::binary | ios::in | ios::out);
    if (!f)
    {
        cerr << "Error opening file " << endl;
        exit(1);
    }

    f.seekg(0 * (long)sizeof(Student), ios::end);
    int size = f.tellg() / (sizeof(Student));
    f.seekg(0, ios::beg);
    for (int i0 = 1; i0 < size; i0++)
        for (int i1 = 0; i1 < size - i0; i1++)
        {
            double avgMark = favgMark(f, i1);
            double avgMark2 = favgMark(f, i1 + 1);
            int mark = fMark(f, i1);
            int mark2 = fMark(f, i1 + 1);
            char student1 = fPrizv(f, i1);
            char student2 = fPrizv(f, i1 + 1);
            if ((avgMark > avgMark2)
                || (avgMark == avgMark2 && mark > mark2)
                || (avgMark == avgMark2 && mark == mark2 && student1 < student2)
                )
                fChange(f, i1, i1 + 1);
        }
    f.seekp(0, ios::end);
}
double favgMark(fstream& f, const int i)
{
    Student s;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    int physics_mark = s.physics;
    int math_mark = s.math;
    int third_mark = s.programming;

    return (physics_mark + math_mark + third_mark) / 3.;
}
int fMark(fstream& f, const int i)
{
    Student s;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    int third_mark = s.third_mark;
    return third_mark;
}
char fPrizv(fstream& f, const int i)
{
    Student s;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    char prizv[81];
    strcpy_s(prizv, sizeof(prizv), s.prizv);
    return *prizv;
}
void fChange(fstream& f, const int i, const int j)
{
    Student si, sj, temp;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&si, sizeof(Student));
    temp = si;
    f.seekg(j * (long)sizeof(Student));
    f.read((char*)&sj, sizeof(Student));
    f.seekp(i * (long)sizeof(Student));
    f.write((char*)&sj, sizeof(Student));
    f.seekp(j * (long)sizeof(Student));
    f.write((char*)&temp, sizeof(Student));
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
