#ifndef MYINFO_H_INCLUDED
#define MYINFO_H_INCLUDED

#define MAX(a, b) ((a)>(b))?(a):(b)

#define MIN(a, b) ((a)<(b))?(a):(b)

#define MAX_SCROL_POS 65535

/** \brief Structure containing information about the window
 *
 */

typedef  struct infoT {
    /// Buffer which contains current text
    char* text;
    /// Pointers to the beginnings of rows in the text
    char** lines;
    /// Size of the opened file
    long fileSize;
    /// Average size of letters
    int cxChar, cyChar;
    /// Size of client region
    int cxClient, cyClient;
    /// Line that is currently on top of the window
    int curLine;
    /// Positions of scrollbars
    int iVscrollPos, iHscrollPos;
    /// Range of scroll bars
    int iVscrollMax, iHscrollMax;
    /// Length of the biggest string in the text
    int iMaxWidth;
    /// Number of strings that needs to be scrolled for scrollbar to update
    int scrolRange;
    /// Number of strings in text
    long numLines;
    /// Defines if text will be shown wrapped or unwrapped
    int mode;// 0 - с версткой, 1 - без
}info;

#endif // MYINFO_H_INCLUDED
