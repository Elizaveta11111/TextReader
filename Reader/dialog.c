#include "dialog.h"
FILE* FileOpenDlg(HWND hwnd){
    static char szFileName[_MAX_PATH];
    static char szTitleName[_MAX_FNAME + _MAX_EXT];
    static char szFilter[] = "Text Files(*.TXT)\0*.txt\0" \
    "ASCII Files(*.ASC)\0*.asc\0" \
    "All Files(*.*)\0*.*\0\0";
    OPENFILENAME ofn;
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = szFilter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.nMaxFile = _MAX_PATH;
    ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = 0; // Set in Open and Close functions
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = "txt";
    ofn.lCustData = 0L;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFileName;
    ofn.lpstrFileTitle = szTitleName;
    ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
    if(GetOpenFileName(&ofn)== TRUE)
        return fopen(ofn.lpstrFile, "rb");
    else
        return NULL;
}
