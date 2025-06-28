/*++

SUNLib

File Name:

	Salitre.h

Summary:

	acess to command line to the non_system programs

--*/

#include "Compiler.h"

#ifndef _SALITRE_H_
#define _SALITRE_H_


//
// console colors
//
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL				consoleoutpudcurrentcolor;
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL				onsoleoutpudcurrentcolorbg;

INT8
ExecuteCommand
(
	CHAR16*											buffer,
	EFI_HANDLE										ImageHandle,
	EFI_SYSTEM_TABLE*								SystemTable,
	CHAR16											mode
);

VOID
ExecuteScript
(
	CONST CHAR16*									bufferf,
	EFI_HANDLE										ImageHandle,
	EFI_SYSTEM_TABLE*								SystemTable
);

#endif // !_SALITRE_H_

