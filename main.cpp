#include <iostream>
#include <clocale>
#include "file_ops.h"
#include <windows.h>
#include <fstream>

int main() {
    //system("chcp 65001 > nul"); // Принудительно ставим UTF-8 в консоли
    //SetConsoleCP(65001);
    //SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "Russian");

    printf("=== PROGRAM STARTED ===\n\n");

    printf("--- Roma's block ---\n");
    printf("1. Creating binary files...\n");
    createBinaryFile("data1.txt", "data2.txt", "students.bin", "grades.bin");

    printf("2. Adding surnames...\n");
    appendSurnameToGrades("students.bin", "grades.bin", "students_full.bin");

    printf("3. Calculating averages...\n");
    calculateAverage("students_full.bin", "students_avg.bin");
    printf("\n");

    printf("--- Misha's block ---\n");
    printf("4. Forming bad students list...\n");
    CreateBadList("students_avg.bin", "bad_students.bin");
    printf("\n");

    printf("--- Jenya's block ---\n");
    int targetGroup = 4;

    printf("5. Group sheet (Alphabetical)...\n");
    printGroupAlphabetical("students_avg.bin", targetGroup);

    printf("6. Group sheet (By Average)...\n");
    printGroupByAverage("students_avg.bin", targetGroup);

    printf("7. Excellent students...\n");
    findExcellentStudents("students_avg.bin", "excellent_students.bin");

    printf("\n=== PROGRAM FINISHED ===\n");
    return 0;
}