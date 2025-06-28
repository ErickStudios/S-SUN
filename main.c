/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

	main.c

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

	the S-SUN operating system code

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include <efi.h>
#include <efilib.h>

#include "./kernel/include/motor.h"

#include "./kernel/licensesafety.h" // before make me anything , Tianocore , please view this file

//#include "./kernel/kernelMK.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// all of this in this section goes to the global data

// ----------------------------------------------------------------------------------------------
// constants

#define MAX_VARIABLES							100
#define SLEEPY_TIMEOUT							60 // in seconds
#define ssun_main(I,S,M)						Console(M)

// ----------------------------------------------------------------------------------------------
// structs

typedef struct {
	CHAR16*										variable;
	CHAR16*										value;
} VariableEntry;

// ----------------------------------------------------------------------------------------------
// tridy party functions for autimatization of the modifications of the system

#include "kernel/usr/programs.h"

// ----------------------------------------------------------------------------------------------
// variables
CHAR16*											languajecu;

//
// Schemes
//
CSCHEME*										SCDefault;
CSCHEME*										SCNature;
CSCHEME*										SCBoy;
CSCHEME*										SCGirl;
CSCHEME*										SCBilly;
CSCHEME*										SCFire;
CSCHEME*										SCOcean;
CSCHEME*										SCFuturistic;
CSCHEME*										SCDesert;
CSCHEME*										SCReactor;
CSCHEME*										SCNight;
CSCHEME*										SCSunDays;
CSCHEME*										SCHolidays;

//
// S++ colors
//
PIXELCOL										consoleoutpudcurrentcolor;
PIXELCOL										consoleoutpudcurrentcolorbg;

//
// (for compatibility) variables system
//
VariableEntry								variables[MAX_VARIABLES];
UINTN										variable_count = 0;

//
// file protocol
//
EFI_FILE_PROTOCOL*								CurrentDirectory = NULL;

//
//
// sleepy powers
BOOLEAN											SleepyPowers;
EFI_EVENT										AutoSleep;

//
// Variables storage
//
EFI_GUID										gEfiSimplePointerProtocolGuid = { 0x31878C87, 0x0B75, 0x11D5, { 0x9A, 0x4F, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D } };
EFI_GUID										VariablesGuid = { 0x5237f6a9, 0x14fa, 0x16bb, { 0xcb, 0x4d, 0xff, 0xb2, 0x3a, 0x51, 0x18, 0x90 } };

//
// multi task saves
//
string											EditorProcess[1024];

//
// S++ dir functions
//
INT8											CurrentDir;

//
// the original params but globalized
//
extern EFI_SYSTEM_TABLE*						globalsystemtable;
extern EFI_HANDLE								globalimagehandle;

// ----------------------------------------------------------------------------------------------
// functions

//
// modern functions
//
prototype VOID
DrawLogo
(
	INT16										x,
	INT16										y,
	INT8										Brightness
);

prototype VOID
DrawLoadingCircle
(
	INT16										x,
	INT16										y,
	INT16										eframe
);

//
// autosleep functions
//

prototype VOID
AutoSleepInitialize
(
)
;

prototype VOID
SleepyPActive
(
	EFI_EVENT									Event, 
	VOID*										Context
)
;

//
// languaje functions
//

prototype VOID
UpdateLanguaje
(
);

//
// string functions
//

prototype CHAR16*
StrStr
(
	CHAR16*										haystack,
	CHAR16*										needle
);

prototype CHAR16*
StrToken
(
	CHAR16*										str,
	const CHAR16*								delim
);

prototype CHAR16**
SplitLines
(
	CHAR16*										text,
	UINTN*										line_count
);

//
// print organized functions
//

prototype VOID
PrintLineWithBackground
(
	EFI_SYSTEM_TABLE*							SystemTable,
	string*										text,
	UINTN										line,
	UINTN										COLOR,
	UINTN										BG
);

prototype void
PrintSSL
(
	string*										text,
	UINTN										column,
	PIXELCOL									tc,
	PIXELCOL									bc,
	UINTN*										size
);

//
// S++ interpreter
//

prototype CHAR16*
SPP_SYNTAX
(
	CHAR16*									buffer
);

prototype INT8
ExecuteCommand
(
	string*									buffer,
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable,
	string									mode
);

prototype MINI_PROGRAM
ExecuteScript
(
	CONST CHAR16*							bufferf,
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable
);

//
// programs
//

prototype EFI_STATUS
SystemManager
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
);

prototype CHAR16*
editor
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable,
	string* textt
);

prototype PROGRAM
Console
(
	CHAR16									mode
);

prototype PROGRAM
Calculator
(
	CSCHEME*								Colors
);

prototype PROGRAM
BITMAP_MAKER
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable
);

prototype GAME
SNAKE
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable
);

prototype GAME
Fisher
(
	CSCHEME*								Colors
);

prototype GAME
Kosilka
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable
);

prototype GAME
ADIVINA_EL_NUMERO
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable
);

prototype CSCHEME*
SchemeEditor
(
	CSCHEME*								SchemeToEdit
);

prototype PROGRAM
Desktop
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable
);

prototype PROGRAM
EmergencyPrompt
(
);

//
// the entry point
//

prototype PROGRAM
efi_main
(
	EFI_HANDLE								ImageHandle,
	EFI_SYSTEM_TABLE*						SystemTable
);
// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

/*
erick : y si hago un siste...
comunidad c : NOOOO
PAM , S-SUN creado
*/

// my variable guid
// EFI_GUID SmallVariables = { 0x52375679, 0x1435, 0x1634, { 0xbb, 0xad, 0xaf, 0x22, 0x3a, 0x51, 0x18, 0x90 } };

// Random
/**
Summary:
this function make a random worcking for
the other random functions
**/
INTN Random(INTN seed)
{
	// Algoritmo de generación simple (Linear Congruential Generator - LCG)
	return (seed * 1103515245 + 12345) & 0x7FFFFFFF;
}

// RandomInRangeINWORCK_MANUALLY
/**
Summary:
this functions is worck of RandomInRangeFINALLY
**/
INTN RandomInRangeINWORCK_MANUALLY(INTN seed, INTN min, INTN max)
{
	UINT32 rand = Random(seed);
	if (min == max) return min;
	return min + (rand % (max - min + 1));
	
}

// RandomInRange
/**
Summary:
this function gets the system time second and use
it for get a random number in a range
**/
INTN RandomInRange(INTN min, INTN max)
{
	// ALGORITMMMMMMMM is the algoritm tttttttttttttttt
	EFI_TIME ALGORITMMMMMMMM;
	globalsystemtable->RuntimeServices->GetTime(&ALGORITMMMMMMMM, NULL);

	// siiiiid is the seed
	INTN siiiiid = ALGORITMMMMMMMM.Second;

	return RandomInRangeINWORCK_MANUALLY(siiiiid, min, max);
}

bool_t
IsInArrayRange
(
	ch16                                                    Text[],
	u32                                                     ArrayLen
)
{
	if (
		Text == NULL
		)
	{
		ShowPanic(NULL_CHAR_POINTER);
	}

	u32														lenght = 0;

	while (Text[lenght])
	{
		lenght++;
	}

	if (
		lenght > ArrayLen
		)
	{
		ShowPanic(SE_FUE_A_LA_BORDA);
	}
	return 1;
}
// SumList
/**
Summary:
this functions sums a list
**/
INTN SumList(INTN list[], UINTN size)
{
	INTN sum = 0;
	for (size_t i = 0; i < size; i++)
	{
		sum += list[i];
	}

	return sum;
}

static VOID updatefilesystem() {
	// Asignar memoria para la estructura

	// Guardar en la NVRAM
	EFI_STATUS Status = StGlobVar(
		L"SSUNState",         // Nombre de la variable
		&VariablesGuid,         // GUID único
		sizeof(CurrentFS),      // Tamaño de los datos
		CurrentFS              // Puntero a los datos
	);

	// Manejar errores
	if (EFI_ERROR(Status)) {
		CHAR16 buffera[256];
		StatusToString(buffera, Status);
		printc(buffera);
	}
	else {
		printc(L"FileSytem Saved\n");
	}
}

/*
ShowPanic

Summary:
	panic pan
*/
VOID
ShowPanic
(
	INT32 code
) 
{
		INT32 x = (((GET_MAX_COL)-25) / 2);
		INT32 y = (((GET_MAX_ROWS)-12) / 2);

		SetScreenAtribute(1, black);
		ClearScreen();

		DrawLogo(x, y, 3);

		DrawScreen();

		gotoxy(x, y + 7);

		SetScreenAtribute(0, bryellow);
		printc(L"S-SUN: ");

		SetScreenAtribute(0, brblue);
		printc(L"AHHh SYS ...SYS?\x2763");

		gotoxy(x, y + 10);

		SetScreenAtribute(0, bryellow);
		printc(L"User: ");

		SetScreenAtribute(0, brgreen);
		printc(L"what happen?");

		gotoxy(x, y + 12);

		SetScreenAtribute(0, bryellow);
		printc(L"S-SUN: ");

		SetScreenAtribute(0, brblue);
		printc(L"the system has been \x2763 crashed");

		gotoxy(x, y + 15);

		SetScreenAtribute(0, bryellow);
		printc(L"S-SUN: ");

		SetScreenAtribute(0, brcyan);
		CHAR16 coder[10];
		SPrint(coder, sizeof(coder), L"0x%x", code);
		printc(coder);

		SetScreenAtribute(0, red);
		printc(L" !! >:(");

		/*
		if (IS_OEM_FOR_MANUFACTURE_COMPILATION) {
			gBS->Stall(1000000);
			SetScreenAtribute(0, darkred);
			printc(L"AHHHHHHHHHHHHHHHHHH");
			gBS->Exit(globalimagehandle, EFI_INVALID_PARAMETER, 0, L"");
		}*/

		gotoxy(x, y + 16);

		SetScreenAtribute(0, brcyan);
		printc(L"\x2763press ctrl+alt+del\x2763or enter to continue");

		gotoxy(x, y + 16);

		SetScreenAtribute(0, brcyan);
		while (true)
		{
			gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, 1);

			EFI_INPUT_KEY Key;

			gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

			if (
				Key.UnicodeChar == CHAR_CARRIAGE_RETURN
				)
			{
				break;
			}
		}

}

VOID
UpdateLanguaje
(
)
{
	LibSetNVVariable(L"Lang", &SmallVariables, sizeof(languajecu), languajecu);
}


CHAR16*
 StrStr
 (
	CHAR16* haystack, CHAR16* needle
)
{
	if (!*needle) return haystack;
	for (; *haystack; haystack++) {
		if (*haystack != *needle) continue;
		CHAR16* h = haystack, * n = needle;
		for (; *h && *n; h++, n++) {
			if (*h != *n) break;
		}
		if (!*n) return haystack;
	}
	return NULL;
}

EFI_STATUS
SSUNSCREENLOGOUT
(
	EFI_HANDLE ImageHandle,
	 EFI_SYSTEM_TABLE* SystemTable
)
{
	SetScreenAtribute(1,black);
	ClearScreen();
	INT8 loadingframe = 0;
	for (size_t i = 0; i < 21; i++)
	{
		ClearScreen();
		loadingframe++;
		DrawLoadingCircle((((GET_MAX_COL)-3) / 2), (((GET_MAX_ROWS)-2) / 2), loadingframe);

		gotoxy((((GET_MAX_COL)-StrLen(TranslateWorck(&LOGOUT_MSG_TRANS, languajecu))) / 2), (((GET_MAX_ROWS)-2) / 2) + 4);
		printc(TranslateWorck(&LOGOUT_MSG_TRANS, languajecu));

		if (
			loadingframe > 7
			)
		{
			loadingframe = 0;
		}

		gBS->Stall(150000);
	}

	/*
	initializeMoonScreen();
	SetScreenAtribute(1, LND_BG_COLOR);
	ChangeToTextMode();
	ClearScreen();
	globMaxRows = (verticalResolution / row_size) / Conio->atributes->size;
	globMaxColumns = (horizontalResolution / 8) / Conio->atributes->size;

	// draw the presentation by-creator
	gotoxy((globMaxColumns - StrLen(COMPANY_NAME)) / 2, globMaxRows - 1);
	printc(COMPANY_NAME);

	// draw the system name
	gotoxy((globMaxColumns - StrLen(OS_NAME)) / 2, globMaxRows / 2);
	printc(OS_NAME);

	// draw the instruccions
	gotoxy((globMaxColumns - StrLen(TranslateWorck(&LOGOUT_MSG_TRANS, languajecu))) / 2, (globMaxRows / 2) + 4);
	printc(TranslateWorck(&LOGOUT_MSG_TRANS,languajecu));

	// set the colors for the loading bar
	gotoxy((globMaxColumns - 30) / 2, (globMaxRows / 2) + 2);
	SetScreenAtribute(0, LND_BAR_FILLED);

	// draw the loading bar
	printc(L"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");

	gotoxy((globMaxColumns - 30) / 2, (globMaxRows / 2) + 2);
	// the animation
	for (INT8 i = 0; i < 30; i++)
	{
		// set the loading dots colors
		SetScreenAtribute(1, LND_BG_COLOR);
		SetScreenAtribute(0, LND_LBAR_BG);
		// draw the dot
		printc(L"\a");
		// wait for make it a animation
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
	}
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);*/
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
	
}

// Función para dividir una cadena CHAR16 en líneas
CHAR16**
SplitLines
(
	CHAR16* text,
	UINTN* line_count
)
{
	CHAR16* lines[2048];
	*line_count = 0;
	CHAR16* token = StrToken(text, L"\n");
	while (token != NULL && *line_count < 100) {
		lines[(*line_count)++] = token;
		token = StrToken(NULL, L"\n");
	}
	return lines;
}

// Implementación de StrToken
string*
StrToken
(
	string* str,
	const string* delim
)
{
	static CHAR16* static_str = NULL; // Mantiene el puntero de la posición en la cadena original
	if (str) static_str = str;
	if (!static_str) return NULL;

	CHAR16* start = static_str;
	while (*static_str) {
		const CHAR16* d = delim;
		while (*d) {
			if (*static_str == *d) {
				*static_str = 0;
				static_str++;
				if (start != static_str - 1)
					return start;
				else {
					start++;
					break;
				}
			}
			d++;
		}
		static_str++;
	}
	static_str = NULL;
	return start != static_str ? start : NULL;
}

VOID
PrintLineWithBackground
(
	EFI_SYSTEM_TABLE* SystemTable, 
	string* text,
	UINTN line, 
	UINTN COLOR, 
	UINTN BG
) 
{
	UINTN Column, MaxColumns;
	string* lineBuffer;
	UINTN textLength = StrLen(text);

	// Obtener el número máximo de columnas de la pantalla
	SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &MaxColumns, NULL);

	MaxColumns = gop->Mode->Info->VerticalResolution / 8;

	// Crear un buffer para la línea completa
	lineBuffer = AllocatePool((MaxColumns + 1) * sizeof(CHAR16));
	if (!lineBuffer) {
		return;
	}

	// Copiar el texto al buffer y llenar el resto con espacios en blanco
	StrCpy(lineBuffer, text);
	for (Column = textLength; Column < MaxColumns; Column++) {
		lineBuffer[Column] = L' ';
	}
	lineBuffer[MaxColumns] = L'\0';

	// Establecer el atributo de color de fondo
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, COLOR | BG);

	// Mover el cursor a la línea especificada
	SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, 0, line);

	// Escribir la línea con el fondo de color
	Print(lineBuffer);

	// Liberar el buffer
	FreePool(lineBuffer);
}

void
PrintSSL
(
	string* text,
	UINTN column,
	PIXELCOL tc,
	PIXELCOL bc,
	UINTN* size
)
{
	SetScreenAtribute(0,tc);
	SetScreenAtribute(1, bc);

	UINTN MaxColumns;
	CHAR16* lineBuffer;
	UINTN* textLength = StrLen(text);

	// Obtener el número máximo de columnas de la pantalla
	MaxColumns = gop->Mode->Info->VerticalResolution / 4;

	gotoxy(0, column);
	for (UINTN Column = 0; Column < size; Column++) {
		printc(text);
	}
}

EFI_STATUS 
SystemManager
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	EFI_INPUT_KEY Key;
	EFI_STATUS Status;
	UINTN Event;

	InitializeLib(ImageHandle, SystemTable);

	MEM_FILE_INT* SYMGR_INSTANCE = Create_MEM_FILE_INT(L"SYMGR_INSTANCE", 1);
	while (TRUE)
	{
		WHILESYSTEMRUNNING(
			EFI_TIME Time;

		ClearScreen();
		SetScreenAtribute(0, black);
		SetScreenAtribute(1, white);
		printc(L"S-SUN system manager");

		SetScreenAtribute(0, white);
		SetScreenAtribute(1, black);
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
		printc(L"\nPlease select an option\n\n");
		printc(L"<F1> Exit\n");
		printc(L"<F2> Get the time\n");
		printc(L"<F3> Get the date\n");
		printc(L"<F4> About S-SUN\n");
		printc(L"<F5> editor\n");

		uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);

		if (Key.ScanCode == SCAN_F1)
		{
			return EFI_SUCCESS;
		}
		else if (Key.ScanCode == SCAN_F2)
		{
			DRAW_TEXT_DIALOG(ImageHandle, SystemTable, KERNEL_GET_TIME(SystemTable), EFI_WHITE, EFI_BACKGROUND_CYAN);
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
		}
		else if (Key.ScanCode == SCAN_F3)
		{
			DRAW_TEXT_DIALOG(ImageHandle, SystemTable, KERNEL_GET_DATE(SystemTable), EFI_WHITE, EFI_BACKGROUND_CYAN);
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
		}
		else if (Key.ScanCode == SCAN_F4)
		{
			ClearScreen();
			SetScreenAtribute(0, black);
			SetScreenAtribute(1, white);
			printc(SystemTable, L"S-SUN - about S-SUN");

			SetScreenAtribute(0, white);
			SetScreenAtribute(1, black);
			printc(L"\nENGLISH\n\n");
			printc(L"S-SUN is a very small Operating System maded by ErickCraftStudios with C and Visual Studio with help of a template (uefi-simple: https://github.com/pbatard/uefi-simple) \n");
			printc(L"\n\nESPAÑOL\n\n");
			printc(L"S-SUN es un sistema operativo muy pequeño hecho por ErickCraftStudios con C y Visual Studio con ayuda de una plantilla (uefi-simple: https://github.com/pbatard/uefi-simple) \n");
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);

		}
		else if (Key.ScanCode == SCAN_F5)
		{
			editor(ImageHandle, SystemTable, L"");
		}
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
			);
	}
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	Free_MEM_FILE_INT(SYMGR_INSTANCE);
	return EFI_SUCCESS;
}

EFI_STATUS 
CreateANewDirFUNCTION
(
	CHAR16* DirName,
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable
) 
{
	if (DirName == NULL || ImageHandle == NULL || SystemTable == NULL) {
		Print(L"KERNEL_CALL -> -> Null Param\n");
		uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
		return EFI_INVALID_PARAMETER;
	}

	EFI_FILE_PROTOCOL* Root;
	EFI_FILE_PROTOCOL* NewDir;
	EFI_LOADED_IMAGE* LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

	CHAR16* msg[100];

	EFI_STATUS Status;

	// Obtener la interfaz Loaded Image
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &LoadedImageProtocol, (void**)&LoadedImage);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"Status getting the image protocol: %r", Status);
		pr_special(msg);
		uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	// Obtener la interfaz de sistema de archivos
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, LoadedImage->DeviceHandle, &FileSystemProtocol, (void**)&FileSystem);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"error on get filesystem protocol: %r", Status);
		pr_warn(msg);
		uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	// Abrir la raíz del sistema de archivos
	Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"error openning the root: %r", Status);
		pr_warn(msg);
		uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	// Crear el nuevo directorio
	Status = uefi_call_wrapper(Root->Open, 5, Root, &NewDir, DirName, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, EFI_FILE_DIRECTORY);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"error creating the file %s: %r", DirName ,Status);
		pr_warn(msg);
		uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	uefi_call_wrapper(NewDir->Close, 1, NewDir);
	return EFI_SUCCESS;
}

