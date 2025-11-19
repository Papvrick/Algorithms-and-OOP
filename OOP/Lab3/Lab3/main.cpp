#include <iostream>
#include "booleanvector.h"
#include <Windows.h>
using namespace std;

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    try {
        //Тест конструкторов
        cout << "1. Тест конструкторов" << endl;
        BooleanVector defaultt;
        BooleanVector normal(7, 1);
        BooleanVector char_v("10101001");
        BooleanVector copy_v(char_v);

        cout << "Конструктор с параметрами:" << endl;
        cout << normal << "  Ожидалось: 1111111" << endl;

        cout << "Конструктор из массива char:" << endl;
        cout << char_v << "  Ожидалось: 10101001" << endl;

        cout << "Конструктор копирования:" << endl;
        cout << copy_v << "  Ожидалось: 10101001" << endl << endl;

        //Тест длины и веса вектора
        cout << "2. Тест методов нахождения длины и веса вектора" << endl;
        BooleanVector testLength;
        int length, weight;
        cout << "Введите вектор: ";
        try {
            cin >> testLength;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> testLength;
        }

        length = testLength.getLength();
        weight = testLength.getWeight();
        cout << "Длина вектора = " << length << endl;
        cout << "Вес вектора = " << weight << endl << endl;

        //обмен содержимого с другим вектором (swap)
        cout << "3. Тест обмена с другим вектором (swap)" << endl;
        BooleanVector swap1("1011101"), swap2("0100110");
        cout << "Вектор 1: " << swap1 << endl;
        cout << "Вектор 2: " << swap2 << endl;
        swap1.swap(swap2);
        cout << "После обмена векторов:" << endl;
        cout << "Вектор 1: " << swap1 << endl;
        cout << "Вектор 2: " << swap2 << endl;
        cout << endl;

        //Инверсия всех компонент вектора
        cout << "4. Тест инверсии всех компонент вектора" << endl;
        BooleanVector invert;
        cout << "Введите вектор: ";
        try {
            cin >> invert;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> invert;
        }

        try {
            invert.invert();
            cout << "Инвертированный вектор: " << invert << endl << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка инверсии: " << e.what() << endl << endl;
        }

        //Инверсия i-ой компоненты
        cout << "5. Тест инверсии i-ой компоненты" << endl;
        BooleanVector testBit;
        cout << "Введите вектор: ";
        try {
            cin >> testBit;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> testBit;
        }

        int x;
        cout << "Введите индекс, который хотите инвертировать: ";
        cin >> x;

        try {
            testBit.invertBit(x);
            cout << "Вектор с инвертированной компонентой " << x << ": " << testBit << endl << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка инверсии бита: " << e.what() << endl << endl;
        }

        //Установка в 0/1 i-ой компоненты
        cout << "6. Тест установки в 0/1 i-ой компоненты" << endl;
        BooleanVector testSetBit;
        cout << "Введите вектор: ";
        try {
            cin >> testSetBit;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> testSetBit;
        }

        int bool_el, el;
        cout << "Введите компоненту, которую хотите установить, и во что хотите установить (0 или 1): ";
        cin >> el >> bool_el;

        try {
            testSetBit.setBit(el, bool_el);
            cout << "Новый вектор: " << testSetBit << endl << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка установки бита: " << e.what() << endl << endl;
        }

        //Перегрузка операций
        cout << "7. Тест перегрузки операций" << endl;
        BooleanVector operations1, operations2;
        cout << "Введите вектор1: ";
        try {
            cin >> operations1;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> operations1;
        }

        cout << "Введите вектор2: ";
        try {
            cin >> operations2;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> operations2;
        }

        int index;
        cout << "Введите индекс: "; cin >> index;

        try {
            cout << "вектор1[" << index << "] = " << operations1[index] << endl; 
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка доступа к вектор1[" << index << "]: " << e.what() << endl;
        }

        try {
            cout << "вектор2[" << index << "] = " << operations2[index] << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка доступа к вектор2[" << index << "]: " << e.what() << endl;
        }

        BooleanVector multiplication, summ, XOR, invertion, equal;

        try {
            multiplication = operations1 & operations2;
            cout << operations1 << " & " << operations2 << " = " << multiplication << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка операции И: " << e.what() << endl;
        }

        try {
            summ = operations1 | operations2;
            cout << operations1 << " | " << operations2 << " = " << summ << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка операции ИЛИ: " << e.what() << endl;
        }

        try {
            XOR = operations1 ^ operations2;
            cout << operations1 << " ^ " << operations2 << " = " << XOR << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка операции XOR: " << e.what() << endl;
        }

        try {
            invertion = ~operations1;
            cout << "~" << operations1 << " = " << invertion << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка инверсии: " << e.what() << endl;
        }

        try {
            equal = operations2;
            cout << "Присванивание: equal = вектор2, equal: " << equal << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка присваивания: " << e.what() << endl;
        }

        cout << endl;

        cout << "8. Побитовые сдвиги" << endl;
        BooleanVector testvector1, testvector2;
        int left, right;

        cout << "Введите вектор для проверки сдвига влево: ";
        try {
            cin >> testvector1;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> testvector1;
        }

        cout << "На сколько хотите сдвинуть?: "; cin >> left;

        try {
            BooleanVector resultleft = testvector1 << left;
            cout << "Результат сдвига влево: " << resultleft << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка сдвига влево: " << e.what() << endl;
        }

        cout << "Введите вектор для проверки сдвига вправо: ";
        try {
            cin >> testvector2;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка ввода: " << e.what() << endl;
            cout << "введите вектор заново: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> testvector2;
        }

        cout << "На сколько хотите сдвинуть?: "; cin >> right;

        try {
            BooleanVector resultright = testvector2 >> right;
            cout << "Результат сдвига вправо: " << resultright << endl;
        }
        catch (const std::runtime_error& e) {
            cout << "Ошибка сдвига вправо: " << e.what() << endl;
        }

    }
    catch (const std::exception& e) {
        cout << "Ошибка в программе: " << e.what() << endl;
        return 1;
    }

    cout << endl;

    // Циклические сдвиги
    cout << "9. Циклические сдвиги" << endl;
    BooleanVector testvector3, testvector4;
    int left_cyclic, right_cyclic;

    cout << "Введите вектор для проверки циклического сдвига влево: ";
    try {
        cin >> testvector3;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка ввода: " << e.what() << endl;
        cout << "введите вектор заново: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> testvector3;
    }

    cout << "На сколько хотите сдвинуть?: ";
    cin >> left_cyclic;

    try {
        BooleanVector resultleft_cyclic = testvector3.cyclicShiftLeft(left_cyclic);
        cout << "Результат циклического сдвига влево: " << resultleft_cyclic << endl;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка циклического сдвига влево: " << e.what() << endl;
    }

    cout << "Введите вектор для проверки циклического сдвига вправо: ";
    try {
        cin >> testvector4;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка ввода: " << e.what() << endl;
        cout << "введите вектор заново: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> testvector4;
    }

    cout << "На сколько хотите сдвинуть?: ";
    cin >> right_cyclic;

    try {
        BooleanVector resultright_cyclic = testvector4.cyclicShiftRight(right_cyclic);
        cout << "Результат циклического сдвига вправо: " << resultright_cyclic << endl;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка циклического сдвига вправо: " << e.what() << endl;
    }

    cout << endl;

    // Установка в 0/1 k компонент, начиная с i-ой
    cout << "10. Тест установки k компонент, начиная с i-ой" << endl;
    BooleanVector testSetBits;
    cout << "Введите вектор: ";
    try {
        cin >> testSetBits;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка ввода: " << e.what() << endl;
        cout << "введите вектор заново: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> testSetBits;
    }

    uint32_t startIdx, countBits;
    bool setValue;
    cout << "Введите стартовый индекс, количество компонент и значение (0/1): ";
    cin >> startIdx >> countBits >> setValue;

    try {
        cout << "Исходный вектор: " << testSetBits << endl;
        testSetBits.setBits(startIdx, countBits, setValue);
        cout << "После установки " << countBits << " бит с индекса " << startIdx << " в " << setValue << ": " << testSetBits << endl;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка установки бит: " << e.what() << endl;
    }

    cout << endl;

    // Установка в 0/1 всех компонент вектора
    cout << "11. Тест установки всех компонент вектора" << endl;
    BooleanVector testSetAll;
    cout << "Введите вектор: ";
    try {
        cin >> testSetAll;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка ввода: " << e.what() << endl;
        cout << "введите вектор заново: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> testSetAll;
    }

    bool allValue;
    cout << "Введите значение для установки всех компонент (0/1): ";
    cin >> allValue;

    try {
        cout << "Исходный вектор: " << testSetAll << endl;
        testSetAll.setAll(allValue);
        cout << "После установки всех бит в " << allValue << ": " << testSetAll << endl;
    }
    catch (const std::runtime_error& e) {
        cout << "Ошибка установки всех бит: " << e.what() << endl;
    }

    return 0;
}