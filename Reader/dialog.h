#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <windows.h>
#include <stdio.h>

/** \brief Function opens modal window which asks user to select a file
 *
 * \param[in] hwnd Handle of the parent window.
 * \return Pointer to the file that has been opened
 * \details Uses library function GetOpenFileName()
 */

FILE* FileOpenDlg(HWND hwnd);

#endif // DIALOG_H_INCLUDED