CHAR16*
ProcessText
(
	CHAR16* A
)
{
	return A;
}

/*
SPP_SYNTAX:

Syntax use:
	MEMS Syntax:
		%(MEM): get a MEM_FILE_STRING or a MEM_FILE_INT
		def%(MEM): verific if a MEM file exist (can be "def" or "DEF")
		X%(MEM_INT) get a MEM_FILE_INT		(can use the two caps in the "X")
					in hex withut the "0x"

	STRICT MEM syntax:
		S%(MEM_STRING) get a MEM_STRING 		(can use the two caps in the "S")
		D%(MEM_STRING) get a MEM_INT			(can use the two caps in the "D")

	COMPARATORS:
		cmp%(syntax1)=(syntax2): verific if syntax  (can be "cmp" or "CMP")
		opr&(syntax1)(op +/-)(syntax2): make it a add or sub (can be "opr" or "OPR")
		&key_and_wait : ...

	S-FRAMEWORCK:
		&confirm (Message): shows a YESS or NO window popup

	ELSE:
		returns the text
*/
CHAR16* 
SPP_SYNTAX
(
	CHAR16* buffer
) 
{
	// input variables
	EFI_INPUT_KEY Key;
	UINTN Event;

	// Get-Hex MEM
	if (
		(buffer[0] == L'x' || buffer[0] == L'X')
		&& buffer[1] == L'%'
		) {
		CHAR16* processmemfile = buffer + 2;

		for (UINTN i = 0; i < MemFileCountInt; i++) {
			if (StrCmp(MemFilesInt[i]->NAME, processmemfile) == 0)
			{
				string* b;
				ValueToHex(b, MemFilesInt[i]->VALUE);
				return b;
				;
			}
		}

		return L"Not Founded";
	}
	// key and wait
	if (
		StrCmp(buffer, L"&key_and_wait")
		== 0
		) {
		// Esperar eventos de teclado
		globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
		globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);

		return KeyToString(Key);
	}
	// Get-String MEM
	if (
		(buffer[0] == L's' || buffer[0] == L'S') 
		&& buffer[1] == L'%'
		) {
		CHAR16* processmemfile = buffer + 2;

		for (UINTN i = 0; i < MemFileCountString; i++) {
			if (StrCmp(MemFilesString[i]->NAME, processmemfile) == 0)
			{
				return MemFilesString[i]->VALUE;
				;
			}
		}

		return L"Not Founded";
	}
	// Get-Int MEM
	if (
		(buffer[0] == L'd' || buffer[0] == L'D')
		&& buffer[1] == L'%'
		) {
		CHAR16* processmemfile = buffer + 2;

		for (UINTN i = 0; i < MemFileCountInt; i++) {
			if (StrCmp(MemFilesInt[i]->NAME, processmemfile) == 0)
			{
				string* b;
				ValueToString(b, false, MemFilesInt[i]->VALUE);
				return b;
			}
		}

		return L"Not Founded";
	}
	// Get MEM
	else if (
		buffer[0] 
		== L'%'
		) {
		CHAR16* processmemfile = buffer + 1;

		for (UINTN i = 0; i < MemFileCountString; i++) {
			if (StrCmp(MemFilesString[i]->NAME,processmemfile) == 0) 
				return MemFilesString[i]->VALUE
			;
		}
		for (UINTN i = 0; i < MemFileCountInt; i++) {
			if (StrCmp(MemFilesInt[i]->NAME, processmemfile) == 0)
			{
				string* b;
				ValueToString(b, false, MemFilesInt[i]->VALUE);
				return b;
			}
		}

		return L"Not Founded";
	}
	// comparator
	else if (
		((buffer[0] == L'c' && buffer[1] == L'm' && buffer[2] == L'p') ||
			(buffer[0] == L'C' && buffer[1] == L'M' && buffer[2] == L'P')) &&
		buffer[3] == L'&'
		) {
		CHAR16* command = buffer + 4;
		CHAR16* equals_pos = StrStr(command, L"=");
		if (equals_pos != NULL) {
			*equals_pos = L'\0';
			CHAR16* MemName = command;
			CHAR16* NewValue = equals_pos + 1;

			CHAR16* VALUE1 = SPP_SYNTAX(MemName);
			CHAR16* VALUE2 = SPP_SYNTAX(NewValue);

			if (StrCmp(VALUE1, VALUE2) == 0) {
				return L"1";
			}
			else
			{
				return L"0";
			}
		}
		return L"invalid comparation";
	}
	// oprerator
	else if (
		((buffer[0] == L'o' && buffer[1] == L'p' && buffer[2] == L'r') ||
			(buffer[0] == L'O' && buffer[1] == L'P' && buffer[2] == L'R')) &&
		buffer[3] == L'&'
		) {
		CHAR16* command = buffer + 4;
		CHAR16* operatorpos;

		operatorpos = StrStr(command, L"+");
		if (operatorpos != NULL) {
			*operatorpos = L'\0';
			CHAR16* MemName = command;
			CHAR16* NewValue = operatorpos + 1;

			CHAR16* VALUE1 = SPP_SYNTAX(MemName);
			CHAR16* VALUE2 = SPP_SYNTAX(NewValue);

			string* b[20]; // Aumentar el tamaño para valores grandes

			ValueToString(b, false, Atoi(VALUE1) + Atoi(VALUE2));
			return b;
		}
		else {
			operatorpos = StrStr(command, L"-");
			if (operatorpos != NULL) {
				*operatorpos = L'\0';
				CHAR16* MemName = command;
				CHAR16* NewValue = operatorpos + 1;

				CHAR16* VALUE1 = SPP_SYNTAX(MemName);
				CHAR16* VALUE2 = SPP_SYNTAX(NewValue);

				string* b[20];
				ValueToString(b, false, Atoi(VALUE1) - Atoi(VALUE2));
				return b;
			}
		}
		return L"invalid comparation";
	}
	// defined MEM syntax
	else if (
		((buffer[0] == L'd' && buffer[1] == L'e' && buffer[2] == L'f') ||
		(buffer[0] == L'D' && buffer[1] == L'E' && buffer[2] == L'F')) &&
		buffer[3]
		== L'%'
		) {
			CHAR16* processmemfile = buffer + 4;
			if (Read_MEM_FILE_INT(processmemfile) != -100) {
				return L"1";
			}
			else {
				if (Read_MEM_FILE_STRING(processmemfile) != NULL) {
					return L"1";
				}
			}
			return L"0";
		}
	// Confirm
	else if (
		StrnCmp(buffer, L"&confirm ",9) 
		== 0
		) {
		CHAR16* processmsg = buffer + 9;

		PIXELCOL syscolor2 = Sceme->buttonscolor;
		PIXELCOL sysbg = Sceme->backgroundcolor;
		PIXELCOL sysbtext = Sceme->buttonstext;
		EFI_INPUT_KEY Key;
		UINTN Event;
		bool confirm = DRAW_DIALOG_MSG_CONFIRM(globalimagehandle, ST, processmsg, syscolor2, sysbtext);
		if (confirm == true) {
			return L"1";
		} else if (confirm == false) {
			return L"0";
		}
	}
	// returns the text if the syntax is invalid
	return buffer;
}

/*
S-SUN editor

Summary:
	the S-SUN editor is a text editor for create documments, programs and texts
*/
CHAR16*
editor
(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable, 
	CHAR16* textt
) 
{
	/*
	for (size_t i = 0; i < 1024; i++) {
		if (textt[i] == L'\0') break; // Si se encuentra el carácter nulo, salir del bucle.
		if (i == 1022 && textt[i + 1] != L'\0') { // Chequear específicamente en la última posición.
			printc(L"error no null character detected\nView the content:\n");
			for (size_t j = 0; j < 1024; j++) {
				CHAR16 e[2] = { textt[j], L'\0' }; // Asegurar que sea imprimible.
				printcu(e);
			}
			DrawScreen(); // Actualizar la pantalla.
			return 0; // Salir indicando que hubo un problema.
		}
	}
	*/

	// buffers of editor
	CHAR16 text[1024] = L""; // Definir un tamaño suficiente para el búfer de entrada
	CHAR16 save_text[1024]; // Búfer para guardar el texto

	printc(L"calculating the lenght");
	// the index
	UINTN Index;
	if (textt != NULL) {
		for (size_t i = 0; i < 1024; i++)
		{
			if (textt[i] == L'\0') break;
			Index++;
		}
	}
	
	// the input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	// the coppy char
	CHAR16* cpy = L"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	u8	buffercpy_bord = 0;

	// screen size
	UINTN MaxColumns, MaxRows;

	// the survival guide alternate
	bool SurvivalGuideAlt = 0;

	// the scroll of the editor
	INTN EditorScroll;

	EditorScroll = 0;

	printc(L"try to restore the backup or open the file");

	// load the file or not load the file that is the question
	if (*EditorProcess == NULL && textt != NULL) {
		StrCpy(&text, textt);
		Index = StrLen(text);
	}
	else
	{
		StrCpy(&text, EditorProcess);
		Index = StrLen(text);
	}
	INTN scrolla;

	PIXELCOL editorbg = black;
	PIXELCOL editorcol1 = Sceme->buttonscolor;
	PIXELCOL editorcol2 = Sceme->buttonstext;

	// editor main loop
	while (TRUE) {
		// parse the screen size
		MaxRows = (verticalResolution / row_size) / Conio->atributes->size;
		MaxColumns = (horizontalResolution / 8) / Conio->atributes->size;

		// set the atributes
		SetScreenAtribute(0, editorcol1);
		SetScreenAtribute(1, editorbg);
		ClearScreen();

		// reset the cursor
		gotoxy(0, 0);
		SetScreenAtribute(1, editorcol1);
		SetScreenAtribute(0, editorcol2);

		// print the upper menu without update the screen
		printcu(taskbar);
		gotoxy(0, 0);
		printcu(TranslateWorck(&SSUNEDITOR_UPPER_BAR_TRANSL, languajecu));

		// set the space
		SetScreenAtribute(0, editorcol1);
		SetScreenAtribute(1, editorbg);
		printcu(L"\n\n\n");

		// print the footer bar
		gotoxy(0, MaxRows - 1);
		SetScreenAtribute(1, editorcol1);
		SetScreenAtribute(0, editorcol2);
		printcu(taskbar);
		gotoxy(0, MaxRows - 1);
		printcu(TranslateWorck(&SSUNEDITOR_LOWER_BAR_TRANSL, languajecu));
		SetScreenAtribute(0, gray);
		SetScreenAtribute(1, black);

		// go to text pos
		gotoxy(0, 4);

		// coppy a coppy of the text
		StrCpy(save_text, text);

		// lines count
		UINTN line_count1 = 0;
		CHAR16** lines1 = SplitLines(text, &line_count1);

		// real scroll
		scrolla = line_count1 - ((MaxRows - 10) - EditorScroll);

		// verific the scroll
		if (scrolla < 0) {
			scrolla = 0;
		}
		
		// if the survivalguide is alternate show if not show the text
		if (SurvivalGuideAlt == 0) {
			for (UINTN i2 = scrolla; i2 < line_count1; i2++) {
				SetScreenAtribute(0, gray);

				if (StrCmp(lines1[i2], L"catch") == 0) {
					SetScreenAtribute(0, brblue);
					printcu(lines1[i2]);
				} 
				else if (StrCmp(lines1[i2], L"end catch") == 0) {
					SetScreenAtribute(0, brblue);
					printcu(lines1[i2]);
				}
				else if (StrnCmp(lines1[i2], L"section::",9 ) == 0) {
					SetScreenAtribute(0, brblue);
					printcu(lines1[i2] + 9);
				}
				else if (StrCmp(lines1[i2], L"LSMEM") == 0) {
					SetScreenAtribute(0, green);
					printcu(lines1[i2]);
				}
				else if (StrCmp(lines1[i2], L"ls") == 0) {
					SetScreenAtribute(0, green);
					printcu(lines1[i2]);
				}
				else if (StrCmp(lines1[i2], L"GUI") == 0) {
					SetScreenAtribute(0, green);
					printcu(lines1[i2]);
				}
				else if (StrnCmp(lines1[i2], L"echo ",5) == 0) {
					SetScreenAtribute(0, bryellow);
					printcu(L"echo ");
					SetScreenAtribute(0, brorange);
					printcu(lines1[i2] + 5);
				}
				else if (StrnCmp(lines1[i2], L"writel ", 7) == 0) {
					SetScreenAtribute(0, bryellow);
					printcu(L"writel ");
					SetScreenAtribute(0, brorange);
					printcu(lines1[i2] + 7);
				}
				else if (StrnCmp(lines1[i2], L"edit ", 5) == 0) {
					SetScreenAtribute(0, brgreen);
					printcu(L"edit ");
					SetScreenAtribute(0, brorange);
					printcu(lines1[i2] + 5);
				}
				else if (StrnCmp(lines1[i2], L"GetMem ", 7) == 0) {
					SetScreenAtribute(0, bryellow);
					printcu(L"GetMem ");
					SetScreenAtribute(0, brgreen);
					printcu(lines1[i2] + 7);
				}
				else if (StrnCmp(lines1[i2], L"#", 1) == 0) {
					SetScreenAtribute(0, green);
					printcu(L"#");
					printcu(lines1[i2] + 1);
				}
				else if (StrnCmp(lines1[i2], L"ReadLine ", 9) == 0) {
					SetScreenAtribute(0, brcyan);
					printcu(L"ReadLine ");
					SetScreenAtribute(0, brgreen);
					printcu(lines1[i2] + 9);
				}
				else if (StrCmp(lines1[i2], L"section") == 0) {
					SetScreenAtribute(0, blue);
					printcu(L"section");
				}
				else if (StrCmp(lines1[i2 - 1], L"section") == 0) {
					SetScreenAtribute(0, brcyan);
					printcu(lines1[i2]);
				}
				else if (StrnCmp(lines1[i2], L"call ", 5) == 0) {
					SetScreenAtribute(0, brred);
					printcu(L"call ");
					SetScreenAtribute(0, brcyan);
					printcu(lines1[i2] + 5);
				}
				else if (StrnCmp(lines1[i2], L"EditMem ", 8) == 0) {
					SetScreenAtribute(0, brcyan);
					printcu(L"EditMem ");
					CHAR16* equals_pos = StrStr(lines1[i2], L"=");
					if (equals_pos != NULL) {
						*equals_pos = L'\0';
						CHAR16* MemName = lines1[i2] + 8;
						CHAR16* NewValue = equals_pos + 1;

						SetScreenAtribute(0, brgreen);
						printcu(MemName);
						SetScreenAtribute(0, gray);
						printcu(L"=");
						SetScreenAtribute(0, brblue);
						printcu(NewValue);
					}
					else
					{
						SetScreenAtribute(0, darkred);
						printcu(lines1[i2] + 8);
					}
				}
				else
				{
					SetScreenAtribute(0, gray);
					printcu(lines1[i2]);
				}

				if (i2 + 1 != line_count1) {
					printcu(L"\n");
				}
			}
			SetScreenAtribute(0, gray);
			printcu(L"\a");
		}
		else {
			/*
			Section:: ."S-SUN editor survival guide"

			Text:: ."S-SUN editor is a powerful tool for making documents
			or programs."

			Section:: ."How To Use"

			Text:: ."use it as another editor, to make programs press F5
			
			NOTE: you can't save your file
			so don't make big programs"
			*/
			// section "S-SUN editor survival guide"
			SetScreenAtribute(0, brblue);
			SetScreenAtribute(1, black);
			printcu(TranslateWorck(&SSUNEDSECTION1_TRANSL, languajecu));
			
			// text
			SetScreenAtribute(0, gray);
			SetScreenAtribute(1, black);
			printcu(TranslateWorck(&SSUNEDSECTION1TEXT_TRANSL, languajecu));

			// section "How To Use"
			SetScreenAtribute(0, brblue);
			SetScreenAtribute(1, black);
			printcu(TranslateWorck(&SSUNEDSECTION2_TRANSL, languajecu));

			// text
			SetScreenAtribute(0, gray);
			SetScreenAtribute(1, black);
			printcu(TranslateWorck(&SSUNEDSECTION2TEXT_TRANSL, languajecu));
		}

		// draw the screen
		SetScreenAtribute(0, gray);
		SetScreenAtribute(1, black);
		DrawScreen();

		StrCpy(text, save_text);
		UINTN Buttom = MaxRows - 1;

		// wait for key
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
			text[Index++] = L'\n';
		}
		// F3 = CONTROL
		else if (Key.ScanCode == SCAN_F3) {
			// wait for acces to the chars
			globalsystemtable->BootServices->WaitForEvent
			(
				1,
				&globalsystemtable->ConIn->WaitForKey,
				&Event
			);
			globalsystemtable->ConIn->ReadKeyStroke
			(
				globalsystemtable->ConIn,
				&Key
			);

			if (Key.UnicodeChar == L'c' || Key.UnicodeChar == L'C') {

				// coppy line

				// the copy char
				UINTN ch;

				// set to the last letter
				ch = Index;

				if (
					buffercpy_bord > 44
					)
				{
					ShowPanic(SE_FUE_A_LA_BORDA);
				}

				for (size_t i = 0; i < 44; i++)
				{
					cpy[i] = L'\0';
				}

				// set the end of the copied string
				cpy[0] = L'\0';

				// coppy the last line
				while (text[ch] != L'\n')
				{
					// recorre the string
					cpy = cpy - 1;

					// coppy the letter
					cpy[0] = text[ch];

					buffercpy_bord++;

					// before character
					ch--;
				}
			}
			else if (Key.UnicodeChar == L'v' || Key.UnicodeChar == L'V') {
				// the character
				UINTN ch;

				// set to the first char
				ch = 0;

				// paste
				while (cpy[ch])
				{
					// copy the characters
					text[Index++] = cpy[ch];
					// next character
					ch++;
				}
			}
		}
		else if (Key.UnicodeChar == CHAR_BACKSPACE) {
			if (Index > 0) {
				text[--Index] = L'\0';
			}
		}
		else if (Key.ScanCode == SCAN_F1) {
			SurvivalGuideAlt = SurvivalGuideAlt == 0 ? 1 : 0;
		}
		else if (Key.ScanCode == SCAN_F2) {
			text[Index] = L'\0';
			StrCpy(&EditorProcess, text);

			return text;
		}
		else if (Key.ScanCode == SCAN_UP) {
			EditorScroll--;
		}
		else if (Key.ScanCode == SCAN_DOWN) {
			EditorScroll++;
			if (EditorScroll < 0) {
				EditorScroll = 0;
			}
		}
		else if (Key.ScanCode == SCAN_ESC) {
			break;
		}
		else if (Key.ScanCode == SCAN_F5) {
			if (IsBinaryApp(text) == 1) {
				BinaryEx(text,false);
				SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
			}
			else {
				StrCpy(save_text, text);
				UINTN line_count = 0;
				ClearScreen();
				SetScreenAtribute(0, gray);
				SetScreenAtribute(1, black);
				CHAR16** lines = SplitLines(text, &line_count);
				SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
				SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
				ExecuteScript(save_text, ImageHandle, SystemTable);
				SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				StrCpy(text, save_text);
			}
		}
		else {
			if (Key.UnicodeChar != 0 || Key.ScanCode == SCAN_F12) {
				if (Key.ScanCode == SCAN_F12) {
					CHAR16 OtherCharacter = ACCES_TO_OTHER_CHARACTERS();
					if (OtherCharacter != NULL) {
						text[Index++] = OtherCharacter;
					}
				}
				else {
					text[Index++] = Key.UnicodeChar;
				}
				EditorScroll = 0;
			}
		}

		text[Index] = L'\0';
		WHILESYSTEMRUNNING();
	}

	*EditorProcess = NULL;

	bool save_confirm = DRAW_DIALOG_MSG_CONFIRM(ImageHandle,SystemTable, TranslateWorck(&EXIT_EDITOR_TEXT, languajecu), Sceme->buttonscolor, Sceme->buttonstext)

		if (save_confirm) {
			ChangeToTextMode();

			SetScreenAtribute(0, white);
			printc(L"\nFileName:\n");
			SetScreenAtribute(0, gray);
			CHAR16* filepathr = ReadLineSeriusWorck();

			BOOLEAN action = 1;
			while (action) {
				action = 0;
				if (
					StrnCmp(filepathr, L"cd ", 3) == 0
					) {
					action = 1;
				}
				if (
					StrnCmp(filepathr, L"mkdir ", 6) == 0
					) {
					action = 1;
				}
				if (
					StrnCmp(filepathr, L"rm ", 3) == 0
					) {
					action = 1;
				}
				if (
					StrCmp(filepathr, L"ls") == 0
					) {
					action = 1;
				}

				if (action) {
					filepathr = ReadLineSeriusWorck();
				}
			}

			CHAR16 filepath[20];
			for (
				size_t i = 0;
				i < (
					StrLen(filepathr) > 18 ? 
					18 : StrLen(filepathr)
					);
				i++
				)
			{
				filepath[i] = filepathr[i];
			}

			if (
				StrLen(filepathr) > 18
				)
			{
				filepath[19] = 0;
			}

			INT16 file_id = os_get_file_id(filepath, CurrentDir);

			if (
				file_id == -1
				)
			{
				os_create_file(filepath, CurrentDir, 0,L"dtr");
				file_id = os_get_file_id(filepath, CurrentDir);
			}

			os_write_file(file_id, text);

			printc(L"saving changes...\n");
			updatefilesystem();
		}

	CHAR16* texttosave = AllocatePool(1024 * sizeof(CHAR16));

	// Copiar elementos de text a texttosave
	for (size_t i = 0; i < 1024; i++) {
		texttosave[i] = text[i];
	}
	return texttosave;
}

