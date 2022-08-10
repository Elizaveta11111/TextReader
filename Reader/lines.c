#include "lines.h"

void GetLinesUnwrap(info* myInfo, HDC hdc) {
    long i, line = 1;
    int width = 0, maxWidth = 0, len = 0;
    SIZE sz;
    myInfo->lines[0] = myInfo->text;
    for(i = 0; i < myInfo->fileSize; i++) {
        if(myInfo->text[i] == '\n'){
            myInfo->lines[line++] = myInfo->text + i;
            GetTextExtentPoint32(hdc, myInfo->text + i - len, len + 1, &sz);
            width = sz.cx;
            if(width > maxWidth)
                maxWidth = width;
            len = -1;
        }
        len++;
    }
    myInfo->lines[line] = myInfo->text + i;
    myInfo->iMaxWidth = maxWidth;
    myInfo->numLines = line;
}

void GetLinesWrap(info* myInfo, HDC hdc) {
    long i, line = 1;
    int width = 0, len = 0;
    int l = myInfo->cxClient/myInfo->cxChar;
    SIZE sz;
    if(myInfo->lines == NULL) {
        return;
    }
    myInfo->lines[0] = myInfo->text;
    for(i = 0; i < myInfo->fileSize; i++) {
        if(myInfo->text[i] == '\n'){
            myInfo->lines[line++] = myInfo->text + i;
            len = -1;
        }
        else if(len > l) {
            GetTextExtentPoint32(hdc, myInfo->text + i - len, len + 1, &sz);
            width = sz.cx;
            if(width > myInfo->cxClient) {
                i--;
                myInfo->lines[line++] = myInfo->text + i;
                len = -1;
            }
        }
        len++;
    }
    myInfo->lines[line] = myInfo->text + i;
    myInfo->numLines = line;
}

int FindCurrentLine(info* myInfo) {
    int i;
    char a;
    for(i = myInfo->iVscrollPos; i > 0; i--){
        a = (myInfo->lines[i])[0];
        if(a == '\n')
            break;
    }
    return i;
}

int FindLine(char** lines, char* line) {
    int i = 0;
        if(lines == NULL) {
        return 0;
    }
    while (lines[i] != line)
        i++;
    return i;
}
