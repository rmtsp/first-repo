#include <iostream>
#include <clocale>
#include "file_ops.h"
#include <windows.h>
#include <fstream>
#include <direct.h> 

int main() {
    //system("chcp 65001 > nul"); // Принудительно ставим UTF-8 в консоли
    //SetConsoleCP(65001);
    //SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "Russian");

    // Создаем папки для порядка
    _mkdir("bin");
    _mkdir("txt");

    printf("=== PROGRAM STARTED ===\n\n");

    printf("--- Roma's block ---\n");
    printf("1. Creating binary files...\n");
    // Файлы сохраняются в папку bin/
    createBinaryFile("data1.txt", "data2.txt", "bin/students.bin", "bin/grades.bin");

    printf("2. Adding surnames...\n");
    appendSurnameToGrades("bin/students.bin", "bin/grades.bin", "bin/students_full.bin");

    printf("3. Calculating averages...\n");
    calculateAverage("bin/students_full.bin", "bin/students_avg.bin");
    printf("\n");

    printf("--- Misha's block ---\n");
    printf("4. Forming bad students list...\n");
    CreateBadList("bin/students_avg.bin", "bin/bad_students.bin");
    printf("\n");

    printf("--- Jenya's block ---\n");
    int targetGroup = 4;

    printf("5. Group sheet (Alphabetical)...\n");
    printGroupAlphabetical("bin/students_avg.bin", targetGroup);

    printf("6. Group sheet (By Average)...\n");
    printGroupByAverage("bin/students_avg.bin", targetGroup);

    printf("7. Excellent students...\n");
    findExcellentStudents("bin/students_avg.bin", "bin/excellent_students.bin");

    // ПУНКТ 4: Запись в текстовые файлы (в папку txt/)
    printf("\n--- Saving lists to text files (Task 4) ---\n");
    saveBadListToTxt("bin/bad_students.bin", "txt/bad_students.txt");
    saveGroupToTxtAlphabetical("bin/students_avg.bin", targetGroup, "txt/group_alphabet.txt");
    saveGroupToTxtAverage("bin/students_avg.bin", targetGroup, "txt/group_average.txt");

    printf("\n=== PROGRAM FINISHED ===\n");
    return 0;
}