/*
ExecuteCommand

Summary:
	Process the command of a S++ instruction for be executed

Command Reference:
	(see the command and read the comment that have)
*/
INT8
ExecuteCommand
(
	string* buffer,
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable,
	string mode
)
{
	EFI_INPUT_KEY Key;
	UINTN Event;

	InitializeLib(ImageHandle, SystemTable);
	/*
	echo (text)
	*/
	if (
		StrnCmp(buffer, L"echo ", 5)
		== 0
		) {
		CHAR16* msgtoecho = buffer + 5;
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
		SetScreenAtribute(0, consoleoutpudcurrentcolor);
		SetScreenAtribute(1, consoleoutpudcurrentcolorbg);

		printc(L"\n");
		if (StrCmp(msgtoecho, L"S-SUN is a very good system , erick") == 0) {
			printc(L"AWWWW Sweety (not the ... i forget it) o^ - ^o\nbut dont replace windows by this ^-^\n... i try to say thanks for the sweet ^-^ but i try to say eh emm forget it");
		}
		else if (StrCmp(msgtoecho, L"S-SUN es un buen sistema , erick") == 0) {
			printc(L"AWWWW que dulce o^ - ^o\npero no remplazes windows por esta ... cosa ^-^\n");
		}
		else if (StrCmp(msgtoecho, L"la sopa...") == 0) {
			printc(L"hoy prepare sopa\nhe aqui\nla sopa, la sopa que sopapea a todas las sopas\nla sopa , la sopa suprema");
		}
		else if (StrCmp(msgtoecho, L"hello ^-^...") == 0) {
			printc(L"hello ^-^ \x2a5\x2a5");
		}
		else if (StrCmp(msgtoecho, L"hola ^-^...") == 0) {
			printc(L"holaaaa ^-^ \x2a5\x2a5");
		}
		else if (StrCmp(msgtoecho, L"babosos in my chanel...") == 0) {
			printc(L"in my channel no babosos");
		}
		else if (StrCmp(msgtoecho, L"tienen el control completo? , erick") == 0) {
			printc(L"no , no tenemos el control completo de la computadora\nya que depende de los bootservices\npor que sino S-SUN se nos muere");
		}
		else
		{
			printc(SPP_SYNTAX(msgtoecho));
		}
	}
	if (
		StrnCmp(
			buffer,
			L"techo "
			, 6
		) == 0
		)
	{
		printcu(L"\n");
		printcu(buffer + 5);
	}
	else if (
		StrCmp(buffer, L"hello view")
		== 0
		)
	{
		editor(ImageHandle, SystemTable, hello_bin);
	}
	/*
	help (theme)
	*/
	else if (
		StrnCmp(buffer, L"help ", 5)
		== 0
		) {
		CHAR16* themetohelp = buffer + 5;
		SetScreenAtribute(0, brcyan);

		printc(L"\n");
		if (StrCmp(themetohelp, L"syntax") == 0) {
			printc(L"SPP_SYNTAX:\n\nSyntax use :\n\tMEMS Syntax :\n\t\t% (MEM) : get a MEM_FILE_STRING or a MEM_FILE_INT\n\t\tX % (MEM_INT)get a MEM_FILE_INT in hex withut the \"0x\"(can use the two caps in the \"X\")\n\n		def%(MEM): verific if a MEM file exist (can be \"def\" or \"DEF\")\n\tSTRICT MEM syntax :\n\t\t S % (MEM_STRING)get a MEM_STRING(can use the two caps in the \"S\")\n\t\t D % (MEM_INT)get a MEM_INT(can use the two caps in the \"D\")\n\n	COMPARATORS:\n\tcmp%(syntax1) = (syntax2) : verific if syntax(can be \"cmp\" or \"CMP\")\n\topr&(syntax1)(op +/-)(syntax2): make it a add or sub (can be \"opr\" or \"OPR\")\n\n\tS - FRAMEWORCK :\n\t\t &confirm(Message) : shows a YESS or NO window popup\n\n\tELSE :\n\t\treturns the text");
		}
	}
	else if (
		StrnCmp(buffer, L"writel ", 7)
		== 0
		) {
		CHAR16* msgtoecho = buffer + 7;
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
		SetScreenAtribute(0, consoleoutpudcurrentcolor);
		SetScreenAtribute(1, consoleoutpudcurrentcolorbg);

		printc(SPP_SYNTAX(msgtoecho));
	}
	else if (
		StrCmp(buffer, L"Reset /all")
		== 0
		) {
		InitializeFileSystem();

		LibDeleteVariable(L"Lang", &SmallVariables);
		LibDeleteVariable(L"Theme", &SmallVariables);
		LibDeleteVariable(L"ThemeT", &SmallVariables);

		LibDeleteVariable(L"S-SUN_State", &VariablesGuid);

		updatefilesystem();
	}
	/*
	textmode and grmode
	*/
	else if (
		StrCmp(buffer, L"textmode")
		== 0
		) {
		ChangeToTextMode();
	}
	/*
	beep (number)
	*/
	else if (
		StrnCmp(buffer, L"beep ", 5)
		==
		0
		)
	{
		CHAR16* command = buffer + 5;
		CHAR16* equals_pos = StrStr(command, L",");
		if (equals_pos != NULL) {
			*equals_pos = L'\0';
			CHAR16* MemName = command;
			CHAR16* NewValue = equals_pos + 1;

			EFI_STATUS Status = Edit_MEM_FILE_STRING(MemName, NewValue);
			PlaySound(Atoi(MemName), Atoi(NewValue));
		}
	}
	else if (
		StrCmp(buffer, L"grmode")
		== 0
		) {
		ChangeToGrapichalMode();
	}
	/*
	int (interruptionID)(letter of the indicator)
	*/
	else if (
		StrnCmp(buffer, L"int ", 4)
		== 0
		)
	{
		CHAR16* interruption = buffer + 4;
		UINTN interruptionSize = StrLen(interruption);

		if (interruption[interruptionSize] = L'f') {
			INTN interruptionConverted = Atoi(interruption);

			FrameworckInterruptions(interruptionConverted);
			return 0;
		}
		else if (interruption[interruptionSize] = L'n') {

		}
	}
	else if (
		StrnCmp(buffer, L"ip ", 3) == 0
		)
	{
		//
		// variables
		// 

		EFI_MAC_ADDRESS* IpTrMgr;
		EFI_MAC_ADDRESS* IpReMgr;

		//
		// dec args
		//

		CHAR16* Args = buffer + 3;

		BOOLEAN GetArgument = 0;

		// ** transmit and recive args
		BOOLEAN TransmitArgument = 0;
		BOOLEAN ReciveArgument = 0;

		//
		// parse args
		//

		INT16 ChA = 0;

		while (
			Args[ChA]
			)
		{
			//
			// get arg
			//
			if (
				Args[ChA] == L'-'
				)
			{
				CHAR16 Arg[10] = L"\0\0\0\0\0\0\0\0\0\0";

				ChA++;

				INT8 ParserArg = 0;

				//
				// get arg chars
				//
				while (
					Args[ChA] != L' ' && Args[ChA]
					)
				{
					Arg[ParserArg] = Args[ChA];
					ParserArg++;
					ChA++;
				}
				Arg[ParserArg] = 0;

				//
				// parse arg
				//

				// -get
				if (
					StrCmp(
						Arg,
						L"get"
					) == 0
					)
					GetArgument = 1;

				// -tr
				if (
					Arg[0] == L't' &&
					Arg[1] == L'r' &&
					Arg[2] == L'='

					) {
					IpTrMgr = StringToIp(Arg + 3);

					TransmitArgument = 1;
				}

				// -re
				if (
					StrnCmp(
						Arg,
						L"re=",
						3
					) == 0
					) {
					IpReMgr = StringToIp(Arg + 3);

					ReciveArgument = 1;
				}
			}

			ChA++;
		}

		//
		// execute args
		//
		if (
			GetArgument
			)
		{
			printc(L"\n");

			//
			// set variables
			//

			CHAR16 IpParse[10];

			//
			// primary part
			//

			SPrint(IpParse, sizeof(IpParse), L"%d", MyIp->Addr[0]);

			SetScreenAtribute(0, brblue);
			printc(IpParse);

			SetScreenAtribute(0, gray);
			printc(L".");

			//
			// secondary part
			//

			SPrint(IpParse, sizeof(IpParse), L"%d", MyIp->Addr[1]);

			SetScreenAtribute(0, bryellow);
			printc(IpParse);

			SetScreenAtribute(0, gray);
			printc(L".");

			//
			// 3rd part
			//

			SPrint(IpParse, sizeof(IpParse), L"%d", MyIp->Addr[2]);

			SetScreenAtribute(0, brgreen);
			printc(IpParse);

			SetScreenAtribute(0, gray);
			printc(L".");

			//
			// secondary part
			//

			SPrint(IpParse, sizeof(IpParse), L"%d", MyIp->Addr[3]);

			SetScreenAtribute(0, brcyan);
			printc(IpParse);
		}
		if (
			TransmitArgument
			)
		{
			printc(L"\n");

			SetScreenAtribute(0, brblue);
			printc(L"Write the command to send to the ip:");
			SetScreenAtribute(0, brgreen);

			EFI_SIMPLE_NETWORK_TRANSMIT Trans;

			CHAR16 CommandT[20];

			CHAR16* ee = ReadLineSeriusWorck();

			for (size_t i = 0; i < 20; i++)
			{
				CommandT[i] = ee[i];
			}

			Net->Transmit(
				Net,
				0,
				sizeof(CommandT),
				CommandT,
				MyIp,
				IpTrMgr,
				0
			);

		}
		if (
			ReciveArgument
			)
		{
			CHAR16 CommandT[20];

			Net->Receive(
				Net,
				0,
				sizeof(CommandT),
				CommandT,
				IpReMgr,
				MyIp,
				0
			);

			ExecuteCommand(CommandT, ImageHandle, SystemTable, mode);
		}
	}
	else if (
		StrnCmp(buffer, L"buzz ", 5) == 0
		)
	{
		//
		// params recorre
		//

		ch16* ps = buffer + 5;

		//
		// parse param
		//

		if (
			StrnCmp(ps, L"frq ", 4) == 0
			)
		{
			//
			// setup
			//

			u16 frequency = 0;
			ch16* value = ps + 4;

			//
			// parsing
			//

			frequency = Atoi(value);

			//
			// send
			//

			SetFrequency(frequency);
		}
		else if (
			StrnCmp(ps, L"mod ", 4) == 0
			)
		{
			//
			// setup
			//

			u8 mode = 0;
			ch16* value = ps + 4;

			//
			// parsing
			//

			mode = Atoi(value);

			//
			// send
			//

			SetChannelMode(mode);
		}
		else if (
			StrCmp(ps, L"on") == 0
			)
		{
			EnableSpeaker();
		}
		else if (
			StrCmp(ps, L"test") == 0
			)
		{
			EnableSpeaker();
			gBS->Stall(70);
			DisableSpeaker();
		}
		else if (
			StrCmp(ps, L"off") == 0
			)
		{
			DisableSpeaker();
		}
		else {
			printc(L"\n");
			printc(L"buzz usage\n");
			printc(L"\nprovides a function to send signals to the buzzer\n\n\n");
			printc(L"ONLY ONE PARAM AT TIME\n\nfrq frequency\nmod mode\noff\non\ntest\n");
		}
	}
	else if (
		StrCmp(buffer, L"wait") 
		== 0
		)
	{
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 500000); \
	}
	/*
	throw (error)
	*/
	else if (
		StrnCmp(buffer, L"throw ", 6) 
		== 0
		) {
		CHAR16* reason = buffer + 6;
		THROW_ERROR(SPP_SYNTAX(reason));
	}
	/*
	edit (file ID)
	*/
	else if (
		StrnCmp(buffer, L"edit ", 5) 
		== 0
		) {
		CHAR16* file = buffer + 5;


		editor(ImageHandle, SystemTable, os_open_file(os_get_file_id(file,CurrentDir)));
	}
	else if (
		StrCmp(buffer, L"LSMEM") 
		== 0
		) {
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK);
		SetScreenAtribute(0, brblue);
		printc(L"\nList of MEM files:\n\n");

		SetScreenAtribute(0, brgreen);

		printc(L"MemFilesString ");

		SetScreenAtribute(0, gray);
		printc(L"([\n");

		for (UINTN i = 0; i < MemFileCountString; i++) {
			if (StrCmp(MemFilesString[i]->NAME, L"[deleted content]")) {
				SetScreenAtribute(0, brblue);
				printc(L"   * ");
				SetScreenAtribute(0, cyan);
				printc(MemFilesString[i]->NAME);
				SetScreenAtribute(0, white);
				printc(L" = ");
				SetScreenAtribute(0, brgreen);
				printc(MemFilesString[i]->VALUE);
				printc(L"\n");
			}
		}
		SetScreenAtribute(0, gray);
		printc(L"])\n\n");

		SetScreenAtribute(0, brgreen);

		printc(L"MemFilesInt ");

		SetScreenAtribute(0, gray);
		printc(L"([\n");
		for (UINTN i = 0; i < MemFileCountInt; i++) {
			if (StrCmp(MemFilesInt[i]->NAME, L"[deleted content]")) {
				SetScreenAtribute(0, brblue);
				printc(L"   * ");
				SetScreenAtribute(0, cyan);
				printc(MemFilesInt[i]->NAME);
				SetScreenAtribute(0, white);
				printc(L" = ");
				string value[100];

				SPrint(value, sizeof(value), L"%d ", MemFilesInt[i]->VALUE);
				SetScreenAtribute(0, brgreen);
				printc(value);
				printc(L"\n");
			}
		}
		SetScreenAtribute(0, gray);
		printc(L"])\n\n");
	}
	/*
	FlushMEM (MEM_NAME)
	*/
	else if (
		StrnCmp(buffer, L"FlushMEM ", 9)
		== 0
		) {
		CHAR16* command = buffer + 9;
		if (Flush_MEM_FILE_STRING(command) == true) return 0;
		if (Flush_MEM_FILE_INT(command) == true) return 0;

	}
	/*
	drw (action/color)
	*/
	else if (
		StrnCmp(buffer, L"drw ", 4) 
		== 0
		) {
		CHAR16* command = buffer + 4;

		if (StrCmp(command, L"down") == 0) {
			DRW_DOWN;
		}
		if (StrCmp(command, L"update") == 0) {
			DrawScreen();
		}
		else
		{
			if (StrCmp(command, L"black") == 0) {
				DRW_COL = black;
			}
			else if (StrCmp(command, L"gray") == 0) {
				DRW_COL = gray;
			}
			else if (StrCmp(command, L"lightgray") == 0) {
				DRW_COL = lightgray;
			}
			else if (StrCmp(command, L"white") == 0) {
				DRW_COL = white;
			}
			else if (StrCmp(command, L"red") == 0) {
				DRW_COL = red;
			}
			else if (StrCmp(command, L"brred") == 0) {
				DRW_COL = brblue;
			}
			else if (StrCmp(command, L"orange") == 0) {
				DRW_COL = orange;
			}
			else if (StrCmp(command, L"brorange") == 0) {
				DRW_COL = brorange;
			}
			else if (StrCmp(command, L"yellow") == 0) {
				DRW_COL = yellow;
			}
			else if (StrCmp(command, L"bryellow") == 0) {
				DRW_COL = bryellow;
			}
			else if (StrCmp(command, L"green") == 0) {
				DRW_COL = green;
			}
			else if (StrCmp(command, L"brgreen") == 0) {
				DRW_COL = brgreen;
			}
			else if (StrCmp(command, L"blue") == 0) {
				DRW_COL = blue;
			}
			else if (StrCmp(command, L"brblue") == 0) {
				DRW_COL = brblue;
			}
			else
			{
				DRW_COL = white;
			}
		}
	}
	/*
	color (color name)
	*/
	else if (
		StrnCmp(buffer, L"color ", 6) 
		== 0
		) {
		CHAR16* command = buffer + 6;
		if (StrCmp(command, L"black") == 0) {
			consoleoutpudcurrentcolor = black;
		}
		else if (StrCmp(command, L"white") == 0) {
			consoleoutpudcurrentcolor = white;
		}
		else if (StrCmp(command, L"gray") == 0) {
			consoleoutpudcurrentcolor = gray;
		}
		else if (StrCmp(command, L"red") == 0) {
			consoleoutpudcurrentcolor = red;
		}
		else if (StrCmp(command, L"darkred") == 0) {
			consoleoutpudcurrentcolor = darkred;
		}
		else if (StrCmp(command, L"brred") == 0) {
			consoleoutpudcurrentcolor = brred;
		}
		else if (StrCmp(command, L"orange") == 0) {
			consoleoutpudcurrentcolor = orange;
		}
		else if (StrCmp(command, L"darkorange") == 0) {
			consoleoutpudcurrentcolor = darkorange;
		}
		else if (StrCmp(command, L"brorange") == 0) {
			consoleoutpudcurrentcolor = brorange;
		}
		else if (StrCmp(command, L"yellow") == 0) {
			consoleoutpudcurrentcolor = yellow;
		}
		else if (StrCmp(command, L"darkyellow") == 0) {
			consoleoutpudcurrentcolor = darkyellow;
		}
		else if (StrCmp(command, L"bryellow") == 0) {
			consoleoutpudcurrentcolor = bryellow;
		}
		else if (StrCmp(command, L"green") == 0) {
			consoleoutpudcurrentcolor = green;
		}
		else if (StrCmp(command, L"darkgreen") == 0) {
			consoleoutpudcurrentcolor = darkgreen;
		}
		else if (StrCmp(command, L"brgreen") == 0) {
			consoleoutpudcurrentcolor = brgreen;
		}
		else if (StrCmp(command, L"blue") == 0) {
			consoleoutpudcurrentcolor = blue;
		}
		else if (StrCmp(command, L"darkblue") == 0) {
			consoleoutpudcurrentcolor = darkblue;
		}
		else if (StrCmp(command, L"brblue") == 0) {
			consoleoutpudcurrentcolor = brblue;
		}
		else if (StrCmp(command, L"cyan") == 0) {
			consoleoutpudcurrentcolor = cyan;
		}
		else if (StrCmp(command, L"darkcyan") == 0) {
			consoleoutpudcurrentcolor = darkcyan;
		}
		else if (StrCmp(command, L"brcyan") == 0) {
			consoleoutpudcurrentcolor = brcyan;
		}
	}
	/*
	bgcol (color name)
	*/
	else if (
		StrnCmp(buffer, L"bgcol ", 6)
		== 0
		) {
		CHAR16* command = buffer + 6;
		if (StrCmp(command, L"black") == 0) {
			consoleoutpudcurrentcolorbg = black;
		}
		else if (StrCmp(command, L"white") == 0) {
			consoleoutpudcurrentcolorbg = white;
		}
		else if (StrCmp(command, L"gray") == 0) {
			consoleoutpudcurrentcolorbg = gray;
		}
		else if (StrCmp(command, L"red") == 0) {
			consoleoutpudcurrentcolorbg = red;
		}
		else if (StrCmp(command, L"darkred") == 0) {
			consoleoutpudcurrentcolorbg = darkred;
		}
		else if (StrCmp(command, L"brred") == 0) {
			consoleoutpudcurrentcolorbg = brred;
		}
		else if (StrCmp(command, L"orange") == 0) {
			consoleoutpudcurrentcolorbg = orange;
		}
		else if (StrCmp(command, L"darkorange") == 0) {
			consoleoutpudcurrentcolorbg = darkorange;
		}
		else if (StrCmp(command, L"brorange") == 0) {
			consoleoutpudcurrentcolorbg = brorange;
		}
		else if (StrCmp(command, L"yellow") == 0) {
			consoleoutpudcurrentcolorbg = yellow;
		}
		else if (StrCmp(command, L"darkyellow") == 0) {
			consoleoutpudcurrentcolorbg = darkyellow;
		}
		else if (StrCmp(command, L"bryellow") == 0) {
			consoleoutpudcurrentcolorbg = bryellow;
		}
		else if (StrCmp(command, L"green") == 0) {
			consoleoutpudcurrentcolorbg = green;
		}
		else if (StrCmp(command, L"darkgreen") == 0) {
			consoleoutpudcurrentcolorbg = darkgreen;
		}
		else if (StrCmp(command, L"brgreen") == 0) {
			consoleoutpudcurrentcolorbg = brgreen;
		}
		else if (StrCmp(command, L"blue") == 0) {
			consoleoutpudcurrentcolorbg = blue;
		}
		else if (StrCmp(command, L"darkblue") == 0) {
			consoleoutpudcurrentcolorbg = darkblue;
		}
		else if (StrCmp(command, L"brblue") == 0) {
			consoleoutpudcurrentcolorbg = brblue;
		}
		else if (StrCmp(command, L"cyan") == 0) {
			consoleoutpudcurrentcolorbg = cyan;
		}
		else if (StrCmp(command, L"darkcyan") == 0) {
			consoleoutpudcurrentcolorbg = darkcyan;
		}
		else if (StrCmp(command, L"brcyan") == 0) {
			consoleoutpudcurrentcolorbg = brcyan;
		}
		}
	/*
	mv drw (x/y/sx/sy) (number/action)
	*/
	else if (
		StrnCmp(buffer, L"mv drw x ", 9) 
		== 0
		) {
		CHAR16* command = buffer + 9;

		if (StrCmp(command, L"r") == 0) {
			DRW_X = 1;
		}
		else if (StrCmp(command, L"scr") == 0) {
			DRW_X = gop->Mode->Info->HorizontalResolution;
		}
		else
		{
			if (command[0] == L'-') {
				string* e = command + 1;

				DRW_X -= Atoi(e);
			}
			else
			{
				DRW_X += Atoi(command);
			}
		}
	}
	else if (
		StrnCmp(buffer, L"mv drw y ", 9) 
		== 0
		) {
		CHAR16* command = SPP_SYNTAX(buffer + 9);

		if (StrCmp(command, L"r") == 0) {
			DRW_Y = 0;
		}
		else if (StrCmp(command, L"scr") == 0) {
			DRW_Y = gop->Mode->Info->VerticalResolution;
		}
		else
		{
			if (command[0] == L'-') {
				string* e = command + 1;

				DRW_Y -= Atoi(e);
			}
			else
			{
				DRW_Y += Atoi(command);
			}
		}
	}
	else if (
		StrnCmp(buffer, L"mv drw sx ", 10) 
		== 0
		) {
		CHAR16* command = buffer + 10;

		if (StrCmp(command, L"r") == 0) {
			DRW_SX = 1;
		}
		else if (StrCmp(command, L"scr") == 0) {
			DRW_SX = gop->Mode->Info->HorizontalResolution;
		}
		else
		{
			if (command[0] == L'-') {
				string* e = command + 1;

				DRW_SX -= Atoi(e);
			}
			else
			{
				DRW_SX += Atoi(command);
			}
		}
	}
	else if (
		StrnCmp(buffer, L"mv drw sy ", 10) 
		== 0
		) {
			CHAR16* command = buffer + 10;

			if (StrCmp(command, L"r") == 0) {
				DRW_SY = 1;
			}
			else if (StrCmp(command, L"scr") == 0) {
				DRW_SY = gop->Mode->Info->VerticalResolution;
			}
			else
			{
				if (command[0] == L'-') {
					string* e = command + 1;
					DRW_SY -= Atoi(e);
				}
				else
				{
					DRW_SY += Atoi(command);
				}
			}
	}
	if (
		StrCmp(buffer, L"g-frv")
		==
		0
		)
	{
		PIXELCOL syscolor2 = Sceme->buttonscolor;
		PIXELCOL sysbg = Sceme->backgroundcolor;
		PIXELCOL sysbtext = Sceme->buttonstext;

		DRAW_DIALOG_MSG(ImageHandle, SystemTable, OemVendor(), syscolor2, sysbtext);
	}
	/*
	EditMem (MEM_NAME)=(MEM_VALUE)
	*/
	else if (
		StrnCmp(buffer, L"EditMem ", 8)
		== 0
		) {
		CHAR16* command = buffer + 8;
		CHAR16* equals_pos = StrStr(command, L"=");
		if (equals_pos != NULL) {
			*equals_pos = L'\0';
			CHAR16* MemName = command;
			CHAR16* NewValue = equals_pos + 1;

			EFI_STATUS Status = Edit_MEM_FILE_STRING(MemName, NewValue);
			if (Status == EFI_NOT_FOUND) {
				Status = Edit_MEM_FILE_INT(MemName, Atoi(SPP_SYNTAX(NewValue))); // Convertir a int si no se encuentra como string
			}
			if (Status == EFI_NOT_FOUND) {
				MEM_FILE_STRING* NEW_MEM_FILE = Create_MEM_FILE_STRING(MemName, SPP_SYNTAX(NewValue));
			}
		}
	}
	else if (
		StrnCmp(buffer, L"if ", 3) 
		== 0
		) {
		CHAR16* command = buffer + 3;
		CHAR16* div_pos = StrStr(command, L":");
		if (div_pos != NULL) {
			*div_pos = L'\0';
			CHAR16* Syntax = command;
			CHAR16* CommandToExecute = div_pos + 1;

			if (StrCmp(SPP_SYNTAX(Syntax), L"1") == 0) {
				return ExecuteCommand(CommandToExecute, ImageHandle, SystemTable, mode);
			}
		}
		}

	/*
	ReadLine (MEM_NAME)
	*/
	else if (
		StrnCmp(buffer, L"ReadLine ", 9) 
		== 0
		) {
		CHAR16* command = buffer + 9;
		if (command != NULL) {
			CHAR16* MemName = command;

			CHAR16* readedLine = ReadLineSeriusWorck();
			SetScreenAtribute(0, consoleoutpudcurrentcolor);
			SetScreenAtribute(1, consoleoutpudcurrentcolorbg);

			EFI_STATUS Status = Edit_MEM_FILE_STRING(MemName, readedLine);
			if (Status == EFI_NOT_FOUND) {
				MEM_FILE_STRING* NEW_MEM_FILE = Create_MEM_FILE_STRING(MemName, readedLine);
			}
		}
	}
	/*
	Shark P Mem_Name &&\t(Mem_Value)
	*/
	else if (
		StrnCmp(buffer, L"Shark P ", 8) 
		== 0
		) {
		CHAR16* command = buffer + 8;
		CHAR16* equals_pos = StrStr(command, L"\t");
		if (equals_pos != NULL) {
			*equals_pos = L'\0';
			CHAR16* MemName = command;
			CHAR16* NewValue = equals_pos + 1;

			CHAR16 Status = Read_MEM_FILE_STRING(MemName, NewValue);
			if (Status == NULL) {
				MEM_FILE_STRING* NEW_MEM_FILE = Create_MEM_FILE_STRING(MemName, NewValue);
			}
			else
			{
				SetScreenAtribute(0, consoleoutpudcurrentcolor);

				printc(L"\n");
				printc(L"the buffer is so mad for the system -_O");
			}
		}
	}
	/*
	Shark I Mem_Name &&\t(Mem_Value)
	*/
	else if (
		StrnCmp(buffer, L"Shark I ", 8) 
		== 0
		) {
		CHAR16* command = buffer + 8;
		CHAR16* equals_pos = StrStr(command, L"\t");
		if (equals_pos != NULL) {
			*equals_pos = L'\0';
			CHAR16* MemName = command;
			CHAR16* NewValue = equals_pos + 1;

			UINTN Status = Read_MEM_FILE_INT(MemName, NewValue);
			if (Status == -1) {
				MEM_FILE_INT* NEW_MEM_FILE = Create_MEM_FILE_INT(MemName, NewValue);
			}
			else
			{
				SetScreenAtribute(0, consoleoutpudcurrentcolor);

				printc(L"\n");
				printc(L"the buffer is so mad for the system -_O");
			}
		}
		}
	else if (
		StrCmp(buffer, L"catch", 6) 
		== 0
		) {
		_KERNEL_CATCH_THROW_ = 1;
	}
	else if (
		StrCmp(buffer, L"end catch", 6) 
		== 0
		) {
		_KERNEL_CATCH_THROW_ = 0;
	}
	else if (
		StrnCmp(buffer, L"mkdir ", 6) 
		== 0
		) {
		CHAR16* DirName = SPP_SYNTAX(buffer + 6);
		os_create_file(DirName, CurrentDir, 1, L"fld");
	}
	else if (
		StrnCmp(buffer, L"rm ", 3)
		== 0
		) {
			CHAR16* DirName = SPP_SYNTAX(buffer + 3);
			os_del_file(Atoi(DirName));
			}
	/*
	gotox (pos)
	gotoy (pos)
	*/
	else if (
		StrnCmp(buffer, L"gotox ", 6) 
		== 0
		) {
		CHAR16* NUMBER = SPP_SYNTAX(buffer + 6);
		gotoxy(Atoi(NUMBER), cursory);
	}
	else if (
		StrnCmp(buffer, L"gotoy ", 6) 
		== 0
		) {
		CHAR16* NUMBER = SPP_SYNTAX(buffer + 6);
		gotoxy(cursorx, Atoi(NUMBER));
		}
	/*
	MsgBox (text)
	*/
	else if (
		StrnCmp(buffer, L"MsgBox ", 7) 
		== 0
		) {
		CHAR16* Text = SPP_SYNTAX(buffer + 7);
		CHAR16* FullMessage[100];
		SPrint(FullMessage, sizeof(FullMessage), L"%s", Text);
		PIXELCOL syscolor2 = Sceme->buttonscolor;
		PIXELCOL sysbg = Sceme->backgroundcolor;
		PIXELCOL sysbtext = Sceme->buttonstext;

		DRAW_DIALOG_MSG(ImageHandle, SystemTable, FullMessage, syscolor2, sysbtext);
	}
	else if (
		StrCmp(buffer, L"about")
		== 0
		)
		{
			printc(L"\n");
			printc(L"\n");

			INTN lrcy = cursory;

			DrawLogo(cursorx, cursory,3);

			gotoxy(11 + 3 + 3 + 3 + 4, lrcy + 1);
			SetScreenAtribute(0, gray);
			printc(L" S-SUN\x2763 by ErickCraftStudiosx\x2763 2025 (c) with CC-BY SA");
			printc(L"\n\n\n");

		}
	else if (
		StrCmp(buffer, L"SyMgr") 
		== 0
		) {
			SystemManager(ImageHandle, SystemTable);
	}
	else if (
		StrnCmp(buffer, L"cd ", 3) 
		== 0
		) {
			CHAR16* folder = buffer + 3;

			if (StrCmp(folder,L"..") == 0) {
				if (CurrentDir != -1) {
					CurrentDir = CurrentFS->Files[CurrentDir].Parent;
				}
			}
			else {
				CurrentDir = os_get_file_id(folder,CurrentDir);
			}
	}	
	else if (
		StrnCmp(buffer, L"./",2) == 0
		)
		{
			CHAR16* file = buffer + 2;

			BinaryEx(os_open_file(os_get_file_id(file, CurrentDir)),0);
		}
	else if (
		StrCmp(buffer, L"ls") 
		== 0
		) {
		printc(L"\n");
		gotoxy(0, cursory);
		SetScreenAtribute(0, gray);
		printcu(L"Name");
		gotoxy(MAX_NAME_CHARACTERS, cursory);
		printcu(L"ext");
		gotoxy(MAX_NAME_CHARACTERS + 4, cursory);
		printcu(L"id");
		printcu(L"\n");
		printcu(L"\n");

		for (size_t i = 0; i < MAX_FILES - 1; i++)
		{
			if (
				CurrentFS->Files[i].IsCreated
				) {
				if (
					CurrentFS->Files[i].Parent == CurrentDir
					)
				{
					if (
						CurrentFS->Files[i].Extension[0] == L'f' &&
						CurrentFS->Files[i].Extension[1] == L'l' &&
						CurrentFS->Files[i].Extension[2] == L'd' &&
						CurrentFS->Files[i].Extension[3] == 0
						)
					{
						SetScreenAtribute(0, brblue);
					}
					else {
						if (
							CurrentFS->Files[i].Content[0] == L'E' &&
							CurrentFS->Files[i].Content[1] == L'A' &&
							CurrentFS->Files[i].Content[2] == 3
							)
						{
							SetScreenAtribute(0, brgreen);
						}
						else if (
							CurrentFS->Files[i].Content[0] == L'M' &&
							CurrentFS->Files[i].Content[1] == L'P' &&
							CurrentFS->Files[i].Content[2] == 3
							)
						{ 
							SetScreenAtribute(0, cyan);
						}
						else {
							SetScreenAtribute(0, gray);
						}
					}

					for (size_t J = 0; J < MAX_NAME_CHARACTERS - 1; J++)
					{
						if (CurrentFS->Files[i].Name[J] > L'!' - 1 && CurrentFS->Files[i].Name[J] < L'z' + 1) {
							CHAR16 e[2];
							e[0] = CurrentFS->Files[i].Name[J];
							e[1] = 0;
							printcu(e);
						}
						if (
							CurrentFS->Files[i].Name[J] == 0
							)
						{
							break;
						}
					}

					SetScreenAtribute(0, brcyan);
					gotoxy(MAX_NAME_CHARACTERS, cursory);
					for (size_t J = 0; J < 4 - 1; J++)
					{
						if (CurrentFS->Files[i].Extension[J] > L'!' - 1 && CurrentFS->Files[i].Extension[J] < L'z' + 1) {
							CHAR16 e[2];
							e[0] = CurrentFS->Files[i].Extension[J];
							e[1] = 0;
							printcu(e);
						}
						if (
							CurrentFS->Files[i].Extension[J] == 0
							)
						{
							break;
						}
					}
					SetScreenAtribute(0, bryellow);
					gotoxy(MAX_NAME_CHARACTERS + 4, cursory);
					CHAR16 bf[20];
					ValueToString(bf, 0, i);
					printcu(bf);

					printcu(L"\n");
				}
			}
		}
	}
	else if (
		StrnCmp(buffer, L"brightness ", 11) == 0
		)
		{
			laptop_birghtness = Atoi(buffer + 11);
		}
	else if (
		StrCmp(buffer, L"cls") 
		== 0
		) {
		ClearScreen();
	}
	else if (
		StrCmp(buffer, L"exit") 
		== 0
		) {
		return 1;
	}
	else if (
		StrnCmp(buffer, L"#",1) 
		== 0
		) {
		}
	else {
		/*
		printc(L"\n");
		SetScreenAtribute(0, brred);
		printc(L"Invalid command");
		*/
	}
	return 0;
}

