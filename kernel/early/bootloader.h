/*++
bootloader.h

abstract:
	define macros and constants for the micro-bootloader of S-SUN or you S-SUN mod
--*/

#include "../console/KernelTextMode.h" // necesary for colors palette

#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

// os name for print
#define OS_NAME L"S-SUN"

// company name for print
#define COMPANY_NAME L"ErickCraftStudios"

#pragma region LoadingScreen

#define LND_BG_COLOR teal
#define LND_LBAR_BG bryellow
					// the moon color
#define LND_BAR_FILLED yellow
					// the sun color has been filled the moon because is a "new day" (new use)
#define LND_BAR_FILL_CH L"\a" // "\a" = for the cube character

#pragma endregion


#endif // _BOOTLOADER_H_
