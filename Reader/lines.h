#ifndef LINES_H_INCLUDED
#define LINES_H_INCLUDED

#include <windows.h>
#include "myinfo.h"

/** \brief Fills the array with pointers to the beginnings of strings and also calculate the maximum of strings lengths if the view mode is unwrap.
 *
 * \param[out] myInfo Pointer to the info structure. This function fills fields: lines, iMaxWidth, numLines.
 * \param[in] hdc Handle of the device context.
 * \details This function goes through text lines and adds it in array of strings if it begins with new line character.
 * To calculate string length it uses library function GetTextExtentPoint32.
*/

void GetLinesUnwrap(info* myInfo, HDC hdc);

/** \brief Fills the array with pointers to the beginnings of strings if the view mode is wrap.
 *
 * \param[out] myInfo Pointer to the info structure. This function fills fields: lines, numLines.
 * \param[in] hdc Handle of the device context.
 * \details This function goes through text lines and adds it in array of strings if it begins with new line character
 * or if the line reached the maximum length. To calculate string length it uses library function GetTextExtentPoint32.
*/

void GetLinesWrap(info* myInfo, HDC hdc);

/** \brief Gets index of beginning of line that is currently displayed on top of window.
 *
 * \param[in] myInfo Pointer to the info structure with text strings.
 * \returns Index of found line.
 * \details Goes through array from line which is currently displayed on top of the window to the beginning of text looking for first new line character.
*/

int FindCurrentLine(info* myInfo);

/** \brief Gets index of given line in given array.
 *
 * \param[in] lines Array with pointers.
 * \param[in] line Pointer that need to be found in array.
 * \returns Index of found line.
 * \details Goes through array elements comparing them to given pointer.
*/

int FindLine(char** lines, char* line);

#endif // LINES_H_INCLUDED