/*
ExecuteScript

Summary:
	executes a s++ script
*/
MINI_PROGRAM
ExecuteScript
(
	CONST CHAR16* bufferf,
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	//
	// variables
	//

	// the input variable
	EFI_INPUT_KEY Key;

	// the buffer
	CHAR16* buffer = StrDuplicate(bufferf);
	
	// the script lines count
	UINTN line_count = 0;

	//
	// script interpretatin
	//

	// divide the text in lines
	CHAR16** lines = SplitLines(buffer, &line_count);

	//
	// recover values
	//

	// the mode restorer
	UINTN smodebackup = SMODE;

	// the ret command line
	INTN returnsection = 0;

	//
	// screen recover
	//

	// save the bgcol no se por que
	PIXELCOL savedcolorbg = Conio->atributes->BG;

	// screen restore
	Pixels* savedbufsc = AllocatePool(sizeof(Pixels) * pixels);
	for (size_t i = 0; i < pixels; i++) {
		// save every pixel
		savedbufsc[i] = bufferscreen[i];
	}

	// screen pixels count restore
	UINTN savpixels = pixels;

	//
	// script ex
	//

	// the script execution
	for (UINTN i = 0; i < line_count; i++) {

		//
		// debugger things
		//

		// read the key to if you want to stop the script 
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		// if you press F11 stop the script
		if (Key.ScanCode == SCAN_F11) {

			bool exit_confirm = DRAW_DIALOG_MSG_CONFIRM(
				ImageHandle,
				SystemTable,
				TranslateWorck(&SCRIPT_EXIT_EDITOR_TEXT, languajecu),
				Sceme->buttonscolor,
				Sceme->buttonstext
			)
				// pedirle permiso al sistema es una opcion
			;

			// if you want to exits entonces sales del script
			if (
				exit_confirm
				) {
				break;
				// ok
			}
		}

		//
		// instruction parser
		//

		// if you call to jump a any section
		if (
			StrnCmp(lines[i], L"call ", 5) == 0
			) // if the command call pues llamar a la seccion 22 (como que 22)
			{
				CHAR16* section_to_search = lines[i] + 5; // the section for search
				INTN search_section = -1; // the section researcher

				// the loop of the no se
				for (search_section = 0; search_section < line_count; search_section++)
				{
					if (
						StrCmp(lines[search_section], section_to_search) == 0 &&
						StrCmp(lines[search_section - 1], L"section") == 0
						) // if coincide this and the section is finded
						{
						// pues ...
							returnsection = i + 1;
							break; // yoyle
					}
				}
				if (
					search_section != -1
					// if the section is finded
					) {
					i = search_section;
					// -> redirect to the section
				}
			}
		// if you want to back to the before that are you before
		else if (
				StrCmp(lines[i], L"ret") == 0
				// the command ret is for solved the no se
			)
			{	
				// return to the line
				i = returnsection - 1;
			}
		// if you want to restore the screen
		else if (
				StrCmp(lines[i], L"restore_screen") == 0
			)
		{
			// load the pixels count
			pixels = savpixels;

			// restore the pixels
			for (size_t i = 0; i < savpixels; i++) {
				bufferscreen[i] = savedbufsc[i];
			}

			// draw the bg
			DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, savedcolorbg);

			// draw the screen
			DrawScreen();
		}
		// execute the command
		else {
			// si...
			INT8 command = ExecuteCommand(lines[i], ImageHandle, SystemTable, L"Dev");
		}
	}

	//
	// free values
	//

	// restore the mode
	SMODE = smodebackup;
}

