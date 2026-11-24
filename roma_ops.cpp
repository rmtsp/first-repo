#include "file_ops.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>

// Объявляем максимальное количество студентов
const int MAX_STUDENTS = 100;

void createBinaryFile(const char* txtFilename1, const char* txtFilename2, const char* binFilename1, const char* binFilename2) {
    std::ifstream txtFile1(txtFilename1);
    std::ofstream binFile1(binFilename1, std::ios::binary);
    if (txtFile1.is_open() && binFile1.is_open()) {
        std::string line;
        while (std::getline(txtFile1, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string token;
            Student st = {}; // Зануляем память

            if (!std::getline(ss, token, ';') || token.empty()) continue;
            try { st.recordBookNumber = std::stoi(token); }
            catch (...) { continue; }

            if (std::getline(ss, token, ';')) std::strncpy(st.surname, token.c_str(), sizeof(st.surname) - 1);
            if (std::getline(ss, token, ';')) std::strncpy(st.name, token.c_str(), sizeof(st.name) - 1);
            if (std::getline(ss, token, ';')) std::strncpy(st.patronymic, token.c_str(), sizeof(st.patronymic) - 1);

            binFile1.write(reinterpret_cast<const char*>(&st), sizeof(Student));
        }
    }
    txtFile1.close(); binFile1.close();

    std::ifstream txtFile2(txtFilename2);
    std::ofstream binFile2(binFilename2, std::ios::binary);
    if (txtFile2.is_open() && binFile2.is_open()) {
        std::string line;
        while (std::getline(txtFile2, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string token;
            Student st = {};

            if (!std::getline(ss, token, ';') || token.empty()) continue;
            try { st.groupNumber = std::stoi(token); }
            catch (...) { continue; }

            if (!std::getline(ss, token, ';') || token.empty()) continue;
            try { st.recordBookNumber = std::stoi(token); }
            catch (...) { continue; }

            if (!std::getline(ss, token, ';')) continue;
            if (std::getline(ss, token, ';') && !token.empty()) { try { st.grades[0] = std::stoi(token); } catch (...) {} }

            if (!std::getline(ss, token, ';')) continue;
            if (std::getline(ss, token, ';') && !token.empty()) { try { st.grades[1] = std::stoi(token); } catch (...) {} }

            if (!std::getline(ss, token, ';')) continue;
            if (std::getline(ss, token, ';') && !token.empty()) { try { st.grades[2] = std::stoi(token); } catch (...) {} }

            binFile2.write(reinterpret_cast<const char*>(&st), sizeof(Student));
        }
    }
    txtFile2.close(); binFile2.close();
    printf("Binary files created successfully.\n");
}

void appendSurnameToGrades(const char* binFilename1, const char* binFilename2, const char* outFilename) {
    // Вместо вектора используем статический массив
    Student students1[MAX_STUDENTS];
    int count1 = 0;

    std::ifstream binFile1(binFilename1, std::ios::binary);
    if (binFile1.is_open()) {
        Student st;
        // Читаем, пока есть данные и не превысили лимит
        while (binFile1.read(reinterpret_cast<char*>(&st), sizeof(Student)) && count1 < MAX_STUDENTS) {
            students1[count1] = st;
            count1++;
        }
        binFile1.close();
    }

    std::ifstream binFile2(binFilename2, std::ios::binary);
    std::ofstream outFile(outFilename, std::ios::binary);
    if (binFile2.is_open() && outFile.is_open()) {
        Student st2;
        while (binFile2.read(reinterpret_cast<char*>(&st2), sizeof(Student))) {
            // Ищем по массиву
            for (int i = 0; i < count1; ++i) {
                if (students1[i].recordBookNumber == st2.recordBookNumber) {
                    std::strncpy(st2.surname, students1[i].surname, sizeof(st2.surname) - 1);
                    break;
                }
            }
            outFile.write(reinterpret_cast<const char*>(&st2), sizeof(Student));
        }
        binFile2.close(); outFile.close();
    }
    printf("Surnames added to grades.\n");
}

void calculateAverage(const char* inFilename, const char* outFilename) {
    std::ifstream inFile(inFilename, std::ios::binary);
    std::ofstream outFile(outFilename, std::ios::binary);
    if (inFile.is_open() && outFile.is_open()) {
        Student st;
        while (inFile.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
            double sum = 0; int count = 0;
            for (int i = 0; i < 3; ++i) {
                if (st.grades[i] > 0) { sum += st.grades[i]; count++; }
            }
            st.average = (count > 0) ? (sum / count) : 0.0;
            outFile.write(reinterpret_cast<const char*>(&st), sizeof(Student));
        }
        inFile.close(); outFile.close();
    }
    printf("Averages calculated.\n");
}