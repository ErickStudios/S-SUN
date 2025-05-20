/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

	KernelOem.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

	for oem uses

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#ifndef OEM_LIB
#define OEM_LIB

#include <efi.h>
#include <efilib.h>

// ---------------------------------------------------------------------------------------------
// OEM definitions

#define IS_MOD							   0
										// set it for inicate a mod of S-SUN
										// this dont change none in the code
										// only shows that are using a custom version for 
										// for (x) OEM

#if !IS_MOD
#define OEMLabel L"Erick S-SUN OS"
				 // this is the complete name of the system
#else
#define OEMLabel L"your OS name here"
#endif

// ---------------------------------------------------------------------------------------------
// OEM tools

#define IN_CONFLIC_WITH_THE_OEM_VENDORS 0
										// set to 1 if a bad thing ocurred
										// debido a que soy un teenager esto me sirve
										// entonces si me hechan de ahi pues...bueno
										// sin ofender claro , fabricantes oem...
										// 
										// por favor terceros, si me hecharon los de oem
										// no distribuyan versiones sin la restrinccion
										// si S-SUN pasa momentos dificiles 

// ---------------------------------------------------------------------------------------------
// OEM compiler

#define IS_OEM_FOR_MANUFACTURE_COMPILATION 0
										// set this if there are a stable version

#define IS_A_STABLE_COMERCIAL_VERSION	   0
										// set if you have a stable version for upload to users
										// i cant use "Realese" by a error that makes that the
										// system never boots , so this is for a technical reason

#define REALESE (IS_A_STABLE_COMERCIAL_VERSION || IS_OEM_FOR_MANUFACTURE_COMPILATION)

#if REALESE
#pragma warm("WARNNING : this is a stable build , if you find errors dont publish this")
#endif

// ---------------------------------------------------------------------------------------------
// Original S-SUN values
#define OS_OEM_VER 10
#define OS_KERNEL_VER 2

// ---------------------------------------------------------------------------------------------
// S-SUN mods values
#define MOD_OS_OEM_VER 0
#define MOD_OS_KERNEL_VER 0

// ---------------------------------------------------------------------------------------------
// NORMALIZATION
#if IS_MOD
#define OS_VERSION (OS_OEM_VER + MOD_OS_OEM_VER)
#define OS_KERNEL_VERSION (OS_KERNEL_VER + MOD_OS_KERNEL_VER)
#else
#define OS_VERSION OS_OEM_VER
#define OS_KERNEL_VERSION OS_KERNEL_VER
#endif


// for idk
typedef struct {
	CHAR16* frv;

	BOOLEAN OemKeyNeedToSetNull;
} OEM_FACTRORIER;

OEM_FACTRORIER Oems[] = {
	{ L"American Megatrends", 1 },
	{ L"EDK ll", 1 },
	{ L"..." , 0 }
};

#define _NOT_OEM_STABLE &OEM_NOT_STABLE_MSG_TRANS

// xd

// ---------------------------------------------------------------------------------------------
// prototypes

BOOLEAN
OemCoppy
(
)
;

CHAR16*
OemVendor
(
)
;

BOOLEAN
OemKeyNeedToSetNullW
(
)
;

// ---------------------------------------------------------------------------------------------
// implementation

#if !IN_CONFLIC_WITH_THE_OEM_VENDORS // if pace and quiet with oem
BOOLEAN
OemCoppy
(
)
{
	return IS_OEM_FOR_MANUFACTURE_COMPILATION;
}

CHAR16*
OemVendor
(

)
{
	return gST->FirmwareVendor;
}

/*
OemKeyNeedToSetNullW

Summary:
	get if a uefi needs to reset the key for the resaon that the key is not set as NULL when
	you no press any key in some games that dont use the waitforevent for key input
*/
BOOLEAN
OemKeyNeedToSetNullW
(
)
{
	for (size_t i = 0; StrCmp(L"...", Oems[i].frv) == 0; i++)
	{
		if (StrCmp(gST->FirmwareVendor, Oems[i].frv) == 0) {
			return Oems[i].OemKeyNeedToSetNull;
		}
	}

	return 0;
}
#endif

#endif // !OEM_LIB