/*
Console

Summary:
	the console of S-SUN
*/
PROGRAM 
Console
(
	CHAR16 mode
) 
{
	//
	// setuping
	// 
	
	u16 count_spaces = 0;

	// the event
	UINTN Event;
	// status
	EFI_STATUS Status;
	// the imput key
	EFI_INPUT_KEY Key;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

	// the prompt and the current letter
	CHAR16 Buffer[512];
	UINTN Index = 0;

	// Obtener la interfaz de sistema de archivos
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, globalimagehandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&FileSystem);
	// Abrir el volumen del sistema de archivos

	// Continuar con el resto de la inicialización
	globalsystemtable->ConOut->ClearScreen(globalsystemtable->ConOut);
	uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);

	/*t
	 * In addition to the standard %-based flags, Print() supports the following:
	 *   %N
	 Set output attribute to normal
	 *   %H       Set output attribute to highlight
	 *   %E       Set output attribute to error
	 *   %r       Human readable version of a status code
	 */
	// the console instance
	MEM_FILE_INT* CONSOLEINSTANCE = Create_MEM_FILE_INT(L"CONSOLEINSTANCE", 1);
	
	// set the screen
	SetScreenAtribute(1, black);
	ClearScreen();
	gotoxy(0,0);

	//
	// draw the prompt
	//

	// action of the command
	CHAR16 handle_action_huh = ExecuteCommand(L"cls", globalimagehandle, globalsystemtable, mode);
	// the title
	SetScreenAtribute(0, gray);
	printc(L"\nS-SUN Console\n\n");

	SetScreenAtribute(0, gray);
	printcu(TranslateWorck(&GENERIC1_TEXT_TRANSL, languajecu));
	SetScreenAtribute(0, brblue);
	printcu(TranslateWorck(&CONSOLE_BUTTON_TRANSL,languajecu));
	printcu(L" ");
	SetScreenAtribute(0, gray);
	printcu(TranslateWorck(&CONSOLE_INSTR1_TRANSL,languajecu));

	printcu(TranslateWorck(&GENERIC2_TEXT_TRANSL,languajecu));
	SetScreenAtribute(0, bryellow);
	printcu(L"intro ");
	SetScreenAtribute(0, gray);
	printcu(TranslateWorck(&CONSOLE_INSTR2_TRANSL,languajecu));
	printcu(TranslateWorck(&GENERIC2_TEXT_TRANSL, languajecu));
	SetScreenAtribute(0, bryellow);
	printcu(L"exit ");
	SetScreenAtribute(0, gray);
	printcu(TranslateWorck(&CONSOLE_INSTR3_TRANSL, languajecu));
	SetScreenAtribute(0, brblue);
	printcu(TranslateWorck(&CONSOLE_BUTTON_TRANSL, languajecu));
	SetScreenAtribute(0, brred);
	printcu(L"\n");
	printcu(TranslateWorck(&WARNING_NOT_T_TRANSL, languajecu));
	printcu(L"\n");

	// the prompt
	SetScreenAtribute(0, brgreen);
	printc(L"root@local");
	SetScreenAtribute(0, gray);
	printc(L":");
	SetScreenAtribute(0, brblue);
	printc(L"^");
	SetScreenAtribute(0, gray);
	printc(L"# ");

	//
	// go to the action
	//

	if (
		count_spaces == 0
		)
	{
		SetScreenAtribute(0, brblue);
	}
	else {
		SetScreenAtribute(0, brgreen);
	}

	// the loop
	while (TRUE) {
		WHILESYSTEMRUNNING(
			ChangeToTextMode();
		globalsystemtable->ConOut->EnableCursor(globalsystemtable->ConOut, TRUE);
		uefi_call_wrapper(globalsystemtable->ConIn->ReadKeyStroke, 2, globalsystemtable->ConIn, &Key);
		// Variables para almacenar la posición del cursor
		UINTN CursorColumn;
		UINTN CursorRow;

		// Obtener la posición actual del cursor
		CursorColumn = globalsystemtable->ConOut->Mode->CursorColumn;
		CursorRow = globalsystemtable->ConOut->Mode->CursorRow;

		// key manager

		if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
			count_spaces = 0;
			Buffer[Index] = L'\0';
			CHAR16 handle_action_huh = ExecuteCommand(Buffer, globalimagehandle, globalsystemtable, mode);
			Index = 0; // Reiniciar el índice para la siguiente entrada
			globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_CYAN | EFI_BACKGROUND_BLACK);
			if (StrCmp(Buffer, L"intro") == 0)
			{
				printc(L"\n");
				SetScreenAtribute(0, gray);
				printc(L"basic commands:\n\n");

				SetScreenAtribute(0, gray);
				printc(L"<");
				SetScreenAtribute(0, brblue);
				printc(L"echo");
				SetScreenAtribute(0, gray);
				printc(L"> displays a text in the screen\n");

				SetScreenAtribute(0, gray);
				printc(L"<");
				SetScreenAtribute(0, brblue);
				printc(L"cls");
				SetScreenAtribute(0, gray);
				printc(L"> clears the screen\n");

				SetScreenAtribute(0, gray);
				printc(L"<");
				SetScreenAtribute(0, brblue);
				printc(L"writel");
				SetScreenAtribute(0, gray);
				printc(L"> displays a text in the screen without a line feed\n");

				SetScreenAtribute(0, gray);
				printc(L"<");
				SetScreenAtribute(0, brblue);
				printc(L"color");
				SetScreenAtribute(0, gray);
				printc(L"> changes the echo and writel text color\n");

				SetScreenAtribute(0, gray);
				printc(L"<");
				SetScreenAtribute(0, brblue);
				printc(L"bgcol");
				SetScreenAtribute(0, gray);
				printc(L"> changes the echo and writel text background color\n");

				SetScreenAtribute(0, gray);
				printc(L"<");
				SetScreenAtribute(0, brblue);
				printc(L"ReadLine");
				SetScreenAtribute(0, gray);
				printc(L"> prompts to the user a text and set the specific variable to the result\n");
				printc(L"\nTIP : dont try to open the historial in this terminal , you broken it");

			}
			if (StrCmp(Buffer, L"exit") == 0)
			{
				return EFI_SUCCESS;
			}
			printc(L"\n");
			SetScreenAtribute(0, brgreen);
			printc(L"root@local");
			SetScreenAtribute(0, gray);
			printc(L":");
			SetScreenAtribute(0, brblue);
			printc(L"^");
			SetScreenAtribute(0, gray);
			printc(L"# ");
			globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
			if (
				count_spaces == 0
				)
			{
				SetScreenAtribute(0, brgreen);
			}
			else {
				SetScreenAtribute(0, brblue);
			}
		}
		else if (Key.ScanCode == SCAN_ESC)
		{
			return EFI_SUCCESS;
		}
		else if (Key.UnicodeChar == CHAR_BACKSPACE) {
			if (
				Index != 0
				) {

				gotoxy(cursorx - 1, cursory);
				globalsystemtable->ConOut->SetCursorPosition(globalsystemtable->ConOut, CursorColumn - 1, CursorRow);
				printc(L" ");
				gotoxy(cursorx - 1, cursory);
				globalsystemtable->ConOut->SetCursorPosition(globalsystemtable->ConOut, CursorColumn - 1, CursorRow);
				if (
					Buffer[Index - 1] == L' '
					)
				{
					count_spaces--;
					if (
						count_spaces == 0
						)
					{
						SetScreenAtribute(0, brgreen);
					}
					else {
						SetScreenAtribute(0, brblue);
					}

					bool_t is_in_para = 0;

					u8 Inx = Index - 2;

					while (
						Buffer[Inx] != L'-'
						)
					{
						if (
							Buffer[Inx] == L' '
							)
						{
							is_in_para = 0;
							break;
						}
						else if (
							Buffer[Inx] == L'='
							)
						{
							is_in_para = 0;
							SetScreenAtribute(0, brcyan);
							break;
						}
						else {
							is_in_para = 1;
						}
						Inx--;
					}
					if (
						is_in_para
						)
					{
						SetScreenAtribute(0, bryellow);
					}
				}
				Buffer[--Index] = L'\0';
			}
		}
		else if (Key.ScanCode == SCAN_F12) {
			CHAR16 OtherCharacter = ACCES_TO_OTHER_CHARACTERS();
			if (OtherCharacter != NULL) {
				Buffer[Index++] = OtherCharacter;
				string a[100];
				SPrint(a, sizeof(a), L"%c", OtherCharacter);
				printc(a);
			}
		}
		else if (Key.UnicodeChar != 0) {
			Buffer[Index++] = Key.UnicodeChar;
			string a[100];
			SPrint(a, sizeof(a), L"%c", Key.UnicodeChar);

			if (
				Key.UnicodeChar == L'-'
				)
			{
				SetScreenAtribute(0, bryellow);
			}

			printc(a);
		}
		Buffer[Index] = L'\0'; // Asegurar el fin de cadena
			);

		if (
			Key.UnicodeChar == L'='
			)
		{

			bool_t is_in_para = 0;

			u8 Inx = Index - 2;

			while (
				Buffer[Inx] != L'-'
				)
			{
				if (
					Buffer[Inx] == L' '
					)
				{
					is_in_para = 0;
					break;
				}
				else {
					is_in_para = 1;
				}
				Inx--;
			}
			if (
				is_in_para
				)
			{
				SetScreenAtribute(0, brcyan);
			}
			}
			if (
				Key.UnicodeChar == L' '
				)
			{
				count_spaces++;
				if (
					count_spaces == 0
					)
				{
					SetScreenAtribute(0, brgreen);
				}
				else {
					SetScreenAtribute(0, brblue);
				}
		}

		if (
			Index > 511
			)
		{
			ShowPanic(SE_FUE_A_LA_BORDA);
		}

		// wait for a key
		globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
	}
	globalsystemtable->ConIn->Reset(globalsystemtable->ConIn, FALSE);
	globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
	Free_MEM_FILE_INT(CONSOLEINSTANCE);
	globalsystemtable->ConOut->EnableCursor(globalsystemtable->ConOut, FALSE);

	return EFI_SUCCESS;
}

/*
Calculator

Summary:
	the calculator
*/
PROGRAM
Calculator
(
	CSCHEME* Colors
)
{
	// input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	Pixels* savedbufsc = AllocatePool(sizeof(Pixels) * pixels);
	for (size_t i = 0; i < pixels; i++) {
		savedbufsc[i] = bufferscreen[i];
	}

	UINTN savpixels = pixels;

	INT8 tab;

	tab = 0;

	UINTN MaxColumns, MaxRows;

	MaxRows = (verticalResolution / row_size) / Conio->atributes->size;
	MaxColumns = (horizontalResolution / 8) / Conio->atributes->size;

	INT16 x = ((MaxColumns - 12) / 2);
	INT16 y = ((MaxRows - 14) / 2);

	typedef enum {
		Add = 1,
		Sub = 2,
		Div = 3,
		Mult = 4,
	} CALC_ACTION;

	CALC_ACTION ACTION;
	INT64 NUMBERS[2];

	NUMBERS[0] = 0;
	NUMBERS[1] = 0;

	INT64 CurrentNum = 0;

	FOREVER
	{
	DrawWindowsFrm(Colors, x, y, 12, 14, L"Calculator");

	CHAR16* OPTIONS[] = {
		L"123",
		L"456",
		L"789",
		L"0+-",
		L"/*=",
		L"<>B"
	};

	gotoxy((x + 1), (y + 2));
	SetScreenColor(Colors->buttonscolor, Colors->buttonstext);
	printcu(L"          ");

	gotoxy((x + 1), (y + 2));
	SetScreenColor(Colors->buttonscolor, Colors->buttonstext);
	CHAR16 ER[10];
	ValueToString(ER,FALSE,NUMBERS[CurrentNum]);
	printcu(ER);
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			gotoxy((x + 1) + (j * 3), (y + 4) + (i * 2));
			CHAR16 e[10];
			SetScreenColor(Colors->buttonscolor, Colors->buttonstext);
			if (tab == (j + (i * 3))) 
				SetScreenColor(Colors->buttonstext, Colors->buttonscolor)
				;
			SPrint(e, sizeof(e), L"%c", OPTIONS[i][j]);
			printcu(e);
		}
	}

	DrawScreen();
	// Esperar eventos de teclado
	globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
	globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);

	if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
		if (tab >= 0 & tab <= 9) {
			if (tab == 9) {
				NUMBERS[CurrentNum] = (NUMBERS[CurrentNum] * 10);
			}
			else
			{
				NUMBERS[CurrentNum] = (NUMBERS[CurrentNum] * 10) + (tab + 1);
			}
		}
		else if (tab == 10) {
			ACTION = Add;
			if (CurrentNum == 0) {
				CurrentNum = 1;
				NUMBERS[1] = 0;
			}
		}
		else if (tab == 11) {
			ACTION = Sub;
			if (CurrentNum == 0) {
				CurrentNum = 1;
				NUMBERS[1] = 0;
			}
		}
		else if (tab == 12) {
			ACTION = Div;
			if (CurrentNum == 0) {
				CurrentNum = 1;
				NUMBERS[1] = 0;
			}
		}
		else if (tab == 13) {
			ACTION = Mult;
			if (CurrentNum == 0) {
				CurrentNum = 1;
				NUMBERS[1] = 0;
			}
		}
		else if (tab == 14) {
			if (ACTION == Add) {
				NUMBERS[0] = NUMBERS[0] + NUMBERS[1];
				CurrentNum = 0;
			}
			else if (ACTION == Sub) {
				NUMBERS[0] = NUMBERS[0] - NUMBERS[1];
				CurrentNum = 0;
			}
			else if (ACTION == Div) {
				if ((NUMBERS[0] == 0) && (NUMBERS[1] == 0)) {
					DRAW_DIALOG_MSG(globalimagehandle, globalsystemtable, L"Error : divide by 0", Sceme->buttonscolor, Sceme->buttonstext);
				}
				else
				{
					NUMBERS[0] = NUMBERS[0] / NUMBERS[1];
					CurrentNum = 0;
				}
			}
			else if (ACTION == Mult) {
				NUMBERS[0] = NUMBERS[0] * NUMBERS[1];
				CurrentNum = 0;
			}
		}
	}
	else if (Key.ScanCode == SCAN_LEFT) {
		tab--;
	}
	else if (Key.ScanCode == SCAN_RIGHT) {
		tab++;
	}
	else if (Key.ScanCode == SCAN_ESC) {
		break;
	}
	else if (Key.ScanCode == SCAN_F2) {
		globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
		globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);

		if (Key.ScanCode == SCAN_UP) {
			pixels = savpixels;
			for (size_t i = 0; i < savpixels; i++) {
				bufferscreen[i] = savedbufsc[i];
			}
			DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Colors->backgroundcolor);
			y--;
		}
		else if (Key.ScanCode == SCAN_DOWN) {
			pixels = savpixels;
			for (size_t i = 0; i < savpixels; i++) {
				bufferscreen[i] = savedbufsc[i];
			}
			DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Colors->backgroundcolor);
			y++;
		}
		else if (Key.ScanCode == SCAN_LEFT) {
			pixels = savpixels;
			for (size_t i = 0; i < savpixels; i++) {
				bufferscreen[i] = savedbufsc[i];
			}
			DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Colors->backgroundcolor);
			x--;
		}
		else if (Key.ScanCode == SCAN_RIGHT) {
			pixels = savpixels;
			for (size_t i = 0; i < savpixels; i++) {
				bufferscreen[i] = savedbufsc[i];
			}
			DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Colors->backgroundcolor);
			x++;
		}
	}
	pixels = savpixels;
	for (size_t i = 0; i < savpixels; i++) {
		bufferscreen[i] = savedbufsc[i];
	}
	DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Colors->backgroundcolor);
	}
	return 0;
}

/*
BITMAP_MAKER

Summary:
	a 8x8 only 0/1 bitmap maker
*/
PROGRAM
BITMAP_MAKER
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	// the input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	// the cursor pos
	INT8 SheetX = 0;
	INT8 SheetY = 0;

	// the bitmap
	bool Bitmap[64] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
	}; // for 8 x 8 map draw pos

	// set the screen
	SetScreenAtribute(1, black);
	ClearScreen();

	// main loop
	FOREVER
	{
		SetScreenAtribute(1, black);
		ClearScreen();
		SetScreenAtribute(0, gray);

		// draw bitmap
		for (size_t j = 0; j < 8; j++) {
			for (size_t i = 0; i < 8; i++)
			{
				SetScreenAtribute(1, green);
				if (i == SheetX) {
					if (j == SheetY) {
						SetScreenAtribute(1, white);
					}
					else
					{
						if (Bitmap[i + j * 8] == 1) {
							SetScreenAtribute(1, brblue);
						}
					}
				}
				else
				{
					if (Bitmap[i + j * 8] == 1) {
						SetScreenAtribute(1, brblue);
					}
				}
				printcu(L" ");
			}
			gotoxy(0, cursory + 1);
		}
		// draw instructions
		printcu(L"\n");
		SetScreenAtribute(1, green);
		printcu(L" ");
		SetScreenAtribute(1, black);
		printcu(L" = 0\n");
		SetScreenAtribute(1, brblue);
		printcu(L" ");
		SetScreenAtribute(1, black);
		printcu(L" = 1");

		DrawScreen();
		// wait the key
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		// scan the key
		IF (Key.ScanCode == SCAN_ESC) {
			return EFI_SUCCESS;
		}
		ELIF(Key.ScanCode == SCAN_UP) {
			SheetY--;
		}
		ELIF(Key.ScanCode == SCAN_DOWN) {
			SheetY++;
		}
		ELIF(Key.ScanCode == SCAN_LEFT) {
			SheetX--;
		}
		ELIF(Key.ScanCode == SCAN_RIGHT) {
			SheetX++;
		}
		ELIF(Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
			if (Bitmap[SheetX + SheetY*8] == 0) {
				Bitmap[SheetX + SheetY * 8] = 1;
			}
			else
			{
				Bitmap[SheetX + SheetY * 8] = 0;
			}
		}

	}
	return EFI_SUCCESS;

}

