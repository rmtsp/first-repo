#define _CRT_SECURE_NO_WARNINGS
#include "file_ops.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdio>

void FormBadList(const char* binFilename, Student* badlist, int& badCount) {
    std::ifstream file(binFilename, std::ios::binary);
    if (!file.is_open()) { badCount = 0; return; }
    badCount = 0;
    Student st;
    while (file.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        bool isBad = false;
        for (int i = 0; i < 3; ++i) {
            if (st.grades[i] > 0 && st.grades[i] <= 3) { isBad = true; break; }
        }
        if (isBad && badCount < 100) {
            std::strncpy(badlist[badCount].surname, st.surname, sizeof(badlist[badCount].surname) - 1);
            badlist[badCount].surname[sizeof(badlist[badCount].surname) - 1] = '\0';
            badlist[badCount].groupNumber = st.groupNumber;
            badlist[badCount].recordBookNumber = st.recordBookNumber;
            badCount++;
        }
    }
    file.close();
}

void SortBadList(Student* badlist, int badCount) {
    for (int i = 0; i < badCount - 1; ++i) {
        for (int j = 0; j < badCount - i - 1; ++j) {
            if (badlist[j].groupNumber > badlist[j + 1].groupNumber) {
                Student temp = badlist[j]; badlist[j] = badlist[j + 1]; badlist[j + 1] = temp;
            }
            else if (badlist[j].groupNumber == badlist[j + 1].groupNumber) {
                if (std::strcmp(badlist[j].surname, badlist[j + 1].surname) > 0) {
                    Student temp = badlist[j]; badlist[j] = badlist[j + 1]; badlist[j + 1] = temp;
                }
            }
        }
    }
}

void PrintBinFile(const char* binFilename, const char* header) {
    std::ifstream inFile(binFilename, std::ios::binary);
    if (inFile.is_open()) {
        printf("%s\n", header);
        Student temp;
        while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
            // Используем %.40s для безопасности, чтобы не прочитать мусор за пределами строки
            printf("%.40s %d %d\n", temp.surname, temp.groupNumber, temp.recordBookNumber);
        }
        inFile.close();
    }
}

void CreateBadList(const char* binFilename, const char* badBinFilename) {
    Student badlist[100];
    int badCount = 0;
    FormBadList(binFilename, badlist, badCount);
    PrintBinFile(binFilename, "Binary file before sort:");
    printf("Sorting bad students...\n");
    SortBadList(badlist, badCount);
    std::ofstream outFile(badBinFilename, std::ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<char*>(badlist), sizeof(Student) * badCount);
        outFile.close();
    }
    PrintBinFile(badBinFilename, "Binary file after sort:");
}