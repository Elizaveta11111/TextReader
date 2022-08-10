#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <commdlg.h>
#include <math.h>
#include "menu.h"
#include "myinfo.h"
#include "dialog.h"
#include "fileinfo.h"
#include "lines.h"
#include "..\MyLib\mylib.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

typedef FILE* (*ReadFuncPtr)(LPARAM);

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow){
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style =  CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = szClassName;
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = sizeof(info*);                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)  GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VSCROLL,
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

FILE* OpenCommandLineFile(LPARAM lParam) {
    CREATESTRUCT* a = (CREATESTRUCT*) lParam;
    return fopen((char*)a->lpCreateParams, "rb");
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    HMENU hMenu;
    FILE* file;
    info* myInfo = (info*)GetWindowLongPtr(hwnd, 0);
    HDC hdc;
    static HMODULE dll = 0;
    static ReadFuncPtr pRF = NULL;
    long scrollines;
    long curLine;
    int lineBeg;
    char* linep;
    int i, x, y, iPaintBeg, iPaintEnd, iVscrollInc, iHscrollInc;

    PAINTSTRUCT ps;

    switch (message){
        case WM_CREATE:
            dll = LoadLibrary("MyLib.dll");

            if (!dll) {
                printf("Unable to load readerDLL.dll");
                PostQuitMessage(1);
            }
            pRF = (ReadFuncPtr)GetProcAddress(dll, "OpenCommandLineFile");
            if (!pRF) {
                printf("Unable to load procedure from the dll");
                PostQuitMessage(1);
            }

            myInfo = (info*)malloc(sizeof(info));
            SetWindowLongPtr(hwnd, 0, (LONG_PTR)myInfo);
            file = pRF(lParam);
            hdc = GetDC(hwnd);
            InitFileInfo(hdc, myInfo);
            if(file != NULL)
                GetFileInfo(file, myInfo, hdc);
            ReleaseDC(hwnd, hdc);
            break;
        case WM_COMMAND :
            hMenu = GetMenu(hwnd);
            switch(LOWORD(wParam)) {
                case IDM_OPEN :
                    hdc = GetDC(hwnd);
                    file = FileOpenDlg(hwnd);
                    if(file != NULL) {
                        FreeFileInfo(myInfo);
                        GetFileInfo(file, myInfo, hdc);
                        SendMessage(hwnd, WM_SIZE, 0, MAKELPARAM(myInfo->cxClient,myInfo->cyClient));
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    ReleaseDC(hwnd, hdc);
                    break;
                case IDM_CLOSE:
                    FreeFileInfo(myInfo);
                    SendMessage(hwnd, WM_SIZE, 0, MAKELPARAM(myInfo->cxClient,myInfo->cyClient));
                    break;
                case IDM_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                case IDM_WRAP:
                    CheckMenuItem(hMenu, IDM_UNWRAP, MF_UNCHECKED);
                    CheckMenuItem(hMenu, IDM_WRAP, MF_CHECKED);
                    myInfo->mode = 0;
                    SendMessage(hwnd, WM_SIZE, 0, MAKELPARAM(myInfo->cxClient,myInfo->cyClient));
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case IDM_UNWRAP:
                    hdc = GetDC(hwnd);
                    CheckMenuItem(hMenu, IDM_UNWRAP, MF_CHECKED);
                    CheckMenuItem(hMenu, IDM_WRAP, MF_UNCHECKED);
                    myInfo->mode = 1;
                    curLine = FindCurrentLine(myInfo);
                    linep = (myInfo->numLines == 0 ? myInfo->text : myInfo->lines[curLine]);
                    GetLinesUnwrap(myInfo, hdc);
                    myInfo->curLine = FindLine(myInfo->lines, linep);
                    SendMessage(hwnd, WM_SIZE, 0, 0);
                    InvalidateRect(hwnd, NULL, TRUE);
                    ReleaseDC(hwnd, hdc);
                    break;
                default :
                    break;
            }
            break;
        case WM_SIZE :
            myInfo->cxClient = LOWORD(lParam);
            myInfo->cyClient = HIWORD(lParam);

            if(myInfo->mode == 0) {
                myInfo->iMaxWidth = 0;
                curLine = FindCurrentLine(myInfo);
                lineBeg = myInfo->curLine - curLine;
                linep = (myInfo->numLines == 0 ? myInfo->text : myInfo->lines[curLine]);
                hdc = GetDC(hwnd);
                GetLinesWrap(myInfo, hdc);
                myInfo->curLine = FindLine(myInfo->lines, linep) + lineBeg;
                ReleaseDC(hwnd, hdc);
            }
            scrollines = myInfo->numLines - myInfo->cyClient / myInfo->cyChar;
            myInfo->scrolRange = scrollines/MAX_SCROL_POS + 1;
            myInfo->iVscrollMax = MAX(0, ceil(scrollines/(double)myInfo->scrolRange));
            myInfo->iVscrollPos = MIN(myInfo->curLine/myInfo->scrolRange, myInfo->iVscrollMax);

            SetScrollRange(hwnd, SB_VERT, 0, myInfo->iVscrollMax, FALSE);
            SetScrollPos(hwnd, SB_VERT, myInfo->iVscrollPos, TRUE);

            myInfo->iHscrollMax = MAX(0, 1 + (myInfo->iMaxWidth - myInfo->cxClient) / myInfo->cxChar);
            myInfo->iHscrollPos = MIN(myInfo->iHscrollPos, myInfo->iHscrollMax);

            SetScrollRange(hwnd, SB_HORZ, 0, myInfo->iHscrollMax, FALSE);
            SetScrollPos(hwnd, SB_HORZ, myInfo->iHscrollPos, TRUE);
            break;
        case WM_VSCROLL :
            scrollines = myInfo->numLines - myInfo->cyClient / myInfo->cyChar;
            switch(LOWORD(wParam)){
                case SB_TOP :
                    iVscrollInc = -myInfo->curLine;
                    break;
                case SB_BOTTOM :
                    iVscrollInc = scrollines - myInfo->curLine;
                    break;
                case SB_LINEUP :
                    iVscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    iVscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    iVscrollInc = MIN(-1, -myInfo->cyClient / myInfo->cyChar);
                    break;
                case SB_PAGEDOWN :
                    iVscrollInc = MAX(1, myInfo->cyClient / myInfo->cyChar);
                    break;
                case SB_THUMBTRACK :
                    iVscrollInc = HIWORD(wParam) - myInfo->iVscrollPos;
                    break;
                default :
                iVscrollInc = 0;
                }
            iVscrollInc = MAX(-myInfo->curLine, MIN(iVscrollInc, scrollines - myInfo->curLine));
            if (iVscrollInc != 0){
                myInfo->curLine += iVscrollInc;
                myInfo->iVscrollPos = floor(myInfo->curLine/myInfo->scrolRange);
                ScrollWindow(hwnd, 0, -myInfo->cyChar * iVscrollInc, NULL, NULL);
                SetScrollPos(hwnd, SB_VERT, myInfo->iVscrollPos, TRUE);
                UpdateWindow(hwnd);
            }
            break;
        case WM_HSCROLL :
            switch(LOWORD(wParam)) {
                case SB_LINEUP :
                    iHscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    iHscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    iHscrollInc = -8;
                    break;
                case SB_PAGEDOWN :
                    iHscrollInc = 8;
                    break;
                case SB_THUMBPOSITION :
                iHscrollInc = HIWORD(wParam) - myInfo->iHscrollPos;
                    break;
                default :
                    iHscrollInc = 0;
            }
            iHscrollInc = MAX(-myInfo->iHscrollPos, MIN(iHscrollInc, myInfo->iHscrollMax - myInfo->iHscrollPos));
            if (iHscrollInc != 0) {
                myInfo->iHscrollPos += iHscrollInc;
                ScrollWindow(hwnd, -myInfo->cxChar * iHscrollInc, 0, NULL, NULL);
                SetScrollPos(hwnd, SB_HORZ, myInfo->iHscrollPos, TRUE);
            }
            break;
        case WM_KEYDOWN :
            switch(wParam){
                case VK_HOME :
                SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0L);
                break;
                case VK_END :
                SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0L);
                break;
                case VK_PRIOR :
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);
                break;
                case VK_NEXT :
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
                break;
                case VK_UP :
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
                break;
                case VK_DOWN :
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
                break;
                case VK_LEFT :
                SendMessage(hwnd, WM_HSCROLL, SB_PAGEUP, 0L);
                break;
                case VK_RIGHT :
                SendMessage(hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L);
                break;
                }
 break;
        case WM_PAINT :
            hdc = BeginPaint(hwnd, &ps);
            iPaintBeg = MAX(0, myInfo->curLine + ps.rcPaint.top / myInfo->cyChar - 1);
            iPaintEnd = MIN(myInfo->numLines, myInfo->curLine + ps.rcPaint.bottom / myInfo->cyChar);
            iPaintEnd = (iPaintEnd == 0) ? -1 : iPaintEnd;

            for(i = iPaintBeg; i <= iPaintEnd; i++) {
                x = myInfo->cxChar *(-myInfo->iHscrollPos);
                y = myInfo->cyChar *(-myInfo->curLine + i);
                TextOut(hdc, x, y , myInfo->lines[i], myInfo->lines[i + 1] - myInfo->lines[i]);
            }
            EndPaint(hwnd, &ps);
            break;
        case WM_DESTROY:
            FreeFileInfo(myInfo);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
