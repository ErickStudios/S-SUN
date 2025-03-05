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
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include ".vs/msvc/kernelMK.h"

#define MAX_VARIABLES 100

EFI_STATUS WriteFile(CHAR16* FileName, CHAR16* Content) {
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

CHAR16* StrStr(CHAR16* haystack, CHAR16* needle) {
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

VOID SetVariable(CHAR16* variable, CHAR16* value) {
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

CHAR16* GetVariable(CHAR16* variable) {
	for (UINTN i = 0; i < variable_count; i++) {
		if (StrCmp(variables[i].variable, variable) == 0) {
			return variables[i].value;
		}
	}
	return NULL;
}

CHAR16* StrToken(CHAR16* str, const CHAR16* delim);

EFI_STATUS SSUNSCREENLOGOUT(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	initializeMoonScreen();
	SetScreenAtribute(1, blue);
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

CHAR16 ExecuteCommand(CHAR16* buffer, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, CHAR16 mode);
// Función para dividir una cadena CHAR16 en líneas
CHAR16** SplitLines(CHAR16* text, UINTN* line_count) {
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
CHAR16* StrToken(CHAR16* str, const CHAR16* delim) {
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
VOID PrintLineWithBackground(EFI_SYSTEM_TABLE* SystemTable, CHAR16* text, UINTN line, UINTN COLOR, UINTN BG) {
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

EFI_STATUS DrawCursor(EFI_SYSTEM_TABLE* SystemTable, CHAR16 CursorChar) {
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
EFI_STATUS DrawText(EFI_SYSTEM_TABLE* SystemTable, CHAR16* Text, UINTN X, UINTN Y) {
	SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, X, Y);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, Text);
	return EFI_SUCCESS;
}

EFI_STATUS guimode(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
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

	// Dibujar fondo de pantalla con un color sólido (azul)
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL BackgroundColor = { 0, 200, 200, 0 }; // Azul
	Status = DrawRectangle(GraphicsOutput, 0, 0, GraphicsOutput->Mode->Info->HorizontalResolution, GraphicsOutput->Mode->Info->VerticalResolution, BackgroundColor);
	if (EFI_ERROR(Status)) {
		return Status;
	}

	// Dibujar barra de tareas en la parte inferior de la pantalla
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL TaskbarColor = { 192, 192, 192, 0 }; // Gris claro
	UINT32 TaskbarHeight = 30;
	DrawRectangle(GraphicsOutput, 0, GraphicsOutput->Mode->Info->VerticalResolution - TaskbarHeight, GraphicsOutput->Mode->Info->HorizontalResolution, TaskbarHeight, TaskbarColor);

	// Inicializar posición del cursor
	UINT32 CursorX = 0;
	UINT32 CursorY = 0;
	UINT32 CursorSize = 10; // Tamaño del cursor
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL CursorColor = { 255, 255, 255, 0 }; // Blanco

	// Bucle principal para mover el cursor
	while (TRUE) {
		if (CursorY > 450) {
			Status = DrawRectangle(GraphicsOutput, 0, GraphicsOutput->Mode->Info->VerticalResolution - TaskbarHeight, GraphicsOutput->Mode->Info->HorizontalResolution, TaskbarHeight, TaskbarColor);
			if (EFI_ERROR(Status)) {
				return Status;
			}

			// Dibujar texto en la pantalla
			CHAR16* WelcomeText = L"S-SUN";
			DrawText(SystemTable, WelcomeText, 0, 5);
		}

		// Dibujar el cursor
		Status = DrawRectangle(GraphicsOutput, CursorX, CursorY, CursorSize, CursorSize, CursorColor);
		if (EFI_ERROR(Status)) {
		}

		// Esperar entrada del usuario
		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
		Status = uefi_call_wrapper(SystemTable->BootServices->WaitForEvent, 3, 1, &SystemTable->ConIn->WaitForKey, &Event);
		if (EFI_ERROR(Status)) {
		}

		// Leer entrada del usuario
		Status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
		if (EFI_ERROR(Status)) {
		}
		EFI_GRAPHICS_OUTPUT_BLT_PIXEL bgtodraw;
		// Borrar el cursor actual (pintar el área con el fondo)
		if (CursorY < 450)
		{
			bgtodraw = BackgroundColor;
		}
		else {
			bgtodraw = TaskbarColor;
		}
		Status = DrawRectangle(GraphicsOutput, CursorX, CursorY, CursorSize, CursorSize, bgtodraw);
		if (EFI_ERROR(Status)) {
		}

		// Actualizar posición del cursor basado en la entrada
		if (Key.ScanCode == SCAN_UP) {
			CursorY -= 10;
		}
		else if (Key.ScanCode == SCAN_DOWN) {
			CursorY += 10;
		}
		else if (Key.ScanCode == SCAN_LEFT) {
			CursorX -= 10;
		}
		else if (Key.ScanCode == SCAN_RIGHT) {
			CursorX += 10;
		}
		else if (Key.ScanCode == SCAN_F5) {
			break;
		}

		// Asegurarse de que el cursor no salga de los límites de la pantalla
		if (CursorX < 0) CursorX = 0;
		if (CursorY < 0) CursorY = 0;
		if (CursorX > GraphicsOutput->Mode->Info->HorizontalResolution - CursorSize) CursorX = GraphicsOutput->Mode->Info->HorizontalResolution - CursorSize;
		if (CursorY > GraphicsOutput->Mode->Info->VerticalResolution - CursorSize) CursorY = GraphicsOutput->Mode->Info->VerticalResolution - CursorSize - TaskbarHeight;
	}

	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
	return EFI_SUCCESS;
}

EFI_STATUS ssun_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, CHAR16 mode);

EFI_STATUS editor(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);

EFI_STATUS SystemManager(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_INPUT_KEY Key;
	EFI_STATUS Status;
	UINTN Event;

	InitializeLib(ImageHandle, SystemTable);

	MEM_FILE_INT* SYMGR_INSTANCE = Create_MEM_FILE_INT(L"SYMGR_INSTANCE", 1);
	while (TRUE)
	{
		WHILESYSTEMRUNNING(
			EFI_TIME Time;
		SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
		SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

		PrintLineWithBackground(SystemTable, L"S-SUN system manager", 0, EFI_BLACK, EFI_BACKGROUND_LIGHTGRAY);

		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
		Print(L"\nPlease select an option\n\n");
		Print(L"<F1> Exit\n");
		Print(L"<F2> Get the time\n");
		Print(L"<F3> Get the date\n");
		Print(L"<F4> About S-SUN\n");
		Print(L"<F5> editor\n");

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
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
			SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

			PrintLineWithBackground(SystemTable, L"S-SUN - about S-SUN", 0, EFI_BLACK, EFI_BACKGROUND_LIGHTGRAY);
			SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);

			Print(L"\nENGLISH\n\n");
			Print(L"S-SUN is a very small Operating System maded by ErickCraftStudios with C and Visual Studio with help of a template (uefi-simple: https://github.com/pbatard/uefi-simple) \n");
			Print(L"\n\nESPAÑOL\n\n");
			Print(L"S-SUN es un sistema operativo muy pequeño hecho por ErickCraftStudios con C y Visual Studio con ayuda de una plantilla (uefi-simple: https://github.com/pbatard/uefi-simple) \n");
			SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);

		}
		else if (Key.ScanCode == SCAN_F5)
		{
			editor(ImageHandle, SystemTable);
		}
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
			);
	}
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	Free_MEM_FILE_INT(SYMGR_INSTANCE);
	return EFI_SUCCESS;
}

EFI_FILE_PROTOCOL* CurrentDirectory = NULL;

EFI_STATUS ListStorageDevices(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
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

EFI_STATUS CreateANewDirFUNCTION(CHAR16* DirName, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
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

EFI_STATUS ChangeDirectory(CHAR16* DirName, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
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

EFI_STATUS ListDirectories(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
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

CHAR16* ProcessText(CHAR16* buffer) {
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

EFI_STATUS editor(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	InitializeLib(ImageHandle, SystemTable);

	CHAR16 text[2048] = L""; // Definir un tamaño suficiente para el búfer de entrada
	CHAR16 save_text[2048] = L""; // Búfer para guardar el texto
	UINTN Index = 0;
	UINTN Event;
	EFI_INPUT_KEY Key;
	UINTN MaxColumns, MaxRows;
	int SurvivalGuideAlt;

	MEM_FILE_INT* EDITOR_INSTANCE = Create_MEM_FILE_INT(L"EDITOR_INSTANCE", 1);
	SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);

	SurvivalGuideAlt = 0;
	while (TRUE) {
			ChangeToTextMode();
		SystemTable->ConOut->EnableCursor(SystemTable->ConOut, TRUE);
		SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &MaxColumns, &MaxRows);
		// Limpiar y actualizar la pantalla
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
		SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
		ClearScreen();
		gotoxy(0,0);
		SetScreenAtribute(1, gray);
		SetScreenAtribute(0, black);
		printc(L"S-SUN editor <ESC = exit> <F1 = see the survival guide> <F5 = run as S++>");
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
		SetScreenAtribute(0, gray);
		SetScreenAtribute(1, black);
		printc(L"\n\n\n");
		// Dividir el texto en líneas y ejecutar cada línea
		StrCpy(save_text, text);
		UINTN line_count1 = 0;
		CHAR16** lines1 = SplitLines(text, &line_count1);
		if (SurvivalGuideAlt == 0) {
			for (UINTN i2 = 0; i2 < line_count1; i2++) {
				printc(lines1[i2]);
				if (i2 + 1 != line_count1) {
					printc(L"\n");
				}
			}
		}
		else
		{
			SetScreenAtribute(0, brblue);
			SetScreenAtribute(1, black);
			printc(L"S-SUN editor survival guide\n\n");
			SetScreenAtribute(0, gray);
			SetScreenAtribute(1, black);
			printc(L"S-SUN editor is a powerfull tool for make documments\nor programs.\n\n");
			SetScreenAtribute(0, brblue);
			SetScreenAtribute(1, black);
			printc(L"How To Use\n\n");
			SetScreenAtribute(0, gray);
			SetScreenAtribute(1, black);
			printc(L"use it as another editor , to make programs press F5\n\nNOTE: you cant save your file\n so dont make big programs");
		}
		// Restaurar el texto guardado
		StrCpy(text, save_text);
		UINTN Buttom = MaxRows - 1; // Última fila
		// Leer una tecla
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
		SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

		// Manejar la entrada de teclado
		if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
			text[Index++] = L'\n';
			Index == 0;
		}
		else if (Key.UnicodeChar == CHAR_BACKSPACE) {
			if (Index > 0) {
				text[--Index] = L'\0';
			}
		}
		else if (Key.ScanCode == SCAN_F1)
		{
			if (SurvivalGuideAlt == 0) {
				SurvivalGuideAlt = 1;
			}
			else
			{
				SurvivalGuideAlt = 0;
			}
		}
		else if (Key.ScanCode == SCAN_ESC) {
			break;
		}
		else if (Key.ScanCode == SCAN_F5) {
			// Guardar el texto antes de ejecutar los comandos
			StrCpy(save_text, text);
			// Dividir el texto en líneas y ejecutar cada línea
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
			// Restaurar el texto guardado
			StrCpy(text, save_text);
		}
		else {
			if (Key.UnicodeChar != 0) {

				text[Index++] = Key.UnicodeChar;
			}
		}

		text[Index] = L'\0'; // Asegurar el fin de cadena
		WHILESYSTEMRUNNING();
	}

	string* filepath = ReadLine(L"\nFile to save: ");
	if (filepath != NULL) {
		WriteFile(filepath, text);
	}
	Free_MEM_FILE_INT(EDITOR_INSTANCE);
	return EFI_SUCCESS;
}

CHAR16 ExecuteCommand(CHAR16* buffer, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, CHAR16 mode) {
	EFI_INPUT_KEY Key;
	UINTN Event;

	InitializeLib(ImageHandle,SystemTable);
	if (StrnCmp(buffer, L"echo ", 5) == 0) {
		CHAR16* msgtoecho = buffer + 5;
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
		printc(L"\n");
		printc(ProcessText(msgtoecho));
	}
	if (StrnCmp(buffer, L"writel ", 7) == 0) {
		CHAR16* msgtoecho = buffer + 7;
		SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
		printc(ProcessText(msgtoecho));
	}
	else if (StrCmp(buffer, L"wait") == 0)
	{
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 500000); \
	}
	else if (StrnCmp(buffer, L"throw ", 6) == 0) {
		CHAR16* reason = buffer + 6;
		THROW_ERROR(ProcessText(reason));
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
		for (UINTN i = 0; i < MemFileCountString; i++) {
			if (StrCmp(MemFilesString[i]->NAME, MemName) == 0) {
				printc(MemFilesString[i]->VALUE);
				found = true;
				break;
			}
		}
		if (!found) {
			printc(L"\n");
			printc(L"MEM not founded");
		}
	}
	else if (StrCmp(buffer, L"FlushMEM ", 9) == 0) {
		CHAR16* command = buffer + 9;
		Flush_MEM_FILE_STRING(command);
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
		if (StrCmp(mode, L"DEV") == 0) {
			CHAR16* DirName = ProcessText(buffer + 3);
			ChangeDirectory(DirName, ImageHandle, SystemTable);
		}
	}	
	else if (StrCmp(buffer, L"ls") == 0) {
		if (StrCmp(mode, L"DEV") == 0) {
			ListDirectories(ImageHandle, SystemTable);
		}
	}
	else if (StrCmp(buffer, L"cls") == 0) {
		ClearScreen();
	}
	else if (StrCmp(buffer, L"exit") == 0) {
		return L"sistema , tu trabajo ha terminado";
	}

	return L"sistema , si puedes seguir";
}

EFI_STATUS ssun_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, CHAR16 mode) {
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

EFI_STATUS Desktop(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	InitializeLib(globalimagehandle, globalsystemtable);
	EFI_FILE_PROTOCOL* Root;
	EFI_STATUS Status;
	UINTN mousex = 0;
	UINTN mousey = 0;
	UINTN Event;
	EFI_INPUT_KEY Key;
	UINTN Tab = 0;
	UINTN optionud = 0;

	UINTN MaxColumns, MaxRows;

	CSCHEME* Sceme;

	CSCHEME* SCDefault = newCSCHEME(yellow, gray, black);
	CSCHEME* SCNature = newCSCHEME(cyan, green, black);
	CSCHEME* SCBoy = newCSCHEME(blue, cyan, black);
	CSCHEME* SCGirl = newCSCHEME(brred, gray, black);
	CSCHEME* SCBilly = newCSCHEME(brblue, brgreen, black);
	CSCHEME* SCFire = newCSCHEME(brorange, red, black);
	CSCHEME* SCOcean = newCSCHEME(blue, brblue, black);

	Sceme = SCDefault;

	MEM_FILE_INT* DESKTOPINSTANCE = Create_MEM_FILE_INT(L"DESKTOPINSTANCE", 1);
	SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &MaxColumns, &MaxRows);
	globalsystemtable->ConOut->ClearScreen(globalsystemtable->ConOut);
	DRAW_TEXT_DIALOG_NO_WAIT(ImageHandle, SystemTable, L"Please wait...", EFI_WHITE, EFI_BACKGROUND_CYAN);
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 5, 1000000); \

	while (TRUE) {
		ChangeToGrapichalMode();
		SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);
		PIXELCOL syscolor2 = Sceme->buttonscolor;
		PIXELCOL sysbg = Sceme->backgroundcolor;
		PIXELCOL sysbtext = Sceme->buttonstext;
		// Configurar la pantalla
		MaxColumns = gop->Mode->Info->HorizontalResolution / 8;
		MaxColumns = MaxColumns - 1;
		MaxRows = gop->Mode->Info->VerticalResolution / 12;

		// Posicionar el cursor para el botón "Start"

		// UINTN MaxRows = MaxRows - 1; // Última fila

		SetScreenAtribute(1, sysbg);

		ClearScreen();
		PrintSSL(L" ", MaxRows - 1, sysbtext, syscolor2, MaxColumns);

		// DRAW START
		gotoxy(0, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 0) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printc(L"S-SUN");

		// DRAW Themes
		gotoxy(8, MaxRows - 1);
		SetScreenAtribute(1, syscolor2);
		SetScreenAtribute(0, sysbtext);
		if (Tab == 1) {
			SetScreenAtribute(1, sysbtext);
			SetScreenAtribute(0, syscolor2);
		}
		printc(L"themes");
	
		// Draw start menu
		if (optionud > 0) {
			if (Tab == 0) {
				for (UINTN i = 0; i < 9; i++)
				{
					gotoxy(0, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printc(L"               ");
				}

				// draw Shutdown button
				gotoxy(0, MaxRows - 1 - 1);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 1) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Shutdown");

				// draw restart button
				gotoxy(0, MaxRows - 1 - 2);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 2) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Restart");

				// draw editor button
				gotoxy(0, MaxRows - 1 - 3);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 3) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Text Editor");

				// draw time view
				gotoxy(0, MaxRows - 1 - 4);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 4) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Get the time");

				// draw date view
				gotoxy(0, MaxRows - 1 - 5);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 5) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Get the date");

				// draw command prompt shortcut
				gotoxy(0, MaxRows - 1 - 6);
				SetScreenAtribute(1, syscolor2);
				SetScreenAtribute(0, sysbtext);
				if (optionud == 6) {
					SetScreenAtribute(1, sysbtext);
					SetScreenAtribute(0, syscolor2);
				}
				printc(L"Console");
			}
			if (Tab == 1) {
				for (UINTN i = 0; i < 8; i++)
				{
					gotoxy(8, MaxRows - 1 - 1 - i);
					SetScreenAtribute(1, syscolor2);
					SetScreenAtribute(0, sysbtext);
					printc(L"               ");
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
					editor(ImageHandle, SystemTable);
					SetScreenAtribute(0, gray);
					printc(L"\nApplication closed press any key to continue...");
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
					printc(L"\nApplication closed press any key to continue...");
					SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
				}
			}
			if (Tab == 1) {
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

// S-SUN main
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	UINTN Event;
	INT16 option;
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	globalimagehandle = ImageHandle;
	globalsystemtable = SystemTable;
#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif
	InitializeLib(ImageHandle, SystemTable);

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

	SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	SystemTable->ConOut->EnableCursor(SystemTable->ConOut, FALSE);
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	ChangeToTextMode();
	PrintLineWithBackground(SystemTable, L"S-SUN anti display bugs text", 0, EFI_BLACK, EFI_BACKGROUND_LIGHTGRAY);

	MEM_FILE_INT* Kernel_Startup = Create_MEM_FILE_INT(L"Kernel_Initialized",1);
	MEM_FILE_INT* Kernel_Instance = Create_MEM_FILE_INT(L"DESKTOPINSTANCE", 1);
	#ifdef DEBUG
		MEM_FILE_INT* DEBUG = Create_MEM_FILE_INT(L"DEBUG GNU-EFI", 1);
	#endif // DEBUG
		uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 100000);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&gop);
	initializeMoonScreen();
	ClearScreen();

	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	printc(L"S-SUN startup logs\n");
	printc(L"\nIf you found a problem with system this can help you to fix it\n\nSystem Startup Logs:\n\n");
	KERNEL_INITIALIZE;
	// Install system
	/*
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN", ImageHandle, SystemTable);
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN\\System", ImageHandle, SystemTable);
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 100000);
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN\\System\\KERNEL_SERVICES", ImageHandle, SystemTable);
	uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 100000);
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN\\User", ImageHandle, SystemTable);
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN\\User\\Documments", ImageHandle, SystemTable);
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN\\User\\Scripts", ImageHandle, SystemTable);
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN\\User\\Sources", ImageHandle, SystemTable);
	KERNELCALL_CREATEDIR_PARAMS(L"\\S-SUN\\User\\Projects", ImageHandle, SystemTable);
	*/
	initializeMoonScreen();
	SetScreenAtribute(1, blue);
	ClearScreen();
	printc(L"\nS-SUN BootLoader v0.2\n\n");
	printc(L"\nS-SUN Operating System\nmaded By ErickCraftStudios\n\n");
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
	Desktop(ImageHandle, SystemTable);
	Free_MEM_FILE_INT(Kernel_Instance);
	return Status;
}