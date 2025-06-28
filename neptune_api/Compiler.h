/*++

SUNLib

File Name:

	Compiler.h

Summary:

	compilation for S-SUN programs

--*/

#ifndef SUNLibCompiler_H
#define SUNLibCompiler_H

#include <efi.h>
#include <efilib.h>

//
// como no , la mejor forma de que un programa pueda acceder a las funciones del sistema es basicamente
// incluir literalmente el header del Kernel en el programa
//

#include "../kernel/kernelMK.h"

#define DriverEntry(						\
					code,					\
					vendor,					\
					name					\
					)						\
											\
	Device32								\
	ProgramEntryName						\
	(										\
		EFI_HANDLE				ImageHandle \
		EFI_SYSTEM_TABLE*		SystemTable \
	)										\

#endif // !SUNLibCompiler_H
