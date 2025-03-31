// the resources used are the next:
/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2023 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */

// S-SUN by ErickCraftStudios but it use GNU-EFI and other more things that idk what are that
// but i port s-sun for make avaible for all
/*
Copyright (c) 1998  Intel Corporation
Todos los derechos reservados.

Copyright (c) 2019, TianoCore and contributors. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions de código fuente deben conservar el aviso de copyright anterior, esta lista de condiciones y el siguiente descargo de responsabilidad.
2. Las redistribuciones en forma binaria deben reproducir el aviso de copyright anterior, esta lista de condiciones y el siguiente descargo de responsabilidad en la documentación y/u otros materiales proporcionados con la distribución.

Sujeto a los términos y condiciones de esta licencia, cada titular de derechos de autor y colaborador concede por la presente a aquellos que reciben derechos bajo esta licencia una licencia perpetua, mundial, no exclusiva, sin cargo, libre de regalías, irrevocable (excepto por incumplimiento de las condiciones de esta licencia) de patente para fabricar, hacer fabricar, usar, ofrecer a la venta, vender, importar y transferir de otro modo este software, donde dicha licencia se aplica únicamente a las reclamaciones de patentes que:
(a) sus Contribuciones (los derechos de autor con licencia de los titulares de derechos de autor y las adiciones no protegibles por derechos de autor de los colaboradores, en forma de fuente o binaria) solas; o
(b) combinación de sus Contribuciones con la obra de autoría a la que se añadieron tales Contribuciones por dicho titular de derechos de autor o colaborador, si, en el momento en que se añadió la Contribución, dicha adición hace que tal combinación sea necesariamente infringida. La licencia de patente no se aplicará a ninguna otra combinación que incluya la Contribución.

DESCARGO DE RESPONSABILIDAD
AUNQUE ESTOY UTILIZANDO GNU-EFI Y QUEMU

	   ESPERO QUE CON ESTO NO ME HAGAN NADA y pueda hacer el sistema operativo
*/
// Seriously, please don't do anything to me.

// code edited by ErickCraftStudios for make a simple OS

/*
File Name:
	main.c

Abstract:
	the S-SUN OS function
*/
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include ".vs/msvc/kernelMK.h"

CHAR16* languajecu;

/*
erick : y si hago un siste...
comunidad c : NOOOO
PAM , S-SUN creado
*/
CSCHEME* Sceme;
PIXELCOL consoleoutpudcurrentcolor;

// the taskbar
CHAR16 taskbar[1024];

// my variable guid
EFI_GUID VariablesGuid = { 0x12345678, 0x1234, 0x1234, { 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90 } };
EFI_GUID SmallVariables = { 0x52375679, 0x1435, 0x1634, { 0xbb, 0xad, 0xaf, 0x22, 0x3a, 0x51, 0x18, 0x90 } };

CHAR16 EditorProcess[1024];

#define MAX_VARIABLES 100
string CurrentDir;

VOID
updatefilesystem
(
)
{
	
	EFI_STATUS Status;

	Status = RT->SetVariable(
		L"S-SUN_State",
		&VariablesGuid,
		EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
		sizeof(CurrentFS),
		&CurrentFS
	);
	if (EFI_ERROR(Status)) {
		printc(L"\nError saving the state: \n");
		string* e[100];
		StatusToString(e, Status);
		printc(e);
	}
}

VOID
UpdateLanguaje
(
)
{
	InitializeLib(globalsystemtable, globalsystemtable);

	CHAR16* langdata;
	langdata = languajecu;
	UINTN DataSize = sizeof(langdata);
	EFI_STATUS status;
	status = globalsystemtable->RuntimeServices->SetVariable(
		L"S-SUN_Languaje",
		&SmallVariables,
		EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
		DataSize,
		&langdata
	);
	string* ea[100];

	StatusToString(ea, status);
	printc(L"\nstatus on set the system languaje: \n");
	printc(ea);
	printc(L"\n");
}

/*
no se por que hago
function
e
(
);
en vez de function e();
*/
EFI_STATUS
WriteFile
(
	CHAR16* FileName,
	 CHAR16* Content
)
{
	// Verificar parámetros nulos
	if (FileName == NULL || Content == NULL) {
		Print(L"KERNEL_CALL -> -> Parámetro nulo: FileName o Content es NULL\n");
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		return EFI_INVALID_PARAMETER;
	}

	InitializeLib(globalimagehandle, globalsystemtable);
	EFI_FILE_PROTOCOL* Root;
	EFI_FILE_PROTOCOL* File;
	EFI_LOADED_IMAGE* LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

	EFI_STATUS Status;
	CHAR16 *msg[100];

	// Obtener la interfaz Loaded Image
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, globalimagehandle, &LoadedImageProtocol, (void**)&LoadedImage);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"Status getting the image protocol: %r", Status);
		pr_warn(msg);
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	// Obtener la interfaz de sistema de archivos
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, LoadedImage->DeviceHandle, &FileSystemProtocol, (void**)&FileSystem);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"error on get filesystem protocol: %r", Status);
		pr_warn(msg);
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	// Abrir la raíz del sistema de archivos
	Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"error opening the root: %r", Status);
		pr_warn(msg);
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	// Crear y abrir el archivo
	Status = uefi_call_wrapper(Root->Open, 5, Root, &File, FileName, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
	if (EFI_ERROR(Status)) {
		SPrint(msg, sizeof(msg), L"error creating/opening the file %s: %r", FileName, Status);
		pr_warn(msg);
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		return Status;
	}

	UINTN Size = StrLen(Content) * sizeof(CHAR16);
	// Escribir el contenido en el archivo
	Status = uefi_call_wrapper(File->Write, 3, File, &Size, Content);
	if (EFI_ERROR(Status)) {
		Print(L"KERNEL_CALL -> -> error writing to the file %s: %r\n", FileName, Status);
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
	}

	uefi_call_wrapper(File->Close, 1, File);
	return Status;
}

