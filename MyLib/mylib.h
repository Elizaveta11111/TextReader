#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <stdio.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

 /** \brief Opens file from command line
 *
 * \param[in] lParam Structure with command line arguments.
 * \return Opened file.
 */


FILE* __declspec(dllexport) OpenCommandLineFile(LPARAM lParam);

#endif // __MAIN_H__
