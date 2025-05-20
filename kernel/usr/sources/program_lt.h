#ifndef _PROGRAM_LT_H_
#define _PROGRAM_LT_H_

#include <efi.h>
#include <efilib.h>

typedef struct {
	CHAR16* Name;
	VOID(*Function)();
} ThridyParterProgram;
#endif