/*
EFI_STATUS CheckAndCreateFiles(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_STATUS Status;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SimpleFileSystem;
	EFI_FILE_PROTOCOL* Root;
	EFI_FILE_PROTOCOL* File;
	EFI_FILE_PROTOCOL* Fileins;
	CHAR16* InstallRegister = L"\\s-sunsys.register"; // Nombre del archivo a verificar
	CHAR16* FileName;
	CHAR16* DirName;

	InitializeLib(ImageHandle, SystemTable);

	// Obtener el dispositivo actual
	EFI_HANDLE DeviceHandle = KERNEL_CURRENT_DOMAIN;

	if (DeviceHandle == NULL) {
		Print(L"error : cannot obtain current device handle\n");
		uefi_call_wrapper(SystemTable->BootServices->Stall, 5, 500000);
		return EFI_DEVICE_ERROR;
	}

	// Obtener el protocolo Simple File System
	Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3, DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFileSystem);
	if (EFI_ERROR(Status)) {
		Print(L"error : cannot get the file protocol, Status: %r\n", Status);
		uefi_call_wrapper(SystemTable->BootServices->Stall, 5, 500000);
		return Status;
	}

	// Abrir el volumen raíz
	Status = uefi_call_wrapper(SimpleFileSystem->OpenVolume, 2, SimpleFileSystem, &Root);
	if (EFI_ERROR(Status)) {
		Print(L"Error: cannot get main folder of device: %r\n", Status);
		uefi_call_wrapper(SystemTable->BootServices->Stall, 5, 500000);
		return Status;
	}

	// Intentar abrir el archivo
	Status = uefi_call_wrapper(Root->Open, 5, Root, &File, InstallRegister, EFI_FILE_MODE_READ, 0);
	if (Status == EFI_NOT_FOUND) {
		Print(L"Installing S-SUN this take a seconds\n");
		FileName = L"\\S-SUN";
		// Crear carpeta del sistema
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 500000);
		Print(L"try to create the system folder\n");
		Status = uefi_call_wrapper(Root->Open, 5, Root, &Fileins, FileName, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
		Print(L"status verified\n");
		if (!EFI_ERROR(Status)) {
			if (Status == EFI_NOT_FOUND) {
				Print(L"system folder created\n");
				uefi_call_wrapper(Fileins->Close, 1, Fileins);
			}
		}

		FileName = L"\\S-SUN\\User";
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 500000);
		Print(L"try to create the User folder\n");
		Status = uefi_call_wrapper(Root->Open, 5, Root, &Fileins, FileName, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
-		Print(L"status verified\n");
		if (!EFI_ERROR(Status)) {
			if (Status == EFI_NOT_FOUND) {
				Print(L"User folder created\n");
				uefi_call_wrapper(Fileins->Close, 1, Fileins);
			}
		}

		// Crear registro
		FileName = L"\\s-sunsys.register";
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 500000);
		Print(L"try to create the system installation register\n");
		Status = uefi_call_wrapper(Root->Open, 5, Root, &Fileins, FileName, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
		Print(L"status verified\n");
		if (!EFI_ERROR(Status)) {
				CHAR16* Content = L"# you can edit this file but if you remove it s-sun will be install agin when you start";
				UINTN Size = StrLen(Content) * sizeof(CHAR16);
				WriteFile(Root, FileName, Content);

				Print(L"system file for only install S-SUN the first time is created!\n");
		}
	}
	else if (!EFI_ERROR(Status)) {
		Print(L"Loanding S-SUN\n");
		uefi_call_wrapper(File->Close, 1, File);
	}
	else {
		Print(L"Kernel Stop the system cant open the installation register\n");
	}

	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 500000);

	// Cerrar el directorio raíz
	uefi_call_wrapper(Root->Close, 1, Root);

	return EFI_SUCCESS;
}
*/

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

typedef struct {
	CHAR16* variable;
	CHAR16* value;
} VariableEntry;

VariableEntry variables[MAX_VARIABLES];
UINTN variable_count = 0;

VOID
SetVariable
(
	CHAR16* variable,
	 CHAR16* value
)
{
	for (UINTN i = 0; i < variable_count; i++) {
		if (StrCmp(variables[i].variable, variable) == 0) {
			variables[i].value = value;
			return;
		}
	}
	variables[variable_count].variable = variable;
	variables[variable_count].value = value;
	variable_count++;
}

CHAR16*
GetVariable
(
	CHAR16* variable
)
{
	for (UINTN i = 0; i < variable_count; i++) {
		if (StrCmp(variables[i].variable, variable) == 0) {
			return variables[i].value;
		}
	}
	return NULL;
}

CHAR16*
StrToken
(
	CHAR16* str,
	 const CHAR16* delim
);

EFI_STATUS
SSUNSCREENLOGOUT
(
	EFI_HANDLE ImageHandle,
	 EFI_SYSTEM_TABLE* SystemTable
)
{
	initializeMoonScreen();
	SetScreenAtribute(1, blue);
	ClearScreen();
	for (size_t i = 0; i < CurrentFS.FilesCount; i++) {
		if (CurrentFS.HEY_CURRENT_SESSION[i].Name != NULL) {
			printc(L"File: ");
			printc(CurrentFS.HEY_CURRENT_SESSION[i].Name);
			printc(L"\n");
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		}
	}
	ClearScreen();
	printc(L"\nS-SUN BootLoader v0.2\n\n");
	printc(L"\nS-SUN Operating System\nmaded By ErickCraftStudios\nPlease Wait while S-SUN logins out\n\n");
	for (size_t i = 0; i < 30; i++)
	{
		SetScreenAtribute(0, white);
		SetScreenAtribute(1, blue);
		printc(L".");
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
	}
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
}

CHAR16 
ExecuteCommand
(
	CHAR16* buffer,
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable,
	CHAR16 mode
)
;

// Función para dividir una cadena CHAR16 en líneas
CHAR16** 
SplitLines
(
	CHAR16* text, 
	UINTN* line_count
) 
{
	static CHAR16* lines[100];
	*line_count = 0;
	CHAR16* token = StrToken(text, L"\n");
	while (token != NULL && *line_count < 100) {
		lines[(*line_count)++] = token;
		token = StrToken(NULL, L"\n");
	}
	return lines;
}

// Implementación de StrToken
CHAR16* 
StrToken
(
	CHAR16* str,
	const CHAR16* delim
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
	CHAR16* text, 
	UINTN line, 
	UINTN COLOR, 
	UINTN BG
) 
{
	UINTN Column, MaxColumns;
	CHAR16* lineBuffer;
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
	CHAR16* text,
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

VOID
RepeatTimes
(
	IN CONST CHAR16* text,
	IN UINTN times,
	OUT CHAR16* buffer
)
{
	InitializeLib(globalimagehandle, globalsystemtable);

	if (text == NULL || times == 0) {
		return NULL; // Caso especial: entrada inválida
	}

	// Calcula la longitud de la cadena
	UINTN text_length = StrLen(text);
	UINTN total_length = text_length * times;

	// Asigna memoria suficiente para la cadena resultante (incluye terminador nulo)
	CHAR16 result[2048];

	// Construir la cadena resultante
	for (UINTN i = 0; i < times; i++) {
		if (text == L" ") {
			SPrint(result, sizeof(result), L"%s ", result);
		}
		else
		{
			SPrint(result, sizeof(result), L"%s%s", result, text);
		}
	}

	// SPrint(result, sizeof(result), L"%s\0", result);

	*buffer = result; // Retorna el puntero a la cadena dinámica
}

EFI_STATUS
DrawCursor
(
	EFI_SYSTEM_TABLE* SystemTable, 
	CHAR16 CursorChar
) 
{
	UINTN Column, Row;

	// Obtener la posición actual del cursor
	Column = SystemTable->ConOut->Mode->CursorColumn;
	Row = SystemTable->ConOut->Mode->CursorRow;

	// Guardar la posición actual del cursor
	UINTN OriginalColumn = Column;
	UINTN OriginalRow = Row;

	// Establecer el color del fondo y del texto (gris para el fondo)
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);

	// Dibujar el carácter en la posición actual del cursor
	SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, Column, Row);
	CHAR16 Str[2] = { CursorChar, L'\0' };
	SystemTable->ConOut->OutputString(SystemTable->ConOut, Str);

	// Restaurar el color original del texto y el fondo
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLUE);

	// Restaurar la posición original del cursor
	SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, OriginalColumn, OriginalRow);

	return EFI_SUCCESS;
}

// Función para dibujar texto utilizando la consola UEFI
EFI_STATUS 
DrawText
(
	EFI_SYSTEM_TABLE* SystemTable, 
	CHAR16* Text, 
	UINTN X, 
	UINTN Y
) 
{
	SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, X, Y);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, Text);
	return EFI_SUCCESS;
}

