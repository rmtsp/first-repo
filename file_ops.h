#pragma once
#include "structs.h"
// В этом файле, каждый прописывает прототипы своих функций



// Roma (a, b, c)
void createBinaryFile(const char* txtFilename1, const char* txtFilename2, const char* binFilename1, const char* binFilename2);
void appendSurnameToGrades(const char* binFilename1, const char* binFilename2, const char* outFilename);
void calculateAverage(const char* inFilename, const char* outFilename);

// Misha 
void CreateBadList(const char* binFilename, const char* badBinFilename);

// Jenja 
void printGroupAlphabetical(const char* binFilename, int group);
void printGroupByAverage(const char* binFilename, int group);
void findExcellentStudents(const char* binFilename, const char* excelBinFilename);