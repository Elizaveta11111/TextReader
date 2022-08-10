#include "mylib.h"

// a sample exported function
FILE* OpenCommandLineFile(LPARAM lParam) {
    CREATESTRUCT* a = (CREATESTRUCT*) lParam;
    return fopen((char*)a->lpCreateParams, "rb");
}
