#include "file_ops.h"
#include <iostream>
#include <fstream>
#include <cstring>

void printGroupAlphabetical(const char* binFileName, int group) {
    std::ifstream file(binFileName, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << binFileName << "\n";
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    int totalStudents = fileSize / sizeof(Student);

    if (totalStudents == 0) {
        std::cout << "Файл пуст.\n";
        file.close();
        return;
    }

    Student* groupStudents = new Student[totalStudents];
    int count = 0;
    Student temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        if (temp.groupNumber == group) {
            groupStudents[count] = temp;
            count++;
        }
    }
    file.close();

    if (count == 0) {
        std::cout << "В группе " << group << " нет студентов.\n";
        delete[] groupStudents;
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int cmp = std::strcmp(groupStudents[j].surname, groupStudents[j + 1].surname);
            if (cmp == 0) {
                cmp = std::strcmp(groupStudents[j].name, groupStudents[j + 1].name);
            }
            if (cmp == 0) {
                cmp = std::strcmp(groupStudents[j].patronymic, groupStudents[j + 1].patronymic);
            }
            if (cmp > 0) {
                Student tempSwap = groupStudents[j];
                groupStudents[j] = groupStudents[j + 1];
                groupStudents[j + 1] = tempSwap;
            }
        }
    }

    std::cout << "=== ВЕДОМОСТЬ ОЦЕНОК ДЛЯ ГРУППЫ " << group << " ===\n";
    std::cout << "---------------------------------------------------------\n";
    std::cout << "Фамилия И.О.\t| Зачетка | Оценки       | Ср. балл\n";
    std::cout << "---------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        std::cout << groupStudents[i].surname << " "
            << groupStudents[i].name[0] << ". "
            << groupStudents[i].patronymic[0] << ".\t| "
            << groupStudents[i].recordBookNumber << "\t  | ";

        for (int g = 0; g < 5; g++) {
            std::cout << groupStudents[i].grades[g] << " ";
        }

        std::cout << "\t | " << groupStudents[i].average << "\n";
    }
    std::cout << "---------------------------------------------------------\n\n";

    delete[] groupStudents;
}

void printGroupByAverage(const char* binFileName, int group) {
    std::ifstream file(binFileName, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << binFileName << "\n";
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    int totalStudents = fileSize / sizeof(Student);

    if (totalStudents == 0) {
        std::cout << "Файл пуст.\n";
        file.close();
        return;
    }

    Student* groupStudents = new Student[totalStudents];
    int count = 0;
    Student temp;

    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        if (temp.groupNumber == group) {
            groupStudents[count] = temp;
            count++;
        }
    }
    file.close();

    if (count == 0) {
        std::cout << "В группе " << group << " нет студентов.\n";
        delete[] groupStudents;
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (groupStudents[j].average < groupStudents[j + 1].average) {
                Student tempSwap = groupStudents[j];
                groupStudents[j] = groupStudents[j + 1];
                groupStudents[j + 1] = tempSwap;
            }
        }
    }

    std::cout << "=== ВЕДОМОСТЬ ГРУППЫ " << group << " (ПО УБЫВАНИЮ СР. БАЛЛА) ===\n";
    std::cout << "---------------------------------------------------------\n";
    std::cout << "Фамилия И.О.\t| Зачетка | Оценки       | Ср. балл\n";
    std::cout << "---------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        std::cout << groupStudents[i].surname << " "
            << groupStudents[i].name[0] << ". "
            << groupStudents[i].patronymic[0] << ".\t| "
            << groupStudents[i].recordBookNumber << "\t  | ";

        for (int g = 0; g < 5; g++) {
            std::cout << groupStudents[i].grades[g] << " ";
        }

        std::cout << "\t | " << groupStudents[i].average << "\n";
    }
    std::cout << "---------------------------------------------------------\n\n";
    delete[] groupStudents;
}

void findExcellentStudents(const char* binFileName, const char* excelBinFilename) {
    std::ifstream file(binFileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << binFileName << "\n";
        return;
    }
    std::ofstream outFile(excelBinFilename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Ошибка: Не удалось создать файл " << excelBinFilename << "\n";
        file.close();
        return;
    }

    Student temp;
    int count = 0;

    std::cout << "=== СПИСОК ОТЛИЧНИКОВ ===\n";
    std::cout << "--------------------------------------\n";
    std::cout << "Фамилия\t\t| Группа | Зачетка\n";
    std::cout << "--------------------------------------\n";

    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        bool isExcellent = true;
        for (int i = 0; i < 5; i++) {
            if (temp.grades[i] != 5) {
                isExcellent = false;
                break;
            }
        }
        if (isExcellent) {
            outFile.write(reinterpret_cast<const char*>(&temp), sizeof(Student));
            std::cout << temp.surname << "\t\t| "
                << temp.groupNumber << "\t | "
                << temp.recordBookNumber << "\n";
            count++;
        }
    }

    file.close();
    outFile.close();

    if (count == 0) {
        std::cout << "Отличников не найдено.\n";
    }
    std::cout << "--------------------------------------\n";
    std::cout << "Всего отличников: " << count << "\n";
    std::cout << "Список сохранен в файл: " << excelBinFilename << "\n\n";
}

