#pragma once
# define _CRT_SECURE_NO_WARNINGS
#include "file_ops.h"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

void FormBadList(const char* binFilename, Student* badlist, int& badCount) {
    std::ifstream filemarks("C:\\Games\\fileb.txt", std::ios::binary);
    std::ifstream filenames("C:\\Games\\input.txt", std::ios::binary);
    Student list[10];
    std::string linemarks;
    std::string linenames;
    size_t marks_size[10]{ 0 };
    size_t students_size = 0;
    for (int j = 0; std::getline(filemarks, linemarks) || std::getline(filenames, linenames);++j, ++students_size) {
        Student st;

        if (linenames.empty()) continue;

        std::stringstream ss1(linenames);
        std::string token1;

        std::getline(ss1, token1, ';');
        st.recordBookNumber = std::stoi(token1);

        std::getline(ss1, token1, ';');
        strncpy(st.surname, token1.c_str(), sizeof(st.surname) - 1);

        std::getline(ss1, token1, ';');
        strncpy(st.name, token1.c_str(), sizeof(st.name) - 1);

        std::getline(ss1, token1);
        strncpy(st.patronymic, token1.c_str(), sizeof(st.patronymic) - 1);

        if (linemarks.empty()) continue;

        std::stringstream ss2(linemarks);
        std::string token2;

        std::getline(ss2, token2, ';');
        st.groupNumber = std::stoi(token2);

        std::getline(ss2, token2, ';');
        std::getline(ss2, token2, ';');

        for (int i = 0;std::getline(ss2, token2, ';');++i, ++marks_size[students_size]) {
            st.grades[i] = std::stoi(token2);
            std::getline(ss2, token2, ';');
        }

        list[j] = st;
    }
    badCount = 0;
    for (int i = 0; i < students_size; ++i) {
        bool isBad = false;
        for (int j = 0; j < marks_size[i];++j) {
            if (list[i].grades[j] <= 3) {
                isBad = true;
                break;
            }
        }
        if (isBad) {
            strncpy(badlist[badCount].surname, list[i].surname, sizeof(badlist[badCount].surname) - 1);
            badlist[badCount].groupNumber = list[i].groupNumber;
            badlist[badCount].recordBookNumber = list[i].recordBookNumber;
            badCount++;
        }
    }
}

void SortBadList(Student* badlist, int badCount) {
    for (int i = 0; i < badCount - 1; ++i) {
        for (int j = 0; j < badCount - i - 1; ++j) {
            if (badlist[j].groupNumber > badlist[j + 1].groupNumber) {
                Student temp = badlist[j];
                badlist[j] = badlist[j + 1];
                badlist[j + 1] = temp;
            }
            else if (badlist[j].groupNumber == badlist[j + 1].groupNumber) {
                if (strcmp(badlist[j].surname, badlist[j + 1].surname) > 0) {
                    Student temp = badlist[j];
                    badlist[j] = badlist[j + 1];
                    badlist[j + 1] = temp;
                }
            }
        }
    }
}



