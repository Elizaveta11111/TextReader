#include "fileinfo.h"
#include "lines.h"

void InitFileInfo(HDC hdc, info* myInfo) {
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    myInfo->text = NULL;
    myInfo->lines = NULL;
    myInfo->fileSize = 0;
    myInfo->cxChar = tm.tmAveCharWidth;
    myInfo->cyChar = tm.tmHeight + tm.tmExternalLeading;
    myInfo->cxClient = 0;
    myInfo->cyClient = 0;
    myInfo->curLine = 0;
    myInfo->iVscrollPos = 0;
    myInfo->iVscrollMax = 0;
    myInfo->iHscrollPos = 0;
    myInfo->iHscrollMax = 0;
    myInfo->iMaxWidth = 0;
    myInfo->numLines = 0;
    myInfo->mode = 1;
}

void GetFileInfo(FILE* file, info* myInfo, HDC hdc) {
    fseek(file, 0, SEEK_END);
    myInfo->fileSize = ftell(file);
    myInfo->text = (char*)malloc(myInfo->fileSize);
    if (myInfo->text == NULL) {
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_SET);
    fread(myInfo->text, 1, myInfo->fileSize, file);
    fclose(file);
    myInfo->lines = (char**) malloc(myInfo->fileSize * sizeof(char*));
    if(myInfo->lines == NULL) {
        FreeFileInfo(myInfo);
        return;
    }
    if(myInfo->mode == 1)
        GetLinesUnwrap(myInfo, hdc);
}

void FreeFileInfo(info* myInfo) {
    if(myInfo->lines != NULL) {
        free(myInfo->lines);
        myInfo->lines = NULL;
    }
    if(myInfo->text != NULL) {
        free(myInfo->text);
        myInfo->text = NULL;
    }
    myInfo->curLine = 0;
    myInfo->iVscrollPos = 0;
    myInfo->iHscrollPos = 0;
    myInfo->iMaxWidth = 0;
    myInfo->numLines = 0;
}
