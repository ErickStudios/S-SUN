#if 0
/*
No pude incluirlo en el c normal asi que esta en S-SUN
*/
#ifndef _LIMA_LANGUAJE_H_
#define _LIMA_LANGUAJE_H_

#include <efi.h>
#include <efilib.h>
#include "KernelTextMode.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef struct {
		CHAR16* Name;
		CHAR16* Value;
	} variable;

	/*
	ExecuteCommand

	Summary:
		execute a lima command
	*/
	VOID
		ExecuteCommand
		(
			const CHAR16* Command
		)
	{
		// echo command
		if (
			(
				StrnCmp(Command, L"echo ", 5) == 0
				) // LOWERCASE
			|| (
				StrnCmp(Command, L"ECHO ", 5) == 0
			) // UPCASE
		)
		{
			printc(Command + 5);
			printc(L"\n");
			return;
		}

		// wriel command
		if (
			(
				StrnCmp(Command, L"writel ", 7) == 0
				) // LOWERCASE
			|| (
				StrnCmp(Command, L"WRITEL ", 7) == 0
				) // UPCASE
			)
		{
			printc(Command + 7);
			return;
		}

		// cls command
		if (
			StrCmp(Command, L"cls") == 0 // LOWERCASE
			|| StrCmp(Command, L"CLS") == 0 // UPCASE
			)
		{
			ClearScreen();
		}
	}

	/*
	ExecuteLimaScript

	Summary:
		execute lima script
	*/
	VOID
		ExecuteLimaScript
		(
			CHAR16 Script[],
			BOOLEAN Debug
		)
	{

		// check the script
		if (!Script) return;

		// the character count
		UINTN chcount = 0;

		// strlen
		while (Script[chcount])
		{
			chcount++;
		}

		// the script divided
		CHAR16 ScriptLines[700][50];

		// the lines count
		UINTN linescount = 0;

		// the subcharacter
		UINTN subcharactercount = 0;

		// divide the script in lines
		for (size_t i = 0; i < chcount; i++)
		{
			// if new line
			if (
				Script[i] == L'\n'
				|| Script[i] == L'\r'
				)
			{
				ScriptLines[linescount][subcharactercount++] = L'\0'; // terminate line
				linescount++; // next lins
				subcharactercount = 0; // reset the character to edit
			}
			else
				ScriptLines[linescount][subcharactercount++] = Script[i]; // add the character
		} // el usuario : enserio todavia no esta listo el script ?, ahhhhhhhh >:(

		// bueno ya

		// the variables
		variable variables[100];

		// execute the script
		for (size_t i = 0; i < linescount; i++) {
			if (
				ScriptLines[i][0] == L'#' // COMMENT
				) {
				continue;
			}
			// jump
			else if (
				(
					ScriptLines[i][0] == L'j' && ScriptLines[i][1] == L'u' && ScriptLines[i][2] == L'm' && ScriptLines[i][3] == L'p'
					) // uppercase
				|| (
					ScriptLines[i][0] == L'J' && ScriptLines[i][1] == L'U' && ScriptLines[i][2] == L'M' && ScriptLines[i][3] == L'P'
					) // lowercase
				) {
				CHAR16* section_to_search = ScriptLines[i + 1];

				// search the section
				for (size_t line_view = 0; line_view < linescount; line_view++)
				{
					if (
						StrCmp(ScriptLines[line_view], L"section") == 0 &&
						StrCmp(ScriptLines[line_view], section_to_search) == 0
						)
					{
						i = line_view;
						break;
					}
				}
			}
			else {
				ExecuteCommand(ScriptLines[i]);
			}
		}
	}
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !_LIMA_LANGUAJE_H_
#endif // ya tenemos S++ que va aparte alguien podria continuar con el proyecto
	   // mejor lo dejo asi por que lima ya es muy portable en html y turbowarp
