#include <iostream>
#include <clocale>
#include "file_ops.h"

int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "=== НАЧАЛО РАБОТЫ ПРОГРАММЫ ===\n\n";

    // ==========================================
    // БЛОК 1. ТИМЛИД (Рома)
    // ==========================================
    std::cout << "--- Блок Ромы (Тимлид) ---\n";

    std::cout << "1. Формирование бинарного файла из текстового...\n";
    createBinaryFile("data.txt", "students.bin");

    std::cout << "2. Добавление фамилий к ведомостям...\n";
    appendSurnameToGrades("students.bin", "students_with_names.bin");

    std::cout << "3. Вычисление среднего балла...\n";
    calculateAverage("students_with_names.bin");
    std::cout << "Готово.\n\n";


    // ==========================================
    // БЛОК 2. РАЗРАБОТЧИК 1 
    // ==========================================

    std::cout << "--- Блок Михаила ---" << '\n';

    std::cout << "формирование списка неуспевающих...\n";
    void FormBadList(const char* binFilename, Student * badlist, int& badCount);

    std::cout << "список неуспевающих до сортировки:\n";

    std::cout << "сортировка списка неуспевающих по группам...\n";
    void SortBadList(Student * badlist, int badCount);


    std::cout << "распечатка бинарного файла после сортировки...\n";
    // ==========================================
    // БЛОК 3. РАЗРАБОТЧИК 2 
    // ==========================================




    std::cout << "\n=== РАБОТА ПРОГРАММЫ ЗАВЕРШЕНА ===\n";
    return 0;
}