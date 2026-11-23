#include "file_ops.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>

void printGroupAlphabetical(const char* binFileName, int group) {
    std::ifstream file(binFileName, std::ios::binary);
    if (!file.is_open()) return;
    file.seekg(0, std::ios::end);
    int totalStudents = file.tellg() / sizeof(Student);
    file.seekg(0, std::ios::beg);
    if (totalStudents == 0) return;

    Student* groupStudents = new Student[totalStudents](); // () в конце зануляет массив!
    int count = 0;
    Student temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        if (temp.groupNumber == group) groupStudents[count++] = temp;
    }
    file.close();
    if (count == 0) { delete[] groupStudents; return; }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int cmp = std::strcmp(groupStudents[j].surname, groupStudents[j + 1].surname);
            if (cmp == 0) cmp = std::strcmp(groupStudents[j].name, groupStudents[j + 1].name);
            if (cmp == 0) cmp = std::strcmp(groupStudents[j].patronymic, groupStudents[j + 1].patronymic);
            if (cmp > 0) { Student t = groupStudents[j]; groupStudents[j] = groupStudents[j + 1]; groupStudents[j + 1] = t; }
        }
    }

    printf("\n=== GROUP %d ALPHABETICAL ===\n", group);
    for (int i = 0; i < count; i++) {
        printf("%.40s %c. %c.\t| %d\t| %d %d %d\t| Avg: %.2f\n",
            groupStudents[i].surname,
            groupStudents[i].name[0], groupStudents[i].patronymic[0],
            groupStudents[i].recordBookNumber,
            groupStudents[i].grades[0], groupStudents[i].grades[1], groupStudents[i].grades[2],
            groupStudents[i].average);
    }
    delete[] groupStudents;
}

void printGroupByAverage(const char* binFileName, int group) {
    std::ifstream file(binFileName, std::ios::binary);
    if (!file.is_open()) return;
    file.seekg(0, std::ios::end);
    int totalStudents = file.tellg() / sizeof(Student);
    file.seekg(0, std::ios::beg);
    if (totalStudents == 0) return;

    Student* groupStudents = new Student[totalStudents]();
    int count = 0;
    Student temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        if (temp.groupNumber == group) groupStudents[count++] = temp;
    }
    file.close();
    if (count == 0) { delete[] groupStudents; return; }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (groupStudents[j].average < groupStudents[j + 1].average) {
                Student t = groupStudents[j]; groupStudents[j] = groupStudents[j + 1]; groupStudents[j + 1] = t;
            }
        }
    }

    printf("\n=== GROUP %d BY AVERAGE ===\n", group);
    for (int i = 0; i < count; i++) {
        printf("%.40s\t| Avg: %.2f\n", groupStudents[i].surname, groupStudents[i].average);
    }
    delete[] groupStudents;
}

void findExcellentStudents(const char* binFileName, const char* excelBinFilename) {
    std::ifstream file(binFileName, std::ios::binary);
    if (!file.is_open()) return;
    std::ofstream outFile(excelBinFilename, std::ios::binary);
    if (!outFile.is_open()) { file.close(); return; }

    Student temp;
    int count = 0;
    printf("\n=== EXCELLENT STUDENTS ===\n");

    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        bool isExcellent = true;
        for (int i = 0; i < 3; ++i) if (temp.grades[i] != 5) { isExcellent = false; break; }
        if (isExcellent) {
            outFile.write(reinterpret_cast<const char*>(&temp), sizeof(Student));
            printf("%.40s\t| Group: %d | Book: %d\n", temp.surname, temp.groupNumber, temp.recordBookNumber);
            count++;
        }
    }
    file.close(); outFile.close();
    if (count == 0) printf("No excellent students found.\n");
    printf("Total excellent: %d\n\n", count);


}
// Запись списка неуспевающих в текстовый файл
void saveBadListToTxt(const char* binFilename, const char* txtFilename) {
    std::ifstream in(binFilename, std::ios::binary);
    std::ofstream out(txtFilename);
    if (!in.is_open() || !out.is_open()) return;

    out << "LIST OF FAILING STUDENTS (Sorted by group, then by surname):\n";
    out << "--------------------------------------------------------\n";
    out << "Surname\t\t| Group | Record Book\n";
    out << "--------------------------------------------------------\n";

    Student st;
    while (in.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        out << st.surname << "\t\t| " << st.groupNumber << "\t| " << st.recordBookNumber << "\n";
    }
    in.close(); out.close();
    std::cout << "Saved to " << txtFilename << "\n";
}

// Запись ведомости (по алфавиту) в текстовый файл
void saveGroupToTxtAlphabetical(const char* binFilename, int group, const char* txtFilename) {
    std::ifstream in(binFilename, std::ios::binary);
    std::ofstream out(txtFilename);
    if (!in.is_open() || !out.is_open()) return;

    // Сначала считываем всех студентов группы в массив для сортировки
    std::vector<Student> groupStudents;
    Student temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        if (temp.groupNumber == group) groupStudents.push_back(temp);
    }
    in.close();

    // Сортируем по алфавиту (стандартная функция std::sort)
    std::sort(groupStudents.begin(), groupStudents.end(), [](const Student& a, const Student& b) {
        return std::strcmp(a.surname, b.surname) < 0;
        });

    out << "GROUP " << group << " (Alphabetical):\n";
    out << "--------------------------------------------------------\n";
    out << "Surname\t\t| Record Book | Grades | Avg\n";
    out << "--------------------------------------------------------\n";
    for (const auto& s : groupStudents) {
        out << s.surname << "\t\t| " << s.recordBookNumber << "\t| "
            << s.grades[0] << " " << s.grades[1] << " " << s.grades[2] << " | " << s.average << "\n";
    }
    out.close();
    std::cout << "Saved to " << txtFilename << "\n";
}

// Запись ведомости (по убыванию среднего) в текстовый файл
void saveGroupToTxtAverage(const char* binFilename, int group, const char* txtFilename) {
    std::ifstream in(binFilename, std::ios::binary);
    std::ofstream out(txtFilename);
    if (!in.is_open() || !out.is_open()) return;

    std::vector<Student> groupStudents;
    Student temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
        if (temp.groupNumber == group) groupStudents.push_back(temp);
    }
    in.close();

    // Сортируем по убыванию среднего балла
    std::sort(groupStudents.begin(), groupStudents.end(), [](const Student& a, const Student& b) {
        return a.average > b.average;
        });

    out << "GROUP " << group << " (By Average):\n";
    out << "--------------------------------------------------------\n";
    out << "Surname\t\t| Avg\n";
    out << "--------------------------------------------------------\n";
    for (const auto& s : groupStudents) {
        out << s.surname << "\t\t| " << s.average << "\n";
    }
    out.close();
    std::cout << "Saved to " << txtFilename << "\n";
}