/*
SNAKE

Summary:
	the clasic Snake game
*/
GAME
SNAKE
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	// prepare the input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	typedef struct {
		INT8 foodid;
		PIXELCOL Color;
	} SnakeFood;

	SnakeFood FoodOrange = { 1 , orange };
	SnakeFood FoodApple = { 2 , red };
	SnakeFood FoodPear = { 3 , green };

	// defines the balls (O_o)... of the snake
	typedef struct {
		INT8 x;
		INT8 y;
	} SnakeBALL;

	// variable of the snake directions
	INT8 snakedirection;

	// variable of the snake pos
	INT8 snakeposx;
	INT8 snakeposy;

	// snake size
	INT8 snakesize;

	// variables of the food pos
	INT8 foodx;
	INT8 foody;
	SnakeFood foodtype;

	// the balls (O_o)... of the snake
	SnakeBALL poses[30];

	// set the food pos
	foodx = 4;
	foody = 4;
	foodtype = FoodOrange;

	// set the snake default size and direction
	snakesize = 3;
	snakedirection = 3;

	// set the screen
	SetScreenAtribute(1, black);

	// set the snake pos
	snakeposx = 2;
	snakeposy = 4;

	// printcu is for print without update the screen
	ClearScreen();

	// displays the message
	printc(L"Press any key to start the FUUN yYYAY");
	SetScreenAtribute(1, black);
	SetScreenAtribute(1, gray);
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);

	// the game loop (while (true))
	GAME_LOOP
	{
		// set the screen
		SetScreenAtribute(1, black);
		ClearScreen();
		SetScreenAtribute(1, gray);

		// draws the floor
		SetScreenAtribute(1, darkgreen);
		printcu(L"            \n            \n            \n            \n            \n            \n            \n            \n            ");
		SetScreenAtribute(1, black);

		// a snake scroe
		string ea[100];

		SetScreenAtribute(0, brgreen);
		// prints the snake score
		printcu(L"\nSCORE: ");
		gotoxy(7, cursory);

		SPrint(ea,sizeof(ea), L"%d ", snakesize);
		SetScreenAtribute(0, brblue);

		printcu(ea);

		// prints the snake pos
		SetScreenAtribute(0, brgreen);
		printcu(L"\nPOS: ");
		gotoxy(5, cursory);
		SPrint(ea, sizeof(ea), L"x: %d y: %d", snakeposx, snakeposy);
		SetScreenAtribute(0, brblue);
		printcu(ea);

		// draws the snake
		for (size_t i = 0; i < snakesize; i++)
		{
			if (poses[i].x != 32) {
				if (poses[i].y != 32) {
					gotoxy(poses[i].x, poses[i].y);
				}
			}

			SetScreenAtribute(1, blue);
			printcu(L" ");
		}

		// draws the food
		gotoxy(foodx,foody);
		SetScreenAtribute(1, foodtype.Color);
		printcu(L" ");

		DrawScreen();
		// condition of the snake position
		if (snakedirection == 0) {
			snakeposy--;
		} 
		else if (snakedirection == 1) {
			snakeposy++;
		}
		else if (snakedirection == 2) {
			snakeposx--;
		}
		else if (snakedirection == 3) {
			snakeposx++;
		}

		// if game over
		if (snakeposx < -1) {
			printc(L"game over");
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
			return EFI_SUCCESS;

		}
		if (snakeposx > 12) {
			printc(L"game over");
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
			return EFI_SUCCESS;

		}
		if (snakeposy < -1) {
			printc(L"game over");
			SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
			return EFI_SUCCESS;

		}
		if (snakeposy > 9) {
			printc(L"game over");
			SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
			return EFI_SUCCESS;

		}

		// recorre the balls (O_o)... of the snake
		for (size_t i = 29; i > 0; i--) {
			if (i > snakesize) {
				poses[i].x = 32;
				poses[i].y = 32;
			}
			else
			{
				poses[i] = poses[i - 1]; // Desplaza cada segmento hacia adelante
			}
		}

		// if the snake touchs the food eat it and the food go to a new pos
		if (snakeposx == foodx) {
			if (snakeposy == foody) {
				snakesize++;
				INT8 foodtoselect = RandomInRange(1, 3);
				if (foodtoselect == 1) {
					foodtype = FoodOrange;
				}
				else if (foodtoselect == 2) {
					foodtype = FoodApple;
				}
				else if (foodtoselect == 3) {
					foodtype = FoodPear;
				}
				foodx = RandomInRange(0,7);
				foody = RandomInRange(0, 7);
			}
		}

		// if the score is 29 for dont bug and crash the system wins
		if (snakesize > 29) {
			printc(L"you reach the max score so you win");
			SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
			return EFI_SUCCESS;
		}
		// update the main ball (O_o)... of the snake pos
		poses[0].x = snakeposx;
		poses[0].y = snakeposy;

		// wait for not hurt the user
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 10, 100000);

		// read the key
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		// scan the key
		IF(Key.ScanCode == SCAN_ESC) {
			return EFI_SUCCESS;
		}
		ELIF(Key.ScanCode == SCAN_UP) {
			snakedirection = 0;
		}
		ELIF(Key.ScanCode == SCAN_DOWN) {
			snakedirection = 1;
		}
		ELIF(Key.ScanCode == SCAN_LEFT) {
			snakedirection = 2;
		}
		ELIF(Key.ScanCode == SCAN_RIGHT) {
			snakedirection = 3;
		}

		if (OemKeyNeedToSetNullW()) {
			Key.ScanCode = SCAN_NULL;
			Key.UnicodeChar = L'\0';
		}
	}
	return EFI_SUCCESS;
	// the ball (O_o)
}

/*
Fisher

Summary:
	the game of catch fishes
*/
GAME
Fisher
(
	CSCHEME* Colors
)
{
	EFI_INPUT_KEY Key; // the key pressed

	typedef struct {
		BOOLEAN inverted
			// 0=left
			// 1=right
			;
		INT8 Class
			// 1=happy
			// 2=star
			// 3=heart
			;
		UINTN Row
			// 1 to (ocean_size)
			;
		UINTN pos
			// 0 - screen size in colummns
			;
		BOOLEAN actived
			// 0=hidden
			// 1=showing
			;
	}
		Fish, // singular
		Fishes // plural
		;

	UINTN score = 0;
	INTN allocated_fish = -1;

	UINTN hooky = 0;
	UINTN playerpos = GET_MAX_COL / 2;

	Fishes Objets[10]; // the objets

	UINTN ocean_size = 10; // the ocean size

	PIXELCOL background = brblue; // the background
	PIXELCOL water = blue; // the water
	PIXELCOL fishes = brgreen; // the fishes
	PIXELCOL boat = darkorange; // the fishes

	SetScreenAtribute(1, background); // set the background
	ClearScreen(); // and set the complete background

	GAME_LOOP{

		SetScreenAtribute(1, background); // set the background
		ClearScreenCu(); // and set the complete background

		gotoxy(0,0);
		CHAR16 score_con[10];

		SetScreenAtribute(0, black); // set the background
		SPrint(score_con, sizeof(score_con), L"%d", score);
		printcu(score_con);

		SetScreenAtribute(1, water); // set the background
		for (size_t i = 0; i < ocean_size; i++)
		{
			gotoxy(0, GET_MAX_ROWS - i);
			printcu(taskbar);
		}

		for (size_t i = 0; i < hooky + 1; i++)
		{
			SetScreenAtribute(1, water); // set the background
			SetScreenAtribute(0, boat); // set the foreground
			gotoxy(playerpos, GET_MAX_ROWS - ocean_size + 1 + i);

			if (
				i == hooky // if the head
				) {
				cursorx--;
				cursorx--;
				SetScreenAtribute(0, water); // set the background
				printcu(L"\a");
				cursorx--;
				SetScreenAtribute(0, boat); // set the foreground
				printcu(L" ¿");
			}
			else
			{
				printcu(L"|");
			}
		}
		SetScreenAtribute(1, background); // set the background
		SetScreenAtribute(0, boat); // set the foreground
		gotoxy(playerpos,GET_MAX_ROWS - ocean_size);
		printcu(L"\x2a22");


		for (
			size_t i = 0; // the variable
			i < 10; // the count of objets
			i++ // next objet
			)
		{
			Fish Objet = Objets[i]; // the objet

			SetScreenAtribute(1, water); // set the background
			SetScreenAtribute(0, fishes); // set the foreground

			if (
				Objet.actived // is showing?
				) {
				gotoxy(
					(
						Objet.inverted // is inverted
						?
							(GET_MAX_COL) : 0 // the pos
						) + (Objet.pos * (Objet.inverted ? -1 : 1)) /* the pos */
					,
					((GET_MAX_ROWS) - ocean_size) + (Objet.Row) // the row
				); // go to fish position

				CHAR16 fish_body;

				switch (Objet.Class)
				{
				case 1:
					fish_body = L'\x2a1';
					// happy
				case 2:
					fish_body = L'\x2a4';
					// star
				case 3:
					fish_body = L'\x2a5';
					// heart
				default:
					fish_body = L'\x2a1';
					break;
				}

				CHAR16* fish_print[2] = {
					fish_body , // the character of the body
					0 // the terminator
				};

				printcu(fish_print);
			}
		}

		SetScreenAtribute(1, background); // set the background

		DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Conio->atributes->BG);
		
		DrawScreen(); // draw the screen

		// wait for not hurt the user
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 10, 100000);

		if (
			RandomInRange(1, 2) == 1 // if spawn fish
			)
		{
			for (
				size_t i = 0; // free search
				i < 10; // the fish count
				i++ // next fish
				)
			{
				if (
					!Objets[i].actived // if is free
					) {
					Objets[i].actived = TRUE; // show the objet
					Objets[i].Class = RandomInRange(1, 3); // the class
					Objets[i].inverted = (RandomInRange(0, 1)); // is inverted
					Objets[i].pos = 0; // pos
					Objets[i].Row = RandomInRange(1, ocean_size); // the row
					// i = 11; // stop
					break;
				}
			}
		}
		else // step the fishes
		{
			for (
				size_t i = 0; // free search
				i < 10; // the fish count
				i++ // next fish
				)
			{
				if (
					Objets[i].actived // if is actived
					)
				{
					if (i != allocated_fish) {
						if (Objets[i].pos == (GET_MAX_COL))
							Objets[i].actived = FALSE; // destroy
						else
							Objets[i].pos++; // sum the pos
					}
				}
			}
		}

		// read the key
		globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);

		if (
			Key.ScanCode == SCAN_ESC // exit?
			)
		{
			return 0; // exit
		}
		else if (
			Key.ScanCode == SCAN_UP
			)
		{
			if (hooky != 0) {
				hooky--;
				Objets[allocated_fish].Row--;
			}
			else {
				if (allocated_fish != -1) {
					Objets[allocated_fish].actived = FALSE; // kill fish
					score++;
					allocated_fish = -1; // desallocate fish
				}
			}
		}
		else if (
			Key.ScanCode == SCAN_DOWN
			)
		{
			hooky++;
		}
		else if (
			Key.UnicodeChar == L' ' // catch fish
			)
		{
			// fish allocation
			for (
				size_t i = 0; // allocater
				i < 10; // max
				i++ // next fish
				)
				if (
					Objets[i].Row == hooky + 1 && // row
					Objets[i].pos == playerpos // in pos
					)
					allocated_fish = i; // allocate fish
		}

		if (OemKeyNeedToSetNullW()) {
			Key.ScanCode = SCAN_NULL;
			Key.UnicodeChar = L'\0';
		}
	}

	return 0;
}

/*
Kosilka

Summary:
	a kosilka inspired-by kosilka kolibriOS game
	(la cortadora de cesped)
*/
GAME
Kosilka
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	// prepare the input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	// the maps
	CHAR16 tostmap[15][16] = {
		L"001200000010000",
		L"002001212020000",
		L"001001211010000",
		L"002001112020000",
		L"001000021010000",
		L"002000012020000",
		L"001000111010000",
		L"002000200020000",
		L"001000100210000",
		L"002000200100000",
		L"001001100200000",
		L"002002200100000",
		L"001000200200000",
		L"002200100111111",
		L"000000100000004",
	};
	CHAR16 spiral[15][16] = {
	L"001000000000000",
	L"002000000000000",
	L"001002121212100",
	L"002001000000200",
	L"001002000000100",
	L"002001002100200",
	L"001002001200100",
	L"002001002100200",
	L"001002441200100",
	L"002001212100200",
	L"001000000000100",
	L"002000000000200",
	L"001212121212100",
	L"000000000000000",
	L"000000000000000",
	};
	CHAR16 ondulations[15][16] = {
		L"000000000000000",
		L"000000000000000",
		L"121212121212100",
		L"000000000000000",
		L"000000000000000",
		L"001212121212121",
		L"000000000000000",
		L"000000000000000",
		L"121212121212100",
		L"000000000000000",
		L"000000000000000",
		L"001212121212121",
		L"000000000000004",
		L"000000000000004",
		L"212121212121212",
	};

	// to the program converts it in Grass
	CHAR16 map[15][16] = {
	   // 123456789ABCDEF
		L"001200000010000", // 1
		L"002001212020000", // 2
		L"001001211010000", // 3
		L"002001112020000", // 4
		L"001000021010000", // 5
		L"002000012020000", // 6
		L"001000111010000", // 7
		L"002000200020000", // 8
		L"001000100210000", // 9
		L"002000200100000", // A
		L"001001100200000", // B
		L"002002200100000", // C
		L"001000200200000", // D
		L"002200100111111", // E
		L"000000100000004", // F
	};
	// the map to load
	CHAR16 maptoload[15][16];

	// variable of the player pos
	INT8 playerx; // x
	INT8 playery; // y

	// set the screen
	SetScreenAtribute(1, black);

	// set the player pos
	playerx = 0; // x
	playery = 0; // y

	// printcu is for print without update the screen
	ClearScreen();

	// displays the message
	printc(L"Press any key to start the FUUN yYYAY");

	SetScreenAtribute(1, black);
	SetScreenAtribute(1, gray);
	// wait for key
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);

	// the game loop (while (true))
	GAME_LOOP
	{
		// set the screen
		SetScreenAtribute(1, black);
		ClearScreen();
		SetScreenAtribute(1, gray);

		// draws the floor
		SetScreenAtribute(1, darkyellow);
		for (size_t i = 0; i < 15; i++)
		{
			for (size_t j = 0; j < 15; j++)
			{
				// the grass
				if (
					map[i][j] == L'0' // comp
					) {
					SetScreenAtribute(1, bryellow); // the grass , parece desierto
				}
				// the trees
				else if (
					map[i][j] == L'1' // comp
					) {
					SetScreenAtribute(1, darkgreen); // the trees without the wood
				}
				// the rocks
				else if (
					map[i][j] == L'2' // comp
					) {
					SetScreenAtribute(1, darkgray); // the rocks
				}
				// the converted grass
				else if (
					map[i][j] == L'3' // comp
					) {
					SetScreenAtribute(1, darkyellow); // ya pasaste por ahi
				}
				// the victory point
				else if (
					map[i][j] == L'4' // comp
					) {
					SetScreenAtribute(1, brblue); // VICTORRYYY
				}
				printcu(L" "); // the cube
			}
			printcu(L"\n"); // new row
		}

		// draw player
		gotoxy(playerx,playery); // player pos
		SetScreenAtribute(1, brgreen); // player color
		printcu(L" "); // player eee

		// draw screen
		DrawScreen();

		// if you touch the meta
		if (
			map[playery][playerx] == L'4' // estas en la meta?
			) {
			UINTN randomselect = RandomInRange(1,3); // select your next adventure

			// selection: 1 - tost
			if (
				randomselect == 1 // selection
				) {
				// load the map
				for (size_t i = 0; i < 15; i++) { 
					for (size_t j = 0; j < 15; j++) 
					maptoload[i][j] = tostmap[i][j]; 
				}
			}
			// selection: 2 - spirals
			else if (
				randomselect == 2 // selection
				) {
				// load the map
				for (size_t i = 0; i < 15; i++) {
					for (size_t j = 0; j < 15; j++)
						maptoload[i][j] = spiral[i][j];
				}
			}
			// selection: 3 - ondulations
			else if (
				randomselect == 3 // selection
				) {
				// load the map
				for (size_t i = 0; i < 15; i++) { // the rows
					for (size_t j = 0; j < 15; j++)
						maptoload[i][j] = ondulations[i][j]; // the colummns
				}
			}
			// load the map
			for (size_t i = 0; i < 15; i++) { // the rows
				for (size_t j = 0; j < 15; j++)
					map[i][j] = maptoload[i][j]; // the colummns 
			}
			// reset player pos
			playerx = 0; // x
			playery = 0; // y
		}

		// read the key
		globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		// scan the key
		IF(Key.ScanCode == SCAN_ESC) {
			return EFI_SUCCESS; // exit of the game
		}
		else if (Key.ScanCode == SCAN_DOWN) {
			if (map[playery + 1][playerx] == L'0' || map[playery + 1][playerx] == L'4') {
				playery++;
				map[playery - 1][playerx] = L'3';
			}
		}
		else if (Key.ScanCode == SCAN_UP) {
			if (playery != 0) {
				if (map[playery - 1][playerx] == L'0' || map[playery - 1][playerx] == L'4') {
					playery--;
					map[playery + 1][playerx] = L'3';
				}
			}
		}
		else if (Key.ScanCode == SCAN_LEFT) {
			if (map[playery][playerx - 1] == L'0' || map[playery][playerx - 1] == L'4') {
				playerx--;
				map[playery][playerx + 1] = L'3';
			}
		}
		else if (Key.ScanCode == SCAN_RIGHT) {
				if (map[playery][playerx + 1] == L'0' || map[playery][playerx + 1] == L'4') {
					playerx++;
					map[playery][playerx - 1] = L'3';
				}
			}
	}
	return EFI_SUCCESS;
}

/*
ADIVINA_EL_NUMERO

Summary:
	the game of try to write the correct number
*/
GAME
ADIVINA_EL_NUMERO
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	// ITSS FUNNN TIMEEE JIJIJI

	// prepare the variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	// prepare the number variable
	INT8 Number;

	// Prepare the Min and Max of number that randomize it
	INT8 MaxNumber;
	INT8 MinNumber;

	// Prepare the user prompt
	UINTN UserPrompt;

	// Set the screen
	SetScreenAtribute(1, black);
	ClearScreen();

	GAME_LOOP
	{
		ChangeToTextMode();
		// Prepare Numbers
		MaxNumber = RandomInRange(0,99);

		FOREVER
		{
			MinNumber = RandomInRange(0, MaxNumber);

			// if the min number finally is not out of the MaxNumber = YEEES
			if (MinNumber < MaxNumber) {
				break;
			}
		}

		// prepare the number
		Number = RandomInRange(MinNumber,MaxNumber);

		// print the message
		SetScreenAtribute(0, cyan);
		printc(L"Which Is The Number that is in range ");
		string a[100];
		SPrint(a, sizeof(a), L"%d to %d", MinNumber, MaxNumber);
		printc(a);
		printc(L"\n");

		// ask the user the answare
		SetScreenAtribute(0, white);
		CHAR16* input = NULL;
		ReadLineSerius(&input);
		if (input == NULL) {
			break;
		}		
		
		UserPrompt = Atoi(input);

		// Yeah go to the verification

		CHAR16* state;
		if (UserPrompt == Number) {
			SetScreenAtribute(0, brblue);
			state = L"Perfect";
		}
		else if (UserPrompt >= (Number - 2) && UserPrompt <= (Number + 2)) {
			SetScreenAtribute(0, brcyan);
			state = L"Good";
		}
		else if (UserPrompt >= (Number - 8) && UserPrompt <= (Number + 8)) {
			SetScreenAtribute(0, brgreen);
			state = L"Great";
		}
		else if (UserPrompt >= (Number - 14) && UserPrompt <= (Number + 14)) {
			SetScreenAtribute(0, orange);
			state = L"hmmm passable";
		}
		else if (UserPrompt >= (Number - 32) && UserPrompt <= (Number + 32)) {
				SetScreenAtribute(0, brred);
				state = L"bad";
		}
		else {
			SetScreenAtribute(0, red);
			state = L"soo bad but dont worry you can try agin";

		}

		printc(L"\n");
		printc(state);
		printc(L"\n");

	}
	return EFI_SUCCESS;
}

/*
SchemeEditor
v
Summary:
	The Schemes editor
*/
CSCHEME*
SchemeEditor
(
	CSCHEME* SchemeToEdit
)
{ 
	CSCHEME* SchemeWorck;

	SchemeWorck = SchemeToEdit;
	CHAR16* Options[] = {
		L"black",
		L"gray",
		L"darkgray",
		L"white",
		L"darkred",
		L"red",
		L"brred",
		L"darkorange",
		L"orange",
		L"brorange",
		L"darkyellow",
		L"yellow",
		L"bryellow",
		L"darkgreen",
		L"green",
		L"brgreen",
		L"darkcyan",
		L"cyan",
		L"brcyan",
		L"darkblue",
		L"blue",
		L"brblue"
	};

	// input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	INT8 tab;

	CHAR16* bg;
	CHAR16* btcol;
	CHAR16* bttext;

	tab = 0;
	bg = CtoA(&SchemeWorck->backgroundcolor);
	btcol = CtoA(&SchemeWorck->buttonscolor);
	bttext = CtoA(&SchemeWorck->buttonstext);

	INT8 WindowSize = 30;
	INT16 WindowPosX = (globMaxColumns - WindowSize) / 2;
	INT16 WindowsPosY = globMaxRows / 2;

	FOREVER{
		gotoxy(WindowPosX, WindowsPosY - 3);
		SetScreenAtribute(1, SchemeWorck->buttonstext);
		SetScreenAtribute(0, SchemeWorck->buttonscolor);
		printcu(L"                              ");
		gotoxy(WindowPosX, WindowsPosY - 3);
		printcu(L"Theme Edit");

		SetScreenAtribute(1, SchemeWorck->buttonscolor);
		SetScreenAtribute(0, SchemeWorck->buttonstext);
		gotoxy(WindowPosX, WindowsPosY - 2);
		printcu(L"                              ");
		gotoxy(WindowPosX, WindowsPosY - 1);
		printcu(L"                              ");
		gotoxy(WindowPosX, WindowsPosY);
		printcu(L"                              ");
		gotoxy(WindowPosX, WindowsPosY + 1);
		printcu(L"                              ");
		gotoxy(WindowPosX, WindowsPosY + 2);
		printcu(L"                              ");
		gotoxy(WindowPosX, WindowsPosY + 3);
		printcu(L"                              ");

		gotoxy(WindowPosX + 1, WindowsPosY);
		SetScreenAtribute(0, white);
		SetScreenAtribute(1, SchemeWorck->backgroundcolor);
		if (tab == 0) {
			printcu(L"\a");
		}
		else
		{
			printcu(L"-");
		}

		gotoxy(WindowPosX + 3, WindowsPosY);
		SetScreenAtribute(0, white);
		SetScreenAtribute(1, SchemeWorck->buttonscolor);
		if (tab == 1) {
			printcu(L"\a");
		}
		else
		{
			printcu(L"-");
		}

		gotoxy(WindowPosX + 5, WindowsPosY);
		SetScreenAtribute(0, white);
		SetScreenAtribute(1, SchemeWorck->buttonstext);
		if (tab == 2) {
			printcu(L"\a");
		}
		else
		{
			printcu(L"-");
		}
		DrawScreen();
		// Esperar eventos de teclado
		globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
		globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);

		if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {

		}
		else if (Key.ScanCode == SCAN_LEFT) {
			tab--;
		}
		else if (Key.ScanCode == SCAN_RIGHT) {
			tab++;
		}
		else if (Key.ScanCode == SCAN_ESC) {
			break;
		}
	}
}