EFI_STATUS guimode
(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable
)
{
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput;
	EFI_SIMPLE_POINTER_PROTOCOL* Mouse;
	EFI_INPUT_KEY Key;
	EFI_SIMPLE_POINTER_STATE MouseState;
	UINTN Event;

	InitializeLib(ImageHandle, SystemTable);

	// Obtener la interfaz de gráficos
	Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&GraphicsOutput);
	if (EFI_ERROR(Status)) {
		Print(L"Error al obtener el protocolo de gráficos: %r\n", Status);
		return Status;
	}

	// Establecer un modo de gráficos específico
	UINTN Mode = 2; // Cambia esto según el modo disponible que desees utilizar
	Status = uefi_call_wrapper(GraphicsOutput->SetMode, 2, GraphicsOutput, Mode);
	if (EFI_ERROR(Status)) {
		Print(L"Error al establecer el modo de gráficos: %r\n", Status);
		return Status;
	}

	// Inicializar posición del cursor
	UINTN CursorX = 0;
	UINTN CursorY = 0;
	UINTN CursorSize = 10; // Tamaño del cursor
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL CursorColor = { 255, 255, 255, 0 }; // Blanco

	// Bucle principal para mover el cursor
	while (TRUE) {
		Status = SystemTable->BootServices->LocateProtocol(
			&SimplePointerProtocol, // GUID del protocolo de mouse
			NULL,                           // Opcional, no necesitas un registro
			(void**)&Mouse          // Dirección para almacenar el puntero al protocolo
		);

		Mouse->GetState(Mouse, &MouseState);

		EFI_GRAPHICS_OUTPUT_BLT_PIXEL BackgroundColor = { 255, 100, 100, 0 }; // Azul
		Status = DrawRectangle(GraphicsOutput, 0, 0, GraphicsOutput->Mode->Info->HorizontalResolution, GraphicsOutput->Mode->Info->VerticalResolution, BackgroundColor);
		if (EFI_ERROR(Status)) {
			return Status;
		}

		// Dibujar barra de tareas en la parte inferior de la pantalla
		EFI_GRAPHICS_OUTPUT_BLT_PIXEL TaskbarColor = { 243, 187, 131, 0 };
		EFI_GRAPHICS_OUTPUT_BLT_PIXEL TaskbarColor2 = { 253, 197, 141, 0 }; 

		UINT32 TaskbarHeight = 12;
		DrawRectangle(GraphicsOutput, 0, GraphicsOutput->Mode->Info->VerticalResolution - TaskbarHeight, GraphicsOutput->Mode->Info->HorizontalResolution, TaskbarHeight, TaskbarColor);
		DrawRectangle(GraphicsOutput, 0, 0, 4, 4, white);
		DrawRectangle(GraphicsOutput, 0, GraphicsOutput->Mode->Info->VerticalResolution - TaskbarHeight - 3, GraphicsOutput->Mode->Info->HorizontalResolution, 3, TaskbarColor2);
		
		SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
		Print(L"%d", MouseState.RelativeMovementX);


		CursorX += MouseState.RelativeMovementX;
		CursorY += MouseState.RelativeMovementY;
	}

	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
	return EFI_SUCCESS;
}

EFI_STATUS 
ssun_main
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable,
	CHAR16 mode
);

EFI_STATUS 
editor
(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable, 
	string* textt
);

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
		printc(SystemTable, L"S-SUN system manager");

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

EFI_FILE_PROTOCOL* CurrentDirectory = NULL;

EFI_STATUS 
ListStorageDevices
(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable
)
{
	EFI_STATUS Status;
	EFI_HANDLE* HandleBuffer;
	UINTN HandleCount;
	UINTN Index;

	// Obtener todos los controladores de sistema de archivos
	Status = uefi_call_wrapper(BS->LocateHandleBuffer, 5, ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &HandleCount, &HandleBuffer);
	if (EFI_ERROR(Status)) {
		return Status;
	}

	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);
	Print(L"\nMap\n");
	for (Index = 0; Index < HandleCount; Index++) {
		EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
		Status = uefi_call_wrapper(BS->HandleProtocol, 3, HandleBuffer[Index], &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);
		if (!EFI_ERROR(Status)) {
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_MAGENTA | EFI_BACKGROUND_BLACK);
			Print(L"STORAGE ");
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
			Print(L"%d %d\n",Index, HandleBuffer[Index]);
		}
		Print(L"\n");
	}

	FreePool(HandleBuffer);
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

