#ifndef FILEINFO_H_INCLUDED
#define FILEINFO_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include "myinfo.h"

/** \brief Fills fields of info structure when window is created
 *
 * \param[in] hwnd Handle of the parent window.
 * \param[out] myInfo Pointer to the info structure
 * \details Sets view mode to unwrap.
*/

void InitFileInfo(HDC hdc, info* myInfo);

/** \brief Gets information about current file
 *
 * \param[in] file Pointer to the opened file
 * \param[out] myInfo Pointer to the info structure
 * \param[in] hdc Handle of the device context.
 * \details This function gets size of opened text, reads it into buffer and if the view mode is unwrap
 * it fills the array with pointers to the beginnings of strings of the text.
*/

void GetFileInfo(FILE* file, info* myInfo, HDC hdc);

/** \brief Frees fields of info structure
 *
 * \param[in] myInfo Pointer to the info structure
 * \details Frees memory that was allocated and removes scrollbars from window
*/

void FreeFileInfo(info* myInfo);

#endif // FILEINFO_H_INCLUDED
