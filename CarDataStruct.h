#pragma once

#include <stdint.h>

typedef enum {
    RecognizeNone,
    RecognizeModel,
    RecognizeType,
    RecognizeBoth
} RecognizeMode;


typedef struct
{
    char Name[256];
    char Mark[256];
    char Modl[256];
    char Genr[256];
    char ProbMarkStr[256];

    char Type[256];
    char Side[256];
    char ProbTypeStr[256];

    char FileFullName[256];

} CarDataStruct;


typedef struct
{
    char Text[256];

} StringStruct;