EFI_STATUS 
ChangeDirectory
(
	CHAR16* DirName, 
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable
) 
{
	EFI_FILE_PROTOCOL* NewDirectory;
	EFI_STATUS Status;

	Status = uefi_call_wrapper(CurrentDirectory->Open, 5, CurrentDirectory, &NewDirectory, DirName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
	if (EFI_ERROR(Status)) {
		Print(L"Error on create dir", Status);
		return Status;
	}

	uefi_call_wrapper(CurrentDirectory->Close, 1, CurrentDirectory);
	CurrentDirectory = NewDirectory;

	return EFI_SUCCESS;
}

EFI_STATUS 
ListDirectories
(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable
) 
{
	EFI_FILE_INFO* FileInfo;
	EFI_STATUS Status;
	UINTN BufferSize = SIZE_OF_EFI_FILE_INFO + 256;
	CHAR8 Buffer[SIZE_OF_EFI_FILE_INFO + 256];

	while (TRUE) {
		BufferSize = SIZE_OF_EFI_FILE_INFO + 256;
		Status = uefi_call_wrapper(CurrentDirectory->Read, 3, CurrentDirectory, &BufferSize, Buffer);

		if (EFI_ERROR(Status) || BufferSize == 0) {
			break;
		}

		FileInfo = (EFI_FILE_INFO*)Buffer;
		Print(L"%s\n", FileInfo->FileName);
	}

	return EFI_SUCCESS;
}

CHAR16* 
ProcessText
(
	CHAR16* buffer
) 
{
	static CHAR16 processed_text[1024];
	UINTN Index = 0;
	UINTN i = 0;

	while (buffer[i] != L'\0') {
		if (buffer[i] == L'%' && buffer[i + 1] == L'[') {
			CHAR16 variable[100];
			UINTN var_index = 0;
			i += 2; // Saltar %[

			// Extraer el nombre de la variable
			while (buffer[i] != L']' && buffer[i] != L'\0') {
				variable[var_index++] = buffer[i++];
			}
			variable[var_index] = L'\0';
			if (buffer[i] == L']') {
				i++; // Saltar ]
			}

			// Obtener el valor de la variable
			CHAR16* value = GetVariable(variable);
			if (value != NULL) {
				// Copiar el valor de la variable al texto procesado
				while (*value != L'\0') {
					processed_text[Index++] = *value++;
				}
			}
			else {
				// Si no se encuentra la variable, copiar el texto tal cual
				processed_text[Index++] = L'%';
				processed_text[Index++] = L'[';
				for (UINTN j = 0; j < var_index; j++) {
					processed_text[Index++] = variable[j];
				}
				processed_text[Index++] = L']';
			}
		}
		else {
			// Copiar el texto tal cual si no se encuentra una variable
			processed_text[Index++] = buffer[i++];
		}
	}

	// Asegurarse de que el texto procesado esté terminado en nulo
	processed_text[Index] = L'\0';
	return processed_text;
}

CHAR16* 
SPP_SYNTAX
(
	CHAR16* buffer
) 
{
	if (StartsWish(buffer, L"%")) {
		CHAR16* processmemfile = buffer + 1;
		return Read_MEM_FILE_STRING(processmemfile);
	}
	return buffer;
}

/*
S-SUN editor

Summary:
	the S-SUN editor is a text editor for create documments, programs and texts
*/
EFI_STATUS 
editor
(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable, 
	CHAR16* textt
) 
{
	// if the text is null dont start
	if (textt == NULL) return 0;

	// buffers of editor
	CHAR16 text[1024] = L""; // Definir un tamaño suficiente para el búfer de entrada
	CHAR16 save_text[1024] = L""; // Búfer para guardar el texto

	// the index
	UINTN Index = StrLen(textt);
	
	// the input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	// screen size
	UINTN MaxColumns, MaxRows;

	// the survival guide alternate
	bool SurvivalGuideAlt = 0;

	// the scroll of the editor
	INTN EditorScroll;

	EditorScroll = 0;

	// the instance of editor
	MEM_FILE_INT* EDITOR_INSTANCE = Create_MEM_FILE_INT(L"EDITOR_INSTANCE", 1);

	// load the file or not load the file that is the question
	if (*EditorProcess == NULL) {
		StrCpy(&text, textt);
	}
	else
	{
		StrCpy(&text, EditorProcess);
		Index = StrLen(EditorProcess);
	}
	INTN scrolla;

	// editor main loop
	while (TRUE) {
		// set to grapichel mode
		ChangeToGrapichalMode();

		// parse the screen size
		MaxColumns = (gop->Mode->Info->HorizontalResolution / 8) / Conio->atributes->size;
		MaxColumns = MaxColumns - 1;
		MaxRows = (gop->Mode->Info->VerticalResolution / 12) / Conio->atributes->size;

		// set the atributes
		SetScreenAtribute(0, gray);
		SetScreenAtribute(1, black);
		ClearScreen();

		// reset the cursor
		gotoxy(0, 0);
		SetScreenAtribute(1, gray);
		SetScreenAtribute(0, black);

		// print the upper menu without update the screen
		printcu(taskbar);
		gotoxy(0, 0);
		printcu(TranslateWorck(&SSUNEDITOR_UPPER_BAR_TRANSL, languajecu));

		// set the space
		SetScreenAtribute(0, gray);
		SetScreenAtribute(1, black);
		printcu(L"\n\n\n");

		// print the footer bar
		gotoxy(0, MaxRows - 1);
		SetScreenAtribute(1, gray);
		SetScreenAtribute(0, black);
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
						SetScreenAtribute(0, brred);
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

			return EFI_SUCCESS;
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
			StrCpy(save_text, text);
			UINTN line_count = 0;
			ClearScreen();
			SetScreenAtribute(0, gray);
			SetScreenAtribute(1, black);
			CHAR16** lines = SplitLines(text, &line_count);
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
			SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
			for (UINTN i = 0; i < line_count; i++) {
				ExecuteCommand(lines[i], ImageHandle, SystemTable, L"DEV");
			}
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
			StrCpy(text, save_text);
		}
		else {
			if (Key.UnicodeChar != 0) {
				text[Index++] = Key.UnicodeChar;
				EditorScroll = 0;
			}
		}

		text[Index] = L'\0';
		WHILESYSTEMRUNNING();
	}

	*EditorProcess = NULL;

	ChangeToTextMode();

	ExecuteCommand(L"ls", ImageHandle,SystemTable,L"Norm");
	SetScreenAtribute(0, white);
	printc(L"\nFileName:\n");
	SetScreenAtribute(0, gray);
	FileName* filepath[100];
	ReadLineSerius(&filepath);

	printc(filepath);
	bool filefound;
	int filepos = 0;

	filefound = false;
	printc(L"verified file\n");

	if (filepath == NULL) {
		return EFI_SUCCESS;
	}

	for (UINTN i = 0; i < CurrentFS.FilesCount; i++)
	{
		if (StrCmp(filepath, CurrentFS.HEY_CURRENT_SESSION[i].Name) == 0) {
			filefound = true;
			filepos = i;
			break;
		}
	}

	if (filefound == true) {
		printc(L"file edited\n");
		StrCpy(CurrentFS.HEY_CURRENT_SESSION[filepos].Content, text);
	}
	else {
		printc(L"creating file...\n");
		printc(L"coping name\n");
		CurrentFS.HEY_CURRENT_SESSION[CurrentFS.FilesCount + 1].Name =filepath;
		printc(L"coping path\n");
		CurrentFS.HEY_CURRENT_SESSION[CurrentFS.FilesCount + 1].path = L"\\";
		printc(L"coping text\n");
		StrCpy(&CurrentFS.HEY_CURRENT_SESSION[CurrentFS.FilesCount + 1].Content , text);
		printc(L"coping direction\n");
		CurrentFS.HEY_CURRENT_SESSION[CurrentFS.FilesCount + 1].Direction = CurrentFS.FilesCount + 1;
		printc(L"file created\n");
		CurrentFS.FilesCount++;
	}

	Free_MEM_FILE_INT(EDITOR_INSTANCE);

	printc(L"saving changes...\n");
	updatefilesystem();

	return EFI_SUCCESS;
}