/*
Desktop

Summary:
	the S-SUN desktop
*/
PROGRAM 
Desktop
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	InitializeLib(globalimagehandle, globalsystemtable);
	EFI_FILE_PROTOCOL* Root;
	EFI_STATUS Status;

	//
	// loading screen (yes unesesary but epic)
	//

	INT8 loadingframe = 0;
	for (size_t i = 0; i < 21; i++)
	{
		ClearScreen();
		loadingframe++;
		DrawLoadingCircle((((GET_MAX_COL)-3) / 2), (((GET_MAX_ROWS)-2) / 2), loadingframe);

		gotoxy((((GET_MAX_COL)-StrLen(TranslateWorck(&GENERIC_TEXT1_TRANSL, languajecu))) / 2), (((GET_MAX_ROWS)-2) / 2) + 4);
		printc(TranslateWorck(&GENERIC_TEXT1_TRANSL,languajecu));

		if (
			loadingframe > 7
			)
		{
			loadingframe = 0;
		}

		gBS->Stall(150000);
	}

	//
	// variables
	//

	BOOLEAN OnlyIcons; // only icons

	OnlyIcons = 1;

	// input variables
	EFI_EVENT Event[2];
	EFI_INPUT_KEY Key;

	EFI_SIMPLE_POINTER_PROTOCOL* mouse = 0;
	EFI_SIMPLE_POINTER_STATE     State;

	//
	// init the mouse (unused)
	//

	gBS->LocateProtocol(
		&gEfiSimplePointerProtocolGuid,
		NULL,
		(VOID**)&mouse
	);

	Status = mouse->Reset(mouse, TRUE);

	EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL* TextInputEx;
	EFI_KEY_STATE KeyState;

	/*
	Event[0] = SystemTable->ConIn->WaitForKey;
	Event[1] = TextInputEx->WaitForKeyEx;
	*/

	//
	// navegation vars
	//

	// the option
	INT8 Tab = 0;
	INT8 optionud = 0;

	// the screen size
	UINTN MaxColumns, MaxRows;

	//
	// misc pre settings
	//

	// the editor save
	CHAR16 textsaved[1024];

	//
	// parse the screen
	//

	// set the screen
	MaxColumns = (gop->Mode->Info->HorizontalResolution / 8) / Conio->atributes->size;
	MaxColumns = MaxColumns - 1;

	Event[0] = gST->ConIn->WaitForKey;

	//
	// set the screen
	//

	ChangeToGrapichalMode();

	//
	// reserve instance
	//

	MEM_FILE_INT* DESKTOPINSTANCE = Create_MEM_FILE_INT(L"DESKTOPINSTANCE", 1);

	//
	// back to screen parser
	//

	SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &MaxColumns, &MaxRows);
	globalsystemtable->ConOut->ClearScreen(globalsystemtable->ConOut);

	//
	// show a gray screen
	//

	SetScreenAtribute(1, gray);
	ClearScreen();
	// DRAW_TEXT_DIALOG_NO_WAIT(ImageHandle, SystemTable, L"Please wait...", EFI_WHITE, EFI_BACKGROUND_CYAN);
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);

	//
	// clear screen and set grp mode
	//

	ClearScreen();
	ChangeToGrapichalMode();	

	// DRAW_DIALOG_MSG_CONFIRM(ImageHandle, SystemTable, L"Welcome to S-SUN, Do you want see the survival guide?");

	//
	// set desktop values
	//

	SetScreenAtribute(1, Sceme->backgroundcolor);

	ChangeToGrapichalMode();
	ClearScreen();

	// ShowCenteredDialoga(ImageHandle, SystemTable, L"if you dont know what you do press F1", Sceme->buttonstext, Sceme->buttonscolor);
	DrawScreen();

	//
	// desktop loop
	//

	while (TRUE) {
		UINTN index;

		//
		// screen sets
		//
		ChangeToGrapichalMode();
		SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);
		
		//
		// set abreviations
		//

		PIXELCOL syscolor2 = Sceme->buttonscolor;
		PIXELCOL sysbg = Sceme->backgroundcolor;
		PIXELCOL sysbtext = Sceme->buttonstext;

		//
		// items position
		//

		UINTN barbt0 = OnlyIcons ? (((GET_MAX_COL) - 13) / 2) : 0;
		UINTN barbt1 = barbt0 + (!OnlyIcons ? 8 : 3);
		UINTN barbt2 = barbt1 + 1 + (!OnlyIcons ? StrLen(TranslateWorck(&THEMES_BUTTON_TRANSL, languajecu)) : 1);
		UINTN barbt3 = barbt2 + 1 + (!OnlyIcons ? StrLen(TranslateWorck(&GAMES_BUTTON_TRANSL, languajecu)) : 1);
		UINTN barbt4 = barbt3 + 1 + (!OnlyIcons ? StrLen(TranslateWorck(&LANGS_BUTTON_TRANSL, languajecu)) : 1);
		UINTN barbt5 = barbt4 + 1 + (!OnlyIcons ? StrLen(TranslateWorck(&TEST_BUTTON_TRANSL, languajecu)) : 1);
		UINTN barbt6 = barbt5 + 1;

		//
		// parse screen (again)
		//

		MaxColumns = (gop->Mode->Info->HorizontalResolution / 8) / Conio->atributes->size;
		MaxColumns = MaxColumns - 1;
		MaxRows = (gop->Mode->Info->VerticalResolution / row_size) / Conio->atributes->size;

		//
		// set the desktop
		//

		SetScreenAtribute(1, sysbg);

		ClearScreen();

		//
		// draw bar
		//

		gotoxy(0, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		printcu(taskbar);

		// DRAW START
		gotoxy(barbt0, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 0) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printcu(OnlyIcons ? L"\x2ac" : L"\x2ac S-SUN");

		// DRAW Themes
		gotoxy(barbt1, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 1) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printcu(OnlyIcons ? L"\x2a8" : TranslateWorck(&THEMES_BUTTON_TRANSL, languajecu));
	
		// DRAW games
		gotoxy(barbt2, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 2) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printcu(OnlyIcons ? L"\x2a9" : TranslateWorck(&GAMES_BUTTON_TRANSL, languajecu));

		// DRAW langs
		gotoxy(barbt3, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 3) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printcu(OnlyIcons ? L"\x2aa" : TranslateWorck(&LANGS_BUTTON_TRANSL, languajecu));

		// DRAW tests
		gotoxy(barbt4, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 4) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printcu(OnlyIcons ? L"\x2ab" : TranslateWorck(&TEST_BUTTON_TRANSL, languajecu));

		// DRAW comunity
		gotoxy(barbt5, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 5) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printcu(OnlyIcons ? L"\x2a1" : TranslateWorck(&CMN_BUTTON_TRANSL, languajecu));

		//
		// menus drawing
		//

		if (optionud > 0) {
			if (Tab == 0) {
				for (INT8 i = 0; i < 8; i++)
				{
					gotoxy(barbt0, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printcu(L"             ");
				}

				// draw Shutdown button
				gotoxy(barbt0, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&OFF_BUTTON_TRANSL, languajecu));

				// draw restart button
				gotoxy(barbt0, MaxRows - 1 - 2);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&RESTART_BUTTON_TRANSL, languajecu));

				// draw help button
				gotoxy(barbt0, MaxRows - 1 - 3);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&HELP_BUTTON_TRANS, languajecu));

				// draw editor button
				gotoxy(barbt0, MaxRows - 1 - 4);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 4) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&TEXTEDITOR_BUTTON_TRANSL, languajecu));

				// draw time view
				gotoxy(barbt0, MaxRows - 1 - 5);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 5) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&GETTIME_BUTTON_TRANSL, languajecu));

				// draw date view
				gotoxy(barbt0, MaxRows - 1 - 6);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 6) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&GETDATE_BUTTON_TRANSL, languajecu));

				// draw command prompt shortcut
				gotoxy(barbt0, MaxRows - 1 - 7);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 7) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&CONSOLE_BUTTON_TRANSL, languajecu));

				// draw calculator shortcut
				gotoxy(barbt0, MaxRows - 1 - 8);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 8) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(TranslateWorck(&CALC_BUTTON_TRANSL, languajecu));
			}
			if (Tab == 1) {
				for (INT8 i = 0; i < 13; i++)
				{
					gotoxy(barbt1, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printcu(L"        ");
				}

				// draw "Default" theme button
				gotoxy(barbt1, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Default");

				// draw "Nature" theme button
				gotoxy(barbt1, MaxRows - 1 - 2);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Nature");

				// draw "Boy" theme button
				gotoxy(barbt1, MaxRows - 1 - 3);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Boy");

				// draw "Girl" theme button
				gotoxy(barbt1, MaxRows - 1 - 4);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 4) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Girl");

				// draw "Billy" theme button
				gotoxy(barbt1, MaxRows - 1 - 5);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 5) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Billy");

				// draw "Fire" theme button
				gotoxy(barbt1, MaxRows - 1 - 6);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 6) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Fire");

				// draw "Ocean" theme button
				gotoxy(barbt1, MaxRows - 1 - 7);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 7) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Ocean");

				// draw "Futuristic" theme button
				gotoxy(barbt1, MaxRows - 1 - 8);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 8) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Futurist");

				// draw "Desert" theme button
				gotoxy(barbt1, MaxRows - 1 - 9);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 9) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Desert");

				// draw "Reactor" theme button
				gotoxy(barbt1, MaxRows - 1 - 10);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 10) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Reactor");

				// draw "Night" theme button
				gotoxy(barbt1, MaxRows - 1 - 11);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 11) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Night");

				// draw "Sun days" theme button
				gotoxy(barbt1, MaxRows - 1 - 12);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 12) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Sun Days");

				// draw "Holidays" theme button
				gotoxy(barbt1, MaxRows - 1 - 13);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 13) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Holidays");
			}
			if (Tab == 2) {
				for (INT8 i = 0; i < 5; i++)
				{
					gotoxy(barbt2, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printcu(L"                 ");
				}

				// draw "ADIVINA EL NUMERO" GAME button
				gotoxy(barbt2, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"ADIVINA EL NUMERO");

				// draw "BITMAP_MAKER" button
				gotoxy(barbt2, MaxRows - 2 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"BITMAP_MAKER");

				// draw "Snake" GAME button
				gotoxy(barbt2, MaxRows - 3 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Snake");

				// draw "Kosilka" GAME button
				gotoxy(barbt2, MaxRows - 4 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 4) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Kosilka");

				// draw "Fisher" GAME button
				gotoxy(barbt2, MaxRows - 5 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 5) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Fisher");
			}
			if (Tab == 3) {
				for (INT8 i = 0; i < 3; i++)
				{
					gotoxy(barbt3, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printcu(L"         ");
				}

				// draw "English" lang button
				gotoxy(barbt3, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"English");

				// draw "Spanish" lang button
				gotoxy(barbt3, MaxRows - 2 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Espanol");

				// draw "Francais" lang button
				gotoxy(barbt3, MaxRows - 3 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"Francais");
			}
			if (Tab == 4) {
				for (INT8 i = 0; i < 3; i++)
				{
					gotoxy(barbt4, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printcu(L"                        ");
				}

				// draw "MsgBoxes" test button
				gotoxy(barbt4, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"MsgBoxes");

				// draw "WindowTest" test button
				gotoxy(barbt4, MaxRows - 2 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printcu(L"WindowTest");

			}
			if (Tab == 5) {
				for (INT8 i = 0; i < GetThridyPartyProgramsCount(); i++)
				{
					gotoxy(barbt5, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printcu(L"                    ");
				}

				// loop for draw buttons
				for (size_t i = 0; i < GetThridyPartyProgramsCount(); i++)
				{
					gotoxy(barbt5, MaxRows - 2 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					if (optionud == (i + 1)) {
						SetScreenAtribute(1, sysbtext);
						SetScreenAtribute(0, syscolor2);
					}
					printcu(ProgramsList[i].Name);
				}

			}

		}

		//
		// draw finally the screen
		//

		DrawScreen();

		//
		// wait for key
		//

		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &index);
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		// -----------
		// key managen
		// -----------

		if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
			//
			// enter in op
			//
			if (optionud == 0) {
				optionud = 1;
			}

			//
			// Start Menu
			//
			else if (Tab == 0) {
				if (optionud == 1) {
					SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
					bool confirm = DRAW_DIALOG_MSG_CONFIRM(ImageHandle, SystemTable, TranslateWorck(&SHUTDOWNCONFIRM_TRANS, languajecu), syscolor2, sysbtext);
					if (confirm) {
						ClearScreen();
						uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
						SSUNSCREENLOGOUT(ImageHandle, SystemTable);
						globalsystemtable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
					}
				}
				if (optionud == 2) {
					SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
					bool confirm = DRAW_DIALOG_MSG_CONFIRM(ImageHandle, SystemTable, TranslateWorck(&RESETCONFIRM_TRANS, languajecu), syscolor2, sysbtext);
					if (confirm) {
						ClearScreen();
						uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
						SSUNSCREENLOGOUT(ImageHandle, SystemTable);
						globalsystemtable->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
					}
				}
				else if (optionud == 3) {
					editor(ImageHandle, SystemTable, TranslateWorck(&HELP_TEXT_TRANS, languajecu));
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				else if (optionud == 4) {
					editor(ImageHandle, SystemTable, NULL);
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				else if (optionud == 5) {
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, KERNEL_GET_TIME(SystemTable), syscolor2, sysbtext);

				}
				else if (optionud == 6)
				{
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, KERNEL_GET_DATE(SystemTable), syscolor2, sysbtext);
				}
				else if (optionud == 7)
				{
					ssun_main(ImageHandle, SystemTable, L"DEV");
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				else if (optionud == 8)
				{
					Calculator(Sceme);
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
			}

			//
			// themes menu
			//
			else if (Tab == 1) {
				if (optionud == 1) {
					Sceme = SCDefault;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 2) {
					Sceme = SCNature;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 3) {
					Sceme = SCBoy;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 4) {
					Sceme = SCGirl;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 5) {
					Sceme = SCBilly;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 6) {
					Sceme = SCFire;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 7) {
					Sceme = SCOcean;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 8) {
					Sceme = SCFuturistic;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 9) {
					Sceme = SCDesert;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 10) {
					Sceme = SCReactor;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 11) {
					Sceme = SCNight;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 12) {
					Sceme = SCSunDays;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
				else if (optionud == 13) {
					Sceme = SCHolidays;
					LibSetNVVariable(L"Theme", &SmallVariables, sizeof(Sceme), Sceme);
					LibSetNVVariable(L"ThemeT", &SmallVariables, sizeof(Sceme->buttonstext), (VOID*)&Sceme->buttonstext);
				}
			}

			//
			// games menu
			//

			else if (Tab == 2) {
				if (optionud == 1) {
					ADIVINA_EL_NUMERO(ImageHandle, SystemTable);
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				if (optionud == 2) {
					BITMAP_MAKER(ImageHandle, SystemTable);
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				if (optionud == 3) {
					SNAKE(ImageHandle, SystemTable);
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				if (optionud == 4) {
					Kosilka(ImageHandle, SystemTable);
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				if (optionud == 5) {
					Fisher(Sceme);
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
			}

			//
			// languajes menu
			//

			else if (Tab == 3) {
				if (optionud == 1) {
					languajecu = L"en";
					UpdateLanguaje();
				}
				else if (optionud == 2) {
					languajecu = L"es";
					UpdateLanguaje();
				}
				else if (optionud == 3) {
					languajecu = L"fr";
					UpdateLanguaje();
				}
			}

			//
			// comunity section
			//

			else if (Tab == 4) {
				if (optionud == 1) {
					ExecuteScript(L"MsgBox A red spy in the base\necho &confirm Are you the red spy?", ImageHandle, SystemTable);
				}
				else if (optionud == 2) {
					ExecuteScript(L"# prepare the variables\n\nEditMem FRWindowTitle=Test\n\nEditMem FRWindowX=0\n\nEditMem FRWindowY=0\n\nEditMem FRWindowSX=5\n\nEditMem FRWindowSY=5\n\ncall main\n\nsection\n\nmain\n\nEditMem key=&key_and_wait\n\n\n\n# restore the screen to the state before of the execution of the script\n\nrestore_screen\n\n\n\nif cmp&%key=ESC:\n\n\n\n# move the window if you press the sandart window control key\n\nif cmp&%key=F2:EditMem movew=1\n\n\n\n# if you move the window read the key\n\nif cmp&%movew=1:EditMem key=&key_and_wait\n\n\n\n# if the window moves process the key\n\nif cmp&%movew=1:if cmp&%key=up arrow:EditMem FRWindowY=opr&%FRWindowY-1\n\nif cmp&%movew=1:if cmp&%key=down arrow:EditMem FRWindowY=opr&%FRWindowY+1\n\nif cmp&%movew=1:if cmp&%key=left arrow:EditMem FRWindowX=opr&%FRWindowX-1\n\nif cmp&%movew=1:if cmp&%key=right arrow:EditMem FRWindowX=opr&%FRWindowX+1\n\n\n\nEditMem movew=0\n\n\n\n# draw the window\n\nint 1f\n\n\n\nret\n", ImageHandle, SystemTable);
				}
			}
			else if (Tab == 5) { 
				if (
					ProgramsList[optionud - 1].Name != NULL
					)
				{
					if (
						ProgramsList[optionud - 1].Function != NULL
						)
					{
						ProgramsList[optionud - 1].Function();
					}
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
			}
		}

		//
		// shortcuts keys
		//

		else if (Key.ScanCode == SCAN_F1) {

			//
			// only icons feature exclusive help
			//

			if (OnlyIcons) {
				switch (Tab)
				{
				case 1:
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, TranslateWorck(&THEMES_BUTTON_TRANSL, languajecu), syscolor2, sysbtext);
					break;
				case 2:
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, TranslateWorck(&GAMES_BUTTON_TRANSL, languajecu), syscolor2, sysbtext);
					break;
				case 3:
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, TranslateWorck(&LANGS_BUTTON_TRANSL, languajecu), syscolor2, sysbtext);
					break;
				case 4:
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, TranslateWorck(&TEST_BUTTON_TRANSL, languajecu), syscolor2, sysbtext);
					break;
				case 5:
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, TranslateWorck(&CMN_BUTTON_TRANSL, languajecu), syscolor2, sysbtext);
					break;
				default:
					break;
				}
			}
			else {
				editor(ImageHandle, SystemTable, TranslateWorck(&HELP_TEXT_TRANS, languajecu));
				SetScreenAtribute(0, gray);
				printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
				SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
			}
			}
		else if (Key.ScanCode == SCAN_ESC) {
				if (optionud != 0) {
					optionud = 0;
				}
				else
				{
					SetScreenAtribute(1, gray);
					SetScreenAtribute(0, black);
					ClearScreen();
					printc(L"ZZzz...");
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &index);
					SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
				}
			}

		//
		// tab managen
		//

		else if (Key.ScanCode == SCAN_LEFT) {
				if (Tab > 0) { Tab--; }
			}
		else if (Key.ScanCode == SCAN_RIGHT) {
				Tab++;
			}

		//
		// menu managen
		//

		else if (Key.ScanCode == SCAN_DOWN) {
				if (optionud > 0) { optionud--; }
			}
		else if (Key.ScanCode == SCAN_UP) {
				optionud++;
			}
	}

	Free_MEM_FILE_INT(DESKTOPINSTANCE);
	return EFI_SUCCESS;
}

/*
EmergencyPrompt

Summary:
	recurre to this if the user interrupt the boot o something went worng
*/
PROGRAM
EmergencyPrompt
(
)
{
	while (true)
	{
		SetScreenAtribute(0, gray);
		printc(L"# ");
		CHAR16* Prompt = ReadLineSeriusWorck();
		printc(L"\n");

		if (
			StrCmp(Prompt, L"./desktop") == 0
			)
		{
			BootStage = 3;
			Desktop(globalimagehandle,globalsystemtable);
		}
		else if (
			StrCmp(Prompt, L"./console") == 0
			)
		{
			BootStage = 3;
			Console(L"Dev");
		}
		else if (
			StrnCmp(Prompt, L"./console ",10) == 0
			)
		{
			ExecuteCommand(Prompt + 10, globalimagehandle, globalsystemtable, L"dev");
		}
		else if (
			StrCmp(Prompt, L"exit") == 0
			)
		{
			gBS->Exit(globalimagehandle, EFI_SUCCESS, 0,L"");
		}
		else if (
			StrCmp(Prompt, L"") == 0
			)
		{

		}
		else {
			printc(L"unknow command\n");
		}
	}
}

VOID
DrawLogo
(
	INT16 x,
	INT16 y,
	INT8 Brightness
)
{
	PIXELCOL color1 = brblue;
	PIXELCOL color2 = brcyan;
	PIXELCOL color3 = brgreen;

	PIXELCOL logocol1 = brorange;
	PIXELCOL logocol2 = bryellow;

	switch (Brightness)
	{
	case 3:
		color1 = brblue;
		color2 = brcyan;
		color3 = brgreen;

		logocol1 = brorange;
		logocol2 = bryellow;
		break;
	case 2:
		color1 = blue;
		color2 = cyan;
		color3 = green;

		logocol1 = orange;
		logocol2 = yellow;
		break;
	case 1:
		color1 = darkblue;
		color2 = darkcyan;
		color3 = darkgreen;

		logocol1 = darkorange;
		logocol2 = darkyellow;
		break;
	case 0:
		color1 = black;
		color2 = black;
		color3 = black;

		logocol1 = black;
		logocol2 = black;
		break;
	default:
		break;
	}

	// the diammond trace
	SetScreenAtribute(0, logocol1);
	gotoxy(x, y);
	printcu(L"\x763\x763\x3a6\x2ad\x3a7\x2763\x763\x3a6\x763\x763\x763\x3a7\x2763\x763\x3a7\x763\x763\x763\x3a6\x2763\x763\x763\x3a7_\x3a6\x763");

	// the diammond center
	gotoxy(x, y);
	SetScreenAtribute(0, logocol2);
	printcu(L"\x763\x763\x763\x763\x763\x763\x2763\x763\x763\x763\x3a5\x763\x763\x2763\x763\x763\x763\x3a4\x763\x763\x2763");

	// draw the letters S-SUN
	INT32 lettersx = x + 7;
	
	// S
	SetScreenAtribute(0, color1);
	gotoxy(lettersx, y + 1);
	printcu(L"/\x2ad\x2763\\_\x2763_/");

	// -
	SetScreenAtribute(0, color2);
	gotoxy(lettersx + 3, y + 1);
	printcu(L"\x2763--");

	// S
	SetScreenAtribute(0, color3);
	gotoxy(lettersx + 3 + 3, y + 1);
	printcu(L"/\x2ad\x2763\\_\x2763_/");

	// U
	gotoxy(lettersx + 3 + 3 + 3, y + 1);
	printcu(L"| \x2ae\x2763| \x2ae\x2763\\_/ ");

	// N
	gotoxy(lettersx + 3 + 3 + 3 + 4, y + 1);
	printcu(L"| \x2ae\x2763|\\\x2ae\x2763| \x2ae");
}

VOID
DrawLoadingCircle
(
	INT16 x,
	INT16 y,
	INT16 eframe
)
{
	gotoxy(x, y);

	SetScreenAtribute(0, gray);
	switch (eframe)
	{
	case 0:
		printc(L"/ \x2763\\_");
		break;
	case 1:
		printc(L"/\x2ad\x2763\\");
		break;
	case 2:
		printc(L"/\x2ad\\\x2763");
		break;
	case 3:
		printc(L"/\x2ad\\\x2763  /");
		break;
	case 4:
		printc(L" \x2ad\\\x2763 _/");
		break;
	case 5:
		printc(L"  \\\x2763\\_/");
		break;
	case 6:
		printc(L"   \x2763\\_/");
		break;
	case 7:
		printc(L"/  \x2763\\_/");
		break;
	default:
		break;
	}
}

PROGRAM
RenovedLoadScreen
(

)
{

	INT32 x = (((GET_MAX_COL)-25) / 2);
	INT32 y = (((GET_MAX_ROWS)-10) / 2);
	INT8 eframe = 0;
	INT8 brg = 3;
	INT16 restfrms = 30;

	while (true) {
		ClearScreen();

		if (
			brg > -1
			) {
			DrawLogo(x, y, brg);
		}

		DrawLoadingCircle(x + 11, y + 10,eframe);

		DrawScreen();

		if (restfrms < 1) {
			break;
		}

		if (
			restfrms > 4
			) {
			eframe++;
		}
		else {
			brg--;
		}
		restfrms--;

		if (
			eframe > 7
			)
		{
			eframe = 0;
		}
		gBS->Stall(150000);
	}
	gBS->Stall(1000000);
	ClearScreen();
}

PROGRAM
BootSettings
(
)
{

	INT32 x = (((GET_MAX_COL)-25) / 2);
	INT32 y = (((GET_MAX_ROWS)-6) / 2);

	INT8 tab = 0;
	INT8 page = 0;

	while (true)
	{
		ClearScreenCu();

		DrawLogo(x, y,3);
		
		if (
			page == 0
			)
		{
			gotoxy(x, y + 7);

			SetScreenAtribute(0, black);
			SetScreenAtribute(1, gray);

			if (
				tab == 0
				)
			{
				SwapColors();
			}
			printcu(L"Set to normal mode");

			SetScreenAtribute(0, black);
			SetScreenAtribute(1, gray);
			gotoxy(x, y + 8);
			if (
				tab == 1
				)
			{
				SwapColors();
			}
			printcu(L"Set to debug mode");

			SetScreenAtribute(0, black);
			SetScreenAtribute(1, gray);
			gotoxy(x, y + 9);
			if (
				tab == 2
				)
			{
				SwapColors();
			}
			printcu(L"Enter Setup");

			SetScreenAtribute(0, black);
			SetScreenAtribute(1, gray);
			gotoxy(x, y + 10);
			if (
				tab == 3
				)
			{
				SwapColors();
			}
			printcu(L"Continue");
		}

		SetScreenAtribute(1, black);
		DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Conio->atributes->BG);
		DrawScreen();

		EFI_INPUT_KEY Key;
		gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, 1);
		uefi_call_wrapper(gST->ConIn->ReadKeyStroke, 2, gST->ConIn, &Key);

		if (
			Key.ScanCode == SCAN_UP
			){
			tab--;
		}
		else if (
			Key.ScanCode == SCAN_DOWN
			) {
			tab++;
		}
	}
}

PROGRAM
DrawSplashBoot
(

)
{
	INT32 x = (((GET_MAX_COL) - 25) / 2);
	INT32 y = (((GET_MAX_ROWS) - 6) / 2);

	ClearScreen();

	DrawLogo(x, y, 3);

	DrawScreen();

	gotoxy(x, y + 7);

	SetScreenAtribute(0, bryellow);
	printcu(L"Enter");
	SetScreenAtribute(0, brblue);
	printcu(TranslateWorck(&SPLASH_OP1_TRANSL,languajecu));

	gotoxy(x, y + 8);

	SetScreenAtribute(0, bryellow);
	printcu(L"ESC");
	SetScreenAtribute(0, brblue);
	printcu(TranslateWorck(&SPLASH_OP2_TRANSL, languajecu));

	for (size_t i = 5; i > 0; i--)
	{
		EFI_INPUT_KEY Key;
		uefi_call_wrapper(gST->ConIn->ReadKeyStroke, 2, gST->ConIn, &Key);

		if (
			Key.ScanCode == SCAN_ESC
			)
		{

			gotoxy(x, y + 9);

			SetScreenAtribute(0, brcyan);
			printcu(L"Status");
			SetScreenAtribute(0, brgreen);
			printc(L". bye :)");

			gBS->Stall(1000000);
			gBS->Exit(globalimagehandle, EFI_SUCCESS, 0, L"");
		}
		else if (
			Key.UnicodeChar == CHAR_CARRIAGE_RETURN
			)
		{
			gotoxy(0, y + 10);
			return EFI_SUCCESS;
		}

		CHAR16 count[5];

		ValueToString(count, 0, i);

		gotoxy(x, y + 5);

		SetScreenAtribute(0, brblue);
		printcu(TranslateWorck(&SPLASH_TEXT1_TRANSL,languajecu));

		gotoxy((x + 25) - StrLen(count), y + 5);
		SetScreenAtribute(0, bryellow);
		printc(count);

		gBS->Stall(1000000);
	}
	gotoxy(0, y + 10);
}

/*
efi_main

Summary:
	...
*/
PROGRAM
efi_main
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	//
	// configure main variables
	//
	UINTN Event;
	INT16 option;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;

	//
	// set the settings
	//
	BootStage = 0;
	*languajecu = L"en";

	//
	// lib init
	// 
	
	// EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	globalimagehandle = ImageHandle;
	globalsystemtable = SystemTable;
#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif
	//PlaySound(400, 50); // Mi
	//PlaySound(600, 50); // Fa

	//
	// test
	// 
	
	// prepare the editor save
	*EditorProcess = NULL;

	PlaySound(0, 10);

	//
	// whatch dog
	// 
	
	// set the dog for that the dog dont f*** me
	Status = gBS->SetWatchdogTimer(0, 0, 0, NULL);

	//
	// screen
	//
	
	// prepare the gop
	EFI_PHYSICAL_ADDRESS FrameBufferBase = gop->Mode->FrameBufferBase;
	UINTN FrameBufferSize = gop->Mode->FrameBufferSize;

	// set the screen size
	SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &horizontalResolution, &verticalResolution);

	// get the gop
	Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&gop);
	
	if (EFI_ERROR(Status)) {
		Print(L"Error al obtener el protocolo de gráficos: %r\n", Status);
		return Status;
	}

	// set a gop mode
	UINTN Mode = 2; 

	Status = uefi_call_wrapper(gop->SetMode, 2, gop, Mode);
	if (EFI_ERROR(Status)) {
		Print(L"Error al establecer el modo de gráficos: %r\n", Status);
		return Status;
	}

	//
	//set the screen
	//
	
	// change to text mode
	ChangeToTextMode();

	// anti bugs
	PrintLineWithBackground(SystemTable, L"S-SUN anti display bugs text", 0, EFI_BLACK, EFI_BACKGROUND_LIGHTGRAY);

	// prepare the s++ consoleoutpud color
	consoleoutpudcurrentcolor = white;
	consoleoutpudcurrentcolorbg = black;

	// initialize my text mode protocol
	initializeMoonScreen();

	//
	// transition to kernel boot
	//

	BootStage = 1;
	CurrentDir = -1;

	// clear the screen
	SetScreenAtribute(1, black);
	ClearScreen();

	languajecu = LibGetVariable(L"Lang", &SmallVariables);

	// wait
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 100000);

	//
	// console settings
	//

	// clear the screen
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&gop);
	
	//
	// parse gop
	//

	INTN MaxColumn = (gop->Mode->Info->HorizontalResolution / 8) / Conio->atributes->size;
	MaxColumn = MaxColumn - 1;

	//
	// idk
	//

	// create the taskbar text
	StrCpy(taskbar, L"");
	for (size_t i = 0; i < MaxColumn + 1; i++)
	{
		SPrint(taskbar, sizeof(taskbar), L"%s%c", taskbar, L' ');
	}

	// SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

	//
	// visual boot
	//

	SetScreenAtribute(1, black);
	ClearScreen();

	DrawSplashBoot();

	//Print(SasmParseBinaryIns(SolveSasmLine(L"mov \'A\',1; 10")));
	 //FatalError(L"Hello World");

	//
	// go to the logs and init varius services
	//

	printc(L"S-SUN startup logs\n");
	printc(L"\nIf you found a problem with system this can help you to fix it\n\nSystem Startup Logs:\n\n");
	
	InitializeNetwork();

	// initialize the kernel
	KERNEL_INITIALIZE;

	//YNConfirmation(L"test");

	FindSoundCard(SystemTable);

	InitializeMems();

	SetScreenAtribute(0, brcyan);

	printc(L"\n\ninitializing the frameworck\n");
	InitializeFrameworck();

	SetScreenAtribute(0, brcyan);

	InitializeFileSystem();

	printc(L"\n\ncreating the themes\n");

	//
	// themes creation
	//
	
	// themes
	SCDefault = newCSCHEME(yellow, gray, black);
	SCNature = newCSCHEME(cyan, green, black);
	SCBoy = newCSCHEME(blue, cyan, black);
	SCGirl = newCSCHEME(brred, gray, black);
	SCBilly = newCSCHEME(darkcyan, brgreen, brblue);
	SCFire = newCSCHEME(brorange, orange, red);
	SCOcean = newCSCHEME(blue, brblue, darkblue);
	SCFuturistic = newCSCHEME(darkgray, brcyan, darkcyan);
	SCDesert = newCSCHEME(bryellow, yellow, black);
	SCReactor = newCSCHEME(darkteal, gray, darkgray);
	SCNight = newCSCHEME(darkblue, bryellow, black);
	SCSunDays = newCSCHEME(brteal, yellow, black);
	SCHolidays = newCSCHEME(white, green, brred);

	//
	// setting reserved
	//

	// prepare the instances
	MEM_FILE_INT* Kernel_Startup = Create_MEM_FILE_INT(L"Kernel_Initialized", 1);
	MEM_FILE_INT* Kernel_Instance = Create_MEM_FILE_INT(L"Kernel_Instance", 1);

	// gnu-efi debug
#if !(REALESE)
	MEM_FILE_INT* DEBUG = Create_MEM_FILE_INT(L"DEBUG GNU-EFI", 1);
#endif // DEBUG

	//
	// ready for the loading screen?
	//

	BootStage = 2;

	//
	// load desktop theme
	//

	CSCHEME* loadfrommem = LibGetVariable(L"Theme", &SmallVariables);

	if (loadfrommem != NULL) {
		Sceme = loadfrommem;
		PIXELCOL*
			buttonstextl
			=
			LibGetVariable(L"ThemeT", &SmallVariables)
			;
		Sceme->buttonstext = *buttonstextl;
	}
	else Sceme = newCSCHEME(yellow, gray, black);

	//
	// a keys for make a setting
	//

	uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
	if (Key.UnicodeChar == L'r') {
		// reset the fs to factory point
		InitializeFileSystem();

		// update the fs backup
		updatefilesystem();

		// reset for not bugs
		globalsystemtable->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);

	}
	else if (Key.ScanCode == SCAN_ESC) {
		EmergencyPrompt();
	}
	
	//
	// loading screen
	//
	
	// set the pre-loadscreen colors
	ClearScreen();
	SetScreenAtribute(1, black);
	SetScreenAtribute(0, white);

	//
	// execute some things
	//
	// execute the kernel.sys (that is internal s++ code)
	ExecuteScript(L"# ---------------------------------\n\n#\tS-SUN kernel\n\n# ---------------------------------\n\n\n\n# internal system file (kernel.sys)\n\n\n\n# ---------------------------------\n\n# POINTERS DEFINITION\n\n\n\nif def%VOID:EditMem VOID=0\n\n\n\n# ---------------------------------\n\n# KERNEL_MAIN\n\n\n\necho Initializing Kernel\n\n\n\n# the kernel main\n\ncall set_vars\n\ncall oem_start\n\ncall flush_vars\n\ncall os_start\n\n\n\n# section to set the vars\n\nsection\n\nset_vars\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_0=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_1=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_2=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_3=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_4=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_5=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_6=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_7=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_8=d%VOID\n\nif CMP&%KERNEL_INITIALIZED=1:EditMem KERNEL_RESERVED_9=d%VOID\n\nret\n\n\n\n# section to flush the vars\n\nsection\n\nflush_vars\n\nFlushMEM KERNEL_RESERVED_0\n\nFlushMEM KERNEL_RESERVED_1\n\nFlushMEM KERNEL_RESERVED_2\n\nFlushMEM KERNEL_RESERVED_3\n\nFlushMEM KERNEL_RESERVED_4\n\nFlushMEM KERNEL_RESERVED_5\n\nFlushMEM KERNEL_RESERVED_6\n\nFlushMEM KERNEL_RESERVED_7\n\nFlushMEM KERNEL_RESERVED_8\n\nFlushMEM KERNEL_RESERVED_9\n\nret\n\n\n\n# oem start section\n\nsection\n\noem_start\n\nEditMem S-SUN_Distributor=unfortunately no one :)\n\nret\n\n\n\n# section to the os takes the absolute control MUAHAHAHAHA >:)\n\nsection\n\nos_start\n\nend_script",ImageHandle,SystemTable);
	// get the system state

	// wait
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 500000);
	
	// loading screen
	initializeMoonScreen();
	SetScreenAtribute(0, white);

	//
	// test again the sound
	//

	PlaySound(700, 50);

	/*
	SetScreenAtribute(1, LND_BG_COLOR);
	ClearScreen();
	*/

	//
	// if you active legacy screen
	//

	// calculate the scren size
	globMaxRows = (verticalResolution / row_size) / Conio->atributes->size;
	globMaxColumns = (horizontalResolution / 8) / Conio->atributes->size;
	if (0) {

		//
		// screen setting
		//
		
		// draw the presentation by-creator
		gotoxy((globMaxColumns - StrLen(COMPANY_NAME)) / 2, globMaxRows - 1);
		printc(COMPANY_NAME);

		// draw the system name
		gotoxy((globMaxColumns - StrLen(OS_NAME)) / 2, globMaxRows / 2);
		printc(OS_NAME);

		// draw the instruccions
		gotoxy((globMaxColumns - StrLen(TranslateWorck(&BOOTINGUP_MSG_TRANS, languajecu))) / 2, (globMaxRows / 2) + 4);
		printc(TranslateWorck(&BOOTINGUP_MSG_TRANS, languajecu));

		// set the colors for the loading bar
		gotoxy((globMaxColumns - 30) / 2, (globMaxRows / 2) + 2);
		SetScreenAtribute(0, LND_LBAR_BG);

		// wait to give it elegance
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 500000);

		// draw the loading bar
		printc(L"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");

		//
		// go to the action
		//

		// again for other time of that reason
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 500000);
		gotoxy((globMaxColumns - 30) / 2, (globMaxRows / 2) + 2);
		// the animation
		for (INT8 i = 0; i < 30; i++)
		{
			// set the loading dots colors
			SetScreenAtribute(1, LND_BG_COLOR);
			SetScreenAtribute(0, LND_BAR_FILLED);
			// draw the dot
			printc(LND_BAR_FILL_CH);
			// wait for make it a animation
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);

			// press the f1 to skip Init.spp
			uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
			if (Key.ScanCode == SCAN_ESC) {
				break;
			}
		}
		SetScreenAtribute(1, black);
		ClearScreen();
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
	}
	else {
		RenovedLoadScreen();
	}

	// if not skiped initialize Init.spp
	/*
	if (Key.ScanCode != SCAN_ESC) {
		// ExecuteScript(CurrentFS->HEY_CURRENT_SESSION[3].Content, ImageHandle, SystemTable);
	}
	*/

	//
	// finally in the desktop YYAAAAYY
	//

	BootStage = 3;

	// initialize the desktop
	Desktop(ImageHandle, SystemTable);
	Free_MEM_FILE_INT(Kernel_Instance);
	return Status;
}

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++