/*
ExecuteCommand

Summary:
	Process the command of a S++ instruction for be executed
*/
string
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

	InitializeLib(ImageHandle,SystemTable);
	if (StrnCmp(buffer, L"echo ", 5) == 0) {
		CHAR16* msgtoecho = buffer + 5;
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
		SetScreenAtribute(0, consoleoutpudcurrentcolor);

		printc(L"\n");
		printc(SPP_SYNTAX(msgtoecho));
	}
	if (StrnCmp(buffer, L"writel ", 7) == 0) {
		CHAR16* msgtoecho = buffer + 7;
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
		SetScreenAtribute(0, consoleoutpudcurrentcolor);

		printc(SPP_SYNTAX(msgtoecho));
	}
	else if (StrCmp(buffer, L"Reset /all") == 0) {
		InitializeFileSystem();

		updatefilesystem();
	}
	else if (StrCmp(buffer, L"wait") == 0)
	{
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 500000); \
	}
	else if (StrnCmp(buffer, L"throw ", 6) == 0) {
		CHAR16* reason = buffer + 6;
		THROW_ERROR(SPP_SYNTAX(reason));
	}
	else if (StrnCmp(buffer, L"edit ", 5) == 0) {
		CHAR16* file = buffer + 5;

		editor(ImageHandle, SystemTable, CurrentFS.HEY_CURRENT_SESSION[Atoi(file)].Content);
	}
	else if (StrnCmp(buffer, L"if ", 3) == 0)
	{

	} 
	else if (StrCmp(buffer, L"LSMEM") == 0) {
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK);
		SetScreenAtribute(0, brblue);
		printc(L"\nList of MEM files:\n\n");
		for (UINTN i = 0; i < MemFileCountString; i++) {
			SetScreenAtribute(0, brblue);
			printc(L"MEM_STRING* ");
			SetScreenAtribute(0, cyan);
			printc(MemFilesString[i]->NAME);
			SetScreenAtribute(0, white);
			printc(L" = ");
			SetScreenAtribute(0, brgreen);
			printc(MemFilesString[i]->VALUE);
			printc(L"\n");
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		}
		
		for (UINTN i = 0; i < MemFileCountInt; i++) {
			SetScreenAtribute(0, brblue);
			printc(L"MEM_STRING* ");
			SetScreenAtribute(0, cyan);
			printc(MemFilesInt[i]->NAME);
			SetScreenAtribute(0, white);
			printc(L" = ");
			string value[100];

			SPrint(value, sizeof(value), L"%d ", MemFilesInt[i]->VALUE);
			SetScreenAtribute(0, brgreen);
			printc(value);
			printc(L"\n");
			uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);
		}
	}
	else if (StrnCmp(buffer, L"GetMem ", 7) == 0) {
		CHAR16* MemName = buffer + 7;
		bool found = false;
		SetScreenAtribute(0, consoleoutpudcurrentcolor);
		for (UINTN i = 0; i < MemFileCountString; i++) {
			if (StrCmp(MemFilesString[i]->NAME, MemName) == 0) {
				printc(MemFilesString[i]->VALUE);
				found = true;
				break;
			}
		}
		if (!found) {
			SetScreenAtribute(0, consoleoutpudcurrentcolor);
			printc(L"\n");
			printc(L"MEM not founded");
		}
	}
	else if (StrCmp(buffer, L"FlushMEM ", 9) == 0) {
		CHAR16* command = buffer + 9;
		Flush_MEM_FILE_STRING(command);
	}
	else if (StrnCmp(buffer, L"drw ", 4) == 0) {
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
	else if (StrnCmp(buffer, L"color ", 6) == 0) {
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
			consoleoutpudcurrentcolor = green;
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
	else if (StrnCmp(buffer, L"mv drw x ", 9) == 0) {
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
	else if (StrnCmp(buffer, L"mv drw y ", 9) == 0) {
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
	else if (StrnCmp(buffer, L"mv drw sx ", 10) == 0) {
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
	else if (StrnCmp(buffer, L"mv drw sy ", 10) == 0) {
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
	else if (StrnCmp(buffer, L"EditMem ", 8) == 0) {
		CHAR16* command = buffer + 8;
		CHAR16* equals_pos = StrStr(command, L"=");
		if (equals_pos != NULL) {
			*equals_pos = L'\0';
			CHAR16* MemName = command;
			CHAR16* NewValue = equals_pos + 1;

			EFI_STATUS Status = Edit_MEM_FILE_STRING(MemName, NewValue);
			if (Status == EFI_NOT_FOUND) {
				Status = Edit_MEM_FILE_INT(MemName, Atoi(NewValue)); // Convertir a int si no se encuentra como string
			}
			if (Status == EFI_NOT_FOUND) {
				MEM_FILE_STRING* NEW_MEM_FILE = Create_MEM_FILE_STRING(MemName,NewValue);
			}
		}
	}
	else if (StrnCmp(buffer, L"ReadLine ", 9) == 0) {
		CHAR16* command = buffer + 9;
		if (command != NULL) {
			CHAR16* MemName = command;

			CHAR16* readedLine;
			SetScreenAtribute(0, consoleoutpudcurrentcolor);

			ReadLineSerius(&readedLine);
			EFI_STATUS Status = Edit_MEM_FILE_STRING(MemName, readedLine);
			if (Status == EFI_NOT_FOUND) {
				MEM_FILE_STRING* NEW_MEM_FILE = Create_MEM_FILE_STRING(MemName, readedLine);
			}
		}
	}
	else if (StrnCmp(buffer, L"Shark P ", 8) == 0) {
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
	else if (StrnCmp(buffer, L"Shark I ", 8) == 0) {
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
	else if (buffer[0] == L'#') {
		CHAR16* equals_pos = StrStr(buffer, L"=");
		if (equals_pos != NULL) {
			*equals_pos = L'\0';
			CHAR16* variable = buffer + 1;
			CHAR16* value = equals_pos + 1;
			SetVariable(variable, value);
			return L"Variable definida";
		}
	}
	else if (StrCmp(buffer, L"showvars") == 0) {
		Print(L"\n");
		for (UINTN i = 0; i < variable_count; i++) {
			Print(L"%s = %s\n", variables[i].variable, variables[i].value);
		}
		return L"Variables mostradas";
	}
	else if (StrCmp(buffer, L"catch", 6) == 0) {
		_KERNEL_CATCH_THROW_ = 1;
	}
	else if (StrCmp(buffer, L"end catch", 6) == 0) {
		_KERNEL_CATCH_THROW_ = 0;
	}
	else if (StrnCmp(buffer, L"md ", 3) == 0) {
		if (StrCmp(mode, L"DEV") == 0) {
			CHAR16* DirName = ProcessText(buffer + 3);
			CreateANewDirFUNCTION(DirName, ImageHandle, SystemTable);
		}
	}
	else if (StrnCmp(buffer, L"MsgBox ", 7) == 0) {
			CHAR16* Text = ProcessText(buffer + 7);
			CHAR16* FullMessage[100];
			SPrint(FullMessage, sizeof(FullMessage), L"%s", Text);
			DRAW_TEXT_DIALOG(ImageHandle, SystemTable, FullMessage, EFI_WHITE, EFI_BACKGROUND_CYAN);
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	else if (StrCmp(buffer, L"SyMgr") == 0) {
			SystemManager(ImageHandle, SystemTable);
	}
	else if (StrCmp(buffer, L"GUI") == 0) {
			guimode(ImageHandle,SystemTable);
	}
	else if (StrnCmp(buffer, L"cd ", 3) == 0) {
	}	
	else if (StrCmp(buffer, L"ls") == 0) {
		printc(L"\n");
		for (size_t i = 0; i < CurrentFS.FilesCount + 1; i++)
		{
			CHAR16* FILENAME[20];
			SPrint(FILENAME, sizeof(FILENAME) * 20, L"%s%s", CurrentFS.HEY_CURRENT_SESSION[i].path , CurrentFS.HEY_CURRENT_SESSION[i].Name);

			if (StartsWish(FILENAME, CurrentDir)) {
				if (CurrentFS.HEY_CURRENT_SESSION[i].Content == FOLDER_CONTENT) {
					SetScreenAtribute(0, green);
				}
				else
				{
					SetScreenAtribute(0, blue);
				}

				printc(CurrentFS.HEY_CURRENT_SESSION[i].Name);
				SetScreenAtribute(0, brblue);

				CHAR16 str[20];
				printc(L" Direction: ");
				SPrint(str, sizeof(str) * 20, L"%d", CurrentFS.HEY_CURRENT_SESSION[i].Direction);
				printc(str);
				printc(L"\n");
			}
			
		}

	}
	else if (StrCmp(buffer, L"cls") == 0) {
		ClearScreen();
	}
	else if (StrCmp(buffer, L"exit") == 0) {
		return L"sistema , tu trabajo ha terminado";
	}
	else if (StrCmp(buffer, L"Desktop") == 0) {
	}
	else if (StrnCmp(buffer, L"#",1) == 0) {}
	return L"sistema , si puedes seguir";
}

/*
ExecuteScript

Summary:
	executes a s++ script
*/
VOID
ExecuteScript
(
	CHAR16* buffer,
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
			// Dividir el texto en líneas y ejecutar cada línea
			UINTN line_count = 0;
			ClearScreen();
			SetScreenAtribute(0, gray);
			SetScreenAtribute(1, black);
			CHAR16** lines = SplitLines(buffer, &line_count);
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
			SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
			for (UINTN i = 0; i < line_count; i++) {
				ExecuteCommand(lines[i], ImageHandle, SystemTable, L"DEV");
			}
}

/*
Console

Summary:
	the console of S-SUN
*/
EFI_STATUS 
Console
(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable, 
	CHAR16 mode
) 
{
	UINTN Event;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

	CHAR16 Buffer[512];
	UINTN Index = 0;
	// Obtener la interfaz de sistema de archivos
	Status = uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&FileSystem);
	// Abrir el volumen del sistema de archivos

	// Continuar con el resto de la inicialización
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);

	/*t
	 * In addition to the standard %-based flags, Print() supports the following:
	 *   %N
	 Set output attribute to normal
	 *   %H       Set output attribute to highlight
	 *   %E       Set output attribute to error
	 *   %r       Human readable version of a status code
	 */
	MEM_FILE_INT* CONSOLEINSTANCE = Create_MEM_FILE_INT(L"CONSOLEINSTANCE", 1);

	SetScreenAtribute(1, black);
	ClearScreen();
	gotoxy(0,0);
	uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_MAGENTA | EFI_BACKGROUND_BLACK);
	CHAR16 handle_action_huh = ExecuteCommand(L"cls", ImageHandle, SystemTable, mode);
	SetScreenAtribute(0, white);
	printc(L"\nS-SUN Console\n");
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_CYAN | EFI_BACKGROUND_BLACK);
	SetScreenAtribute(0, cyan);
	printc(L"S-SUN> ");
	SetScreenAtribute(0, white);
	while (TRUE) {
		WHILESYSTEMRUNNING(
			ChangeToTextMode();
			SystemTable->ConOut->EnableCursor(SystemTable->ConOut, TRUE);
		uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
		// Variables para almacenar la posición del cursor
		UINTN CursorColumn;
		UINTN CursorRow;

		// Obtener la posición actual del cursor
		CursorColumn = SystemTable->ConOut->Mode->CursorColumn;
		CursorRow = SystemTable->ConOut->Mode->CursorRow;
		if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
			Buffer[Index] = L'\0';
			CHAR16 handle_action_huh = ExecuteCommand(Buffer, ImageHandle, SystemTable, mode);
			Index = 0; // Reiniciar el índice para la siguiente entrada
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_CYAN | EFI_BACKGROUND_BLACK);
			if (StrCmp(Buffer, L"exit") == 0)
			{
				return EFI_SUCCESS;
			}
			SetScreenAtribute(0, cyan);
			printc(L"\nS-SUN> ");
			gotoxy(7, cursory);
			SetScreenAtribute(0, white);
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

		}
		else if (Key.ScanCode == SCAN_UP) {
			gotoxy(cursorx, cursory - 1);
		}
		else if (Key.ScanCode == SCAN_DOWN) {
			gotoxy(cursorx, cursory + 1);
		}
		else if (Key.ScanCode == SCAN_ESC)
		{
			return EFI_SUCCESS;
		}
		else if (Key.UnicodeChar == CHAR_BACKSPACE) {
			gotoxy(cursorx - 1, cursory);
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, CursorColumn - 1, CursorRow);
			printc(L" ");
			gotoxy(cursorx - 1, cursory);
			SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, CursorColumn - 1, CursorRow);
			Buffer[--Index] = L'\0';
		}
		else if (Key.UnicodeChar != 0) {
			Buffer[Index++] = Key.UnicodeChar;
			string a[100];
			SPrint(a, sizeof(a), L"%c", Key.UnicodeChar);
			SetScreenAtribute(0, white);
			printc(a);
		}
		Buffer[Index] = L'\0'; // Asegurar el fin de cadena
		);
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	Free_MEM_FILE_INT(CONSOLEINSTANCE);
	SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);

	return EFI_SUCCESS;
}

#define ssun_main Console

/*
BITMAP_MAKER

Summary:
	a 8x8 only 0/1 bitmap maker
*/
EFI_STATUS
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
				printc(L" ");
			}
			gotoxy(0, cursory + 1);
		}
		// draw instructions
		printc(L"\n");
		SetScreenAtribute(1, green);
		printc(L" ");
		SetScreenAtribute(1, black);
		printc(L" = 0\n");
		SetScreenAtribute(1, brblue);
		printc(L" ");
		SetScreenAtribute(1, black);
		printc(L" = 1");

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
EFI_STATUS
SNAKE
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	// prepare the input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

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

	// the balls (O_o)... of the snake
	SnakeBALL poses[30];

	// set the food pos
	foodx = 4;
	foody = 4;

	// set the snake default size and direction
	snakesize = 3;
	snakedirection = 3;

	// set the screen
	SetScreenAtribute(1, black);
	ClearScreen();

	// set the snake pos
	snakeposx = 2;
	snakeposy = 4;

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
		printc(L"            \n            \n            \n            \n            \n            \n            \n            \n            ");
		SetScreenAtribute(1, black);

		// a snake scroe
		string ea[100];

		SetScreenAtribute(0, brgreen);
		// prints the snake score
		printc(L"\nSCORE: ");
		gotoxy(7, cursory);

		SPrint(ea,sizeof(ea), L"%d ", snakesize);
		SetScreenAtribute(0, brblue);

		printc(ea);

		// prints the snake pos
		SetScreenAtribute(0, brgreen);
		printc(L"\nPOS: ");
		gotoxy(5, cursory);
		SPrint(ea, sizeof(ea), L"x: %d y: %d", snakeposx, snakeposy);
		SetScreenAtribute(0, brblue);
		printc(ea);

		// draws the snake
		for (size_t i = 0; i < snakesize; i++)
		{
			if (poses[i].x != 32) {
				if (poses[i].y != 32) {
					gotoxy(poses[i].x, poses[i].y);
				}
			}

			SetScreenAtribute(1, blue);
			printc(L" ");
		}

		// draws the food
		gotoxy(foodx,foody);
		SetScreenAtribute(1, orange);
		printc(L" ");

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

	}
	return EFI_SUCCESS;
	// the ball (O_o)
}

/*
ADIVINA_EL_NUMERO

Summary:
	the game of try to write the correct number
*/
EFI_STATUS
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
			MinNumber = RandomInRange(0, 99);

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

		printc(input);
		
		
		UserPrompt = Atoi(input);

		// Yeah go to the verification

		CHAR16* state;
		if (UserPrompt == Number) {
			state = L"Perfect";
		}
		else if (UserPrompt >= Number - 2 | UserPrompt <= Number + 2) {
			state = L"Good";
		}
		else if (UserPrompt >= Number - 8 | UserPrompt <= Number + 8) {
			state = L"Great";
		}
		else if (UserPrompt >= Number - 14 | UserPrompt <= Number + 14) {
			state = L"hmmm passable";
		}
		else if (UserPrompt >= Number - 32 | UserPrompt <= Number + 32) {
			state = L"bad";
		}
		else {
			state = L"soo bad but dont worry you can try agin";
		}

		printc(L"\n");
		printc(state);
		printc(L"\n");

	}
	return EFI_SUCCESS;
}

/*
Desktop

Summary:
	the S-SUN desktop
*/
EFI_STATUS 
Desktop
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	InitializeLib(globalimagehandle, globalsystemtable);
	EFI_FILE_PROTOCOL* Root;
	EFI_STATUS Status;

	// input variables
	UINTN Event;
	EFI_INPUT_KEY Key;

	// the option
	INT8 Tab = 0;
	INT8 optionud = 0;

	// the screen size
	UINTN MaxColumns, MaxRows;

	// themes
	CSCHEME* SCDefault = newCSCHEME(yellow, gray, black);
	CSCHEME* SCNature = newCSCHEME(cyan, green, black);
	CSCHEME* SCBoy = newCSCHEME(blue, cyan, black);
	CSCHEME* SCGirl = newCSCHEME(brred, gray, black);
	CSCHEME* SCBilly = newCSCHEME(brblue, brgreen, black);
	CSCHEME* SCFire = newCSCHEME(brorange, red, black);
	CSCHEME* SCOcean = newCSCHEME(blue, brblue, black);
	CSCHEME* SCFuturistic = newCSCHEME(darkgray, brcyan, black);
	CSCHEME* SCDesert = newCSCHEME(bryellow, yellow, black);

	// set the default theme
	Sceme = SCDefault;

	// set the screen
	MaxColumns = (gop->Mode->Info->HorizontalResolution / 8) / Conio->atributes->size;
	MaxColumns = MaxColumns - 1;

	// create the taskbar text
	StrCpy(taskbar, L"");
	for (size_t i = 0; i < MaxColumns + 1; i++)
	{
		SPrint(taskbar, sizeof(taskbar), L"%s%c", taskbar, L' ');
	}

	MEM_FILE_INT* DESKTOPINSTANCE = Create_MEM_FILE_INT(L"DESKTOPINSTANCE", 1);
	SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &MaxColumns, &MaxRows);
	globalsystemtable->ConOut->ClearScreen(globalsystemtable->ConOut);
	DRAW_TEXT_DIALOG_NO_WAIT(ImageHandle, SystemTable, L"Please wait...", EFI_WHITE, EFI_BACKGROUND_CYAN);
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000); \

	ClearScreen();
	ChangeToGrapichalMode();	

	// DRAW_DIALOG_MSG_CONFIRM(ImageHandle, SystemTable, L"Welcome to S-SUN, Do you want see the survival guide?");

	SetScreenAtribute(1, Sceme->backgroundcolor);

	while (TRUE) {
		ChangeToGrapichalMode();
		SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);
		PIXELCOL syscolor2 = Sceme->buttonscolor;
		PIXELCOL sysbg = Sceme->backgroundcolor;
		PIXELCOL sysbtext = Sceme->buttonstext;
		// Configurar la pantalla
		MaxColumns = (gop->Mode->Info->HorizontalResolution / 8) / Conio->atributes->size;
		MaxColumns = MaxColumns - 1;
		MaxRows = (gop->Mode->Info->VerticalResolution / 12) / Conio->atributes->size;

		// Posicionar el cursor para el botón "Start"

		// UINTN MaxRows = MaxRows - 1; // Última fila

		SetScreenAtribute(1, sysbg);

		ClearScreen();

		gotoxy(0, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		printc(taskbar);

		// DRAW START
		gotoxy(0, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 0) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printc(L"\x4 S-SUN");

		// DRAW Themes
		gotoxy(8, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 1) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printc(TranslateWorck(&THEMES_BUTTON_TRANSL, languajecu));
	
		// DRAW games
		gotoxy(15, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 2) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printc(TranslateWorck(&GAMES_BUTTON_TRANSL, languajecu));

		// DRAW games
		gotoxy(21, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 3) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printc(TranslateWorck(&LANGS_BUTTON_TRANSL, languajecu));

		// Draw start menu
		if (optionud > 0) {
			if (Tab == 0) {
				for (INT8 i = 0; i < 7; i++)
				{
					gotoxy(0, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printc(L"             ");
				}

				// draw Shutdown button
				gotoxy(0, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(TranslateWorck(&OFF_BUTTON_TRANSL, languajecu));

				// draw restart button
				gotoxy(0, MaxRows - 1 - 2);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(TranslateWorck(&RESTART_BUTTON_TRANSL, languajecu));

				// draw editor button
				gotoxy(0, MaxRows - 1 - 3);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(TranslateWorck(&TEXTEDITOR_BUTTON_TRANSL, languajecu));

				// draw time view
				gotoxy(0, MaxRows - 1 - 4);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 4) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(TranslateWorck(&GETTIME_BUTTON_TRANSL, languajecu));

				// draw date view
				gotoxy(0, MaxRows - 1 - 5);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 5) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(TranslateWorck(&GETDATE_BUTTON_TRANSL, languajecu));

				// draw command prompt shortcut
				gotoxy(0, MaxRows - 1 - 6);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 6) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(TranslateWorck(&CONSOLE_BUTTON_TRANSL, languajecu));
			}
			if (Tab == 1) {
				for (INT8 i = 0; i < 9; i++)
				{
					gotoxy(8, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printc(L"        ");
				}

				// draw "Default" theme button
				gotoxy(8, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Default");

				// draw "Nature" theme button
				gotoxy(8, MaxRows - 1 - 2);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Nature");

				// draw "Boy" theme button
				gotoxy(8, MaxRows - 1 - 3);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Boy");

				// draw "Girl" theme button
				gotoxy(8, MaxRows - 1 - 4);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 4) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Girl");

				// draw "Billy" theme button
				gotoxy(8, MaxRows - 1 - 5);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 5) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Billy");

				// draw "Fire" theme button
				gotoxy(8, MaxRows - 1 - 6);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 6) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Fire");

				// draw "Ocean" theme button
				gotoxy(8, MaxRows - 1 - 7);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 7) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Ocean");

				// draw "Futuristic" theme button
				gotoxy(8, MaxRows - 1 - 8);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 8) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Futurist");

				// draw "Monochromatic" theme button
				gotoxy(8, MaxRows - 1 - 9);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 9) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Desert");
			}
			if (Tab == 2) {
				for (INT8 i = 0; i < 4; i++)
				{
					gotoxy(15, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printc(L"                        ");
				}

				// draw "ADIVINA EL NUMERO" GAME button
				gotoxy(15, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"ADIVINA EL NUMERO");

				// draw "BITMAP_MAKER" button
				gotoxy(15, MaxRows - 2 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"BITMAP_MAKER");

				// draw "Snake" GAME button
				gotoxy(15, MaxRows - 3 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Snake");
			}
			if (Tab == 3) {
				for (INT8 i = 0; i < 3; i++)
				{
					gotoxy(21, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printc(L"                        ");
				}

				// draw "English" lang button
				gotoxy(21, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"English");

				// draw "Spanish" lang button
				gotoxy(21, MaxRows - 2 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Spanish");

				// draw "Francais" lang button
				gotoxy(21, MaxRows - 3 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Francais");
			}
		}

		// Esperar eventos de teclado
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		// Manejo de navegación por teclado
		if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
			if (Tab == 0) {
				if (optionud == 1) {
					SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
					ClearScreen();
					uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
					SSUNSCREENLOGOUT(ImageHandle,SystemTable);
					globalsystemtable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL); \
				}
				if (optionud == 2) {
					SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
					ClearScreen();
					uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);
					SSUNSCREENLOGOUT(ImageHandle, SystemTable);
					globalsystemtable->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL); \
				}
				else if (optionud == 3) {
					editor(ImageHandle, SystemTable, L"editor default text :)");
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				else if (optionud == 4) {
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, KERNEL_GET_TIME(SystemTable));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				else if (optionud == 5)
				{
					DRAW_DIALOG_MSG(ImageHandle, SystemTable, KERNEL_GET_DATE(SystemTable));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
				else if (optionud == 6)
				{
					ssun_main(ImageHandle, SystemTable, L"DEV");
					SetScreenAtribute(0, gray);
					printc(TranslateWorck(&EXITMESSAGE_TRANS, languajecu));
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
			}
			else if (Tab == 1) {
				if (optionud == 1) {
					Sceme = SCDefault;
				}
				else if (optionud == 2) {
					Sceme = SCNature;
				}
				else if (optionud == 3) {
					Sceme = SCBoy;
				}
				else if (optionud == 4) {
					Sceme = SCGirl;
				}
				else if (optionud == 5) {
					Sceme = SCBilly;
				}
				else if (optionud == 6) {
					Sceme = SCFire;
				}
				else if (optionud == 7) {
					Sceme = SCOcean;
				}
				else if (optionud == 8) {
					Sceme = SCFuturistic;
				}
				else if (optionud == 9) {
					Sceme = SCDesert;
				}
			}
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
			}
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
		}
		else if (Key.ScanCode == SCAN_LEFT) {
			if (Tab > 0) { Tab--; }
		}
		else if (Key.ScanCode == SCAN_RIGHT) {
			Tab++;
		}
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
efi_main

Summary:
	...
*/
EFI_STATUS 
efi_main
(
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable
)
{
	UINTN Event;
	INT16 option;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;

	*languajecu = L"en";

	// EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	globalimagehandle = ImageHandle;
	globalsystemtable = SystemTable;
#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif
	// prepare the editor save
	*EditorProcess = NULL;

	// set the dog for that the dog dont f*** me
	Status = gBS->SetWatchdogTimer(0, 0, 0, NULL);

	// prepare the gop
	EFI_PHYSICAL_ADDRESS FrameBufferBase = gop->Mode->FrameBufferBase;
	UINTN FrameBufferSize = gop->Mode->FrameBufferSize;

	// set the screen size
	SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &horizontalResolution, &verticalResolution);

	// generalize it
	horizontalResolution = horizontalResolution / 8;
	verticalResolution = verticalResolution / 12;

	// Obtener la interfaz de gráficos
	Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&gop);
	if (EFI_ERROR(Status)) {
		Print(L"Error al obtener el protocolo de gráficos: %r\n", Status);
		return Status;
	}

	// Establecer un modo de gráficos específico
	UINTN Mode = 2; // Cambia esto según el modo disponible que desees utilizar
	Status = uefi_call_wrapper(gop->SetMode, 2, gop, Mode);
	if (EFI_ERROR(Status)) {
		Print(L"Error al establecer el modo de gráficos: %r\n", Status);
		return Status;
	}

	// change to text mode
	ChangeToTextMode();

	// anti bugs
	PrintLineWithBackground(SystemTable, L"S-SUN anti display bugs text", 0, EFI_BLACK, EFI_BACKGROUND_LIGHTGRAY);

	// prepare the s++ consoleoutpud color
	consoleoutpudcurrentcolor = white;

	// initialize my text mode protocol
	initializeMoonScreen();

	// clear the screen
	ClearScreen();

	CHAR16* ldata = NULL;
	UINTN BufferSize = 0;

	// get the languaje variable
	Status = gRT->GetVariable(
		L"S-SUN_Languaje",
		&SmallVariables,
		NULL,
		sizeof(ldata),
		&ldata
	);

	string* ea[100];

	StatusToString(ea, Status);
	printc(L"\nstatus on load the system languaje: \n");
	printc(ea);
	printc(L"\n");

	if (Status == EFI_NOT_FOUND) {
		UpdateLanguaje();
	}
	else if (Status == EFI_BUFFER_TOO_SMALL) {
		UpdateLanguaje();
	}
	else if (Status == EFI_SUCCESS) {
		Status = SystemTable->RuntimeServices->GetVariable(
			L"S-SUN_Languaje",
			&SmallVariables,
			NULL,
			sizeof(ldata),
			&ldata
		);
		languajecu = ldata;
	}

	// prepare the instances
	MEM_FILE_INT* Kernel_Startup = Create_MEM_FILE_INT(L"Kernel_Initialized",1);
	MEM_FILE_INT* Kernel_Instance = Create_MEM_FILE_INT(L"Kernel_Instance", 1);

	// gnu-efi debug
	#ifdef _DEBUG
		MEM_FILE_INT* DEBUG = Create_MEM_FILE_INT(L"DEBUG GNU-EFI", 1);
	#endif // DEBUG

	// wait
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 100000);

	// clear the screen
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&gop);

	// SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

	printc(L"S-SUN startup logs\n");
	printc(L"\nIf you found a problem with system this can help you to fix it\n\nSystem Startup Logs:\n\n");
	
	// initialize the kernel
	KERNEL_INITIALIZE;

	// set the pre-loadscreen colors
	ClearScreen();
	SetScreenAtribute(1, black);
	SetScreenAtribute(0, white);

	printc(L"press F1 to skip init.spp\n");
	FS_IMPACT data;
	UINTN atributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
	
	// get the system state
	Status = SystemTable->RuntimeServices->GetVariable(
		L"S-SUN_State",
		&VariablesGuid,
		NULL,
		sizeof(data),
		&data
	);

	// verific the state
	if (Status == EFI_NOT_FOUND) {
		InitializeFileSystem();

		// update the filesystem
		updatefilesystem();
	}
	else if (!EFI_ERROR(Status))
	{
		// load the variable
		Status = SystemTable->RuntimeServices->GetVariable(
			L"S-SUN_State",
			&VariablesGuid,
			NULL,
			sizeof(data),
			&data
		);
		CurrentFS = data;

		if (&CurrentFS == &data) {
			printc(L"\nthe system state has been loaded correctlly. :) \n");
		}
	}
	if (Status == EFI_BUFFER_TOO_SMALL) {
		// fix the buffer
		printc(L"\ntryting to view what is the reason of BUFFER_TO_SMALL >:)\ndont worry i try to save you S-SUN files ;)\n");

		InitializeFileSystem();

		updatefilesystem();
	}
	else
	{
		// initialize the filesystem
		Status = RT->GetVariable(
			L"S-SUN_State",
			&VariablesGuid,
			NULL,
			sizeof(data),
			&data
		);
		// set the filesystem
		CurrentFS = data;
		string* e[100];

		// get status
		StatusToString(e, Status);
		printc(L"\nstatus on load the system state: \n");
		printc(e);
		printc(L"\n");
	}
	
	// wait
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 500000);

	// loading screen
	initializeMoonScreen();
	SetScreenAtribute(1, blue);
	ClearScreen();

	printc(L"\nS-SUN BootLoader v0.2\n\n");
	printc(L"\nS-SUN Operating System\nmaded By ErickCraftStudios\n\n");
	// the animation
	for (INT8 i = 0; i < 30; i++)
	{
		SetScreenAtribute(0, white);
		SetScreenAtribute(1, blue);
		printc(L".");
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);

		// press the f1 to skip Init.spp
		uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
		if (Key.ScanCode == SCAN_F1) {
			break;
		}
	}
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000);

	// if not skiped initialize Init.spp
	if (Key.ScanCode != SCAN_F1) {
		ExecuteScript(CurrentFS.HEY_CURRENT_SESSION[3].Content, ImageHandle, SystemTable);
	}
		// The platform logo may still be displayed → remove it
			/*
		SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
		option = 1;
		while (TRUE) {
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);
			SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
			Print(L"\n  Select an option to start S-SUN or continue with your OS\n\n");
			uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
			if (Key.ScanCode == SCAN_UP) {
				option--;
				if (option < 1) option = 4; // Si la opción es menor a 1, ir a la última opción
			}
			else if (Key.ScanCode == SCAN_DOWN) {
				option++;
				if (option > 4) option = 1; // Si la opción es mayor a 3, ir a la primera opción
			}
			else if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
				if (option == 1) {
					// Iniciar S-SUN normalmente
					ssun_main(ImageHandle, SystemTable, L"Norm");
				}
				else if (option == 2) {
					// Iniciar S-SUN en modo desarrollador
					ssun_main(ImageHandle, SystemTable, L"DEV");
				}
				else if (option == 3) {
					SystemManager(ImageHandle, SystemTable);
				}
				else if (option == 4) {
					// Salir
					return EFI_SUCCESS;
				}
			}

			// Mostrar opciones del menú
			Print(L"    ");
			if (option == 1) {
				SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
			}
			Print(L"Start S-SUN Normally\n");
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);
			Print(L"    ");

			if (option == 2) {
				SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
			}
			Print(L"Start S-SUN Developer Mode (in this I test new things before I launch it)\n");
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);
			Print(L"    ");

			if (option == 3) {
				SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
			}
			Print(L"Start S-SUN System Manager\n");
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);

			Print(L"    ");

			if (option == 4) {
				SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
			}
			Print(L"Exit\n");
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);

			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
		}
		*/
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);

	// initialize the desktop
	Desktop(ImageHandle, SystemTable);
	Free_MEM_FILE_INT(Kernel_Instance);
	return Status;
}