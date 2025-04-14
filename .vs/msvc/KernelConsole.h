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

/*+++
* KernelConsole.h
+++*/

#include <efi.h>
#include <efilib.h>
#include "KernelCore.h"  
#include "KernelDef.h"

// S-SUN kernel Console
#ifndef _KERNEL_CONSOLE_
#define _KERNEL_CONSOLE_

EFI_DEVICE_IO_INTERFACE *GlobalIoFncs; // Declaración externa

#define CONSOLE_BRIGHT_BG  0x80
#define CONSOLE_YELLOW_BG (EFI_BACKGROUND_BROWN | CONSOLE_BRIGHT_BG)

#define CONSOLE_COLOR_TO_BG(color) \
    CONSOLE_COLOR_TO_BG_f(color)

    UINTN CONSOLE_COLOR_TO_BG_f(UINTN color) {
        if (color == EFI_BLACK) { return EFI_BACKGROUND_BLACK; }
        if (color == EFI_WHITE) { return EFI_BACKGROUND_LIGHTGRAY; }
        if (color == EFI_LIGHTGRAY) { return EFI_BACKGROUND_LIGHTGRAY; }
        if (color == EFI_RED) { return EFI_BACKGROUND_RED; }
        if (color == EFI_YELLOW) { return CONSOLE_YELLOW_BG; }
        if (color == EFI_BLUE) { return EFI_BACKGROUND_BLUE; }
        if (color == EFI_BROWN) { return EFI_BACKGROUND_BROWN; }
        if (color == EFI_CYAN) { return EFI_BACKGROUND_CYAN; }
        if (color == EFI_GREEN) { return EFI_BACKGROUND_GREEN; }
        if (color == EFI_MAGENTA) { return EFI_BACKGROUND_MAGENTA; }
        return EFI_BACKGROUND_LIGHTGRAY;
    }

#define CONSOLE_BG_TO_COLOR(color) \
    CONSOLE_BG_TO_COLOR_f(color)

    UINTN CONSOLE_BG_TO_COLOR_f(UINTN color) {
        if (color == EFI_BACKGROUND_BLACK) { return EFI_BLACK; }
        if (color == EFI_BACKGROUND_LIGHTGRAY) { return EFI_WHITE; }
        if (color == EFI_BACKGROUND_LIGHTGRAY) { return EFI_LIGHTGRAY; }
        if (color == EFI_BACKGROUND_RED) { return EFI_RED; }
        if (color == CONSOLE_YELLOW_BG) { return EFI_YELLOW; }
        if (color == EFI_BACKGROUND_BLUE) { return EFI_BLUE; }
        if (color == EFI_BACKGROUND_BROWN) { return EFI_BROWN; }
        if (color == EFI_BACKGROUND_CYAN) { return EFI_CYAN; }
        if (color == EFI_BACKGROUND_GREEN) { return EFI_GREEN; }
        if (color == EFI_BACKGROUND_MAGENTA) { return EFI_MAGENTA; }
        return EFI_WHITE;
    }

#define DRAW_TEXT_DIALOG(ImageHamdle ,SystemTable, message, color, bg) \
    ShowCenteredDialog(ImageHamdle, SystemTable, message, color, bg);
    EFI_STATUS 
    ShowCenteredDialog
    (
        EFI_HANDLE* ImageHamdle,
        EFI_SYSTEM_TABLE* SystemTable, 
        CHAR16* message, 
        UINTN color, 
        UINTN BG
    )
    {
        UINTN MaxColumns, MaxRows;
        CHAR16 FullMessage[100];
        SPrint(FullMessage, sizeof(FullMessage), L"   %s   ", message);

        UINTN MsgLength = StrLen(FullMessage);
        UINTN StartColumn, StartRow;

        InitializeLib(ImageHamdle, SystemTable);

        // Obtener el tamaño de la pantalla
        MaxRows = (verticalResolution / 12) / Conio->atributes->size;
        MaxColumns = (horizontalResolution / 8) / Conio->atributes->size;

        // Calcular la posición inicial para centrar el mensaje
        StartColumn = (MaxColumns - MsgLength) / 2;
        StartRow = MaxRows / 2;

        // Establecer el atributo de color de fondo y texto
        gotoxy(StartColumn, StartRow - 2);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        gotoxy(StartColumn, StartRow - 1);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        // Mover el cursor a la posición inicial calculada
        gotoxy(StartColumn, StartRow);
        SetScreenAtribute(0, white);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        gotoxy(StartColumn, StartRow + 1);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        gotoxy(StartColumn, StartRow + 2);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        gotoxy(StartColumn, StartRow + 3);
        SetScreenAtribute(0, white);
        SetScreenAtribute(1, black);
        printc(L" [OK (AnyKey) ] ");
        // Esperar a que se presione cualquier tecla
        if (message == L"" || message == L"(null)") {
            THROW_ERROR(THROW_NULLSTRING);
        }
        EFI_INPUT_KEY Key;
        UINTN Event;
        SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
        while (TRUE)
        {
            EFI_INPUT_KEY Key;
            UINTN Event;
            SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
            break;
        }
        return EFI_SUCCESS;
    }

#define DRAW_DIALOG_MSG(ImageHamdle ,SystemTable, message, bar , text) \
    ShowCenteredDialoga(ImageHamdle, SystemTable, message, text , bar, Sceme);
    EFI_STATUS
        ShowCenteredDialoga
        (
            EFI_HANDLE* ImageHamdle,
            EFI_SYSTEM_TABLE* SystemTable,
            CHAR16* message,
            PIXELCOL bar,
            PIXELCOL text,
            CSCHEME* Colors
    )
    {
        Pixels* savedbufsc = AllocatePool(sizeof(Pixels) * pixels);
        for (size_t i = 0; i < pixels; i++) {
            savedbufsc[i] = bufferscreen[i];
        }

        UINTN savpixels = pixels;

        UINTN MaxColumns, MaxRows;
        CHAR16* FullMessage[100];
        CHAR16 CloseMsg[100];

        INT16 x = 0;
        INT16 y = 0;
        SPrint(FullMessage, sizeof(FullMessage), L"      %s      ", message);
        SPrint(CloseMsg, sizeof(CloseMsg), L"  OK  ", message);

        UINTN MsgLength = StrLen(FullMessage);
        UINTN CloseMsgLength = StrLen(CloseMsg);
        UINTN StartColumn, StartRow;

        // Obtener el tamaño de la pantalla
        MaxRows = (verticalResolution / 12) / Conio->atributes->size;
        MaxColumns = (horizontalResolution / 8) / Conio->atributes->size;

        EFI_INPUT_KEY Key;
        UINTN Event;
        while (TRUE)
        {
            // Calcular la posición inicial para centrar el mensaje
            StartColumn = ((MaxColumns - MsgLength) / 2) + x;
            StartRow = (MaxRows / 2) + y;

            gotoxy(StartColumn, StartRow - 2);
            SetScreenAtribute(0, bar);
            SetScreenAtribute(1, bar);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow - 2);
            SetScreenAtribute(1, bar);
            SetScreenAtribute(0, text);
            printcu(L"MsgBox");

            gotoxy(StartColumn, StartRow - 1);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow);
            SetScreenAtribute(0, bar);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow + 1);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow + 2);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow + 3);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(((MaxColumns - CloseMsgLength) / 2) + x, (StartRow + 2));
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, bar);
            printcu(CloseMsg);

            DrawScreen();
            SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
            SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
            if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
                return EFI_SUCCESS;
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
        }

        pixels = savpixels;
        for (size_t i = 0; i < savpixels; i++) {
            bufferscreen[i] = savedbufsc[i];
        }
        FreePool(savedbufsc);
        DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Colors->backgroundcolor);
        return EFI_SUCCESS;
    }

#define DRAW_DIALOG_MSG_CONFIRM(ImageHamdle ,SystemTable, message, bar , text) \
    ShowCenteredDialogaConf(ImageHamdle, SystemTable, message, text , bar, Sceme);
    BOOLEAN 
    ShowCenteredDialogaConf
    (
        EFI_HANDLE* ImageHamdle, 
        EFI_SYSTEM_TABLE* SystemTable,
        CHAR16* message,
        PIXELCOL bar,
        PIXELCOL text,
        CSCHEME *Colors
    ) 
    {
        Pixels* savedbufsc = AllocatePool(sizeof(Pixels) * pixels);
        for (size_t i = 0; i < pixels; i++) {
            savedbufsc[i] = bufferscreen[i];
        }

        UINTN savpixels = pixels;

        INT16 x = 0;
        INT16 y = 0;

        UINTN MaxColumns, MaxRows;
        CHAR16* FullMessage[100];
        CHAR16 CloseMsg[100];
        EFI_INPUT_KEY Key;

        SPrint(FullMessage, sizeof(FullMessage), L"      %s      ", message);
        SPrint(CloseMsg, sizeof(CloseMsg), L"  OK  ", message);

        UINTN MsgLength = StrLen(FullMessage);
        UINTN CloseMsgLength = StrLen(CloseMsg);
        UINTN StartColumn, StartRow;

        InitializeLib(ImageHamdle, SystemTable);

        // Obtener el tamaño de la pantalla
        MaxRows = (verticalResolution / 12) / Conio->atributes->size;
        MaxColumns = (horizontalResolution / 8) / Conio->atributes->size;

        UINTN Event;
        UINTN tab;
        while (TRUE)
        {
            // Calcular la posición inicial para centrar el mensaje
            StartColumn = ((MaxColumns - MsgLength) / 2) + x;
            StartRow = (MaxRows / 2) + y;

            gotoxy(StartColumn, StartRow - 2);
            SetScreenAtribute(0, bar);
            SetScreenAtribute(1, bar);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow - 2);
            SetScreenAtribute(1, bar);
            SetScreenAtribute(0, text);
            printcu(L"MsgBox");

            gotoxy(StartColumn, StartRow - 1);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow);
            SetScreenAtribute(0, bar);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow + 1);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow + 2);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn, StartRow + 3);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, text);
            printcu(FullMessage);

            gotoxy(StartColumn + 1, StartRow + 2);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, bar);
            printcu(L"YES (enter)");

            gotoxy(StartColumn + 15, StartRow + 2);
            SetScreenAtribute(0, text);
            SetScreenAtribute(1, bar);
            printcu(L"NO (esc)");
            DrawScreen();

            SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
            SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

            if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
                return true;
            } else if (Key.ScanCode == SCAN_ESC) {
                return false;
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

        }
        pixels = savpixels;
        for (size_t i = 0; i < savpixels; i++) {
            bufferscreen[i] = savedbufsc[i];
        }
        DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Colors->backgroundcolor);
        FreePool(savedbufsc);

        return false;
    }


#define DRAW_TEXT_DIALOG_NO_WAIT(ImageHamdle ,SystemTable, message, color, bg) \
    ShowCenteredDialognowait(ImageHamdle, SystemTable, message, color, bg);
    EFI_STATUS 
    ShowCenteredDialognowait
    (
        EFI_HANDLE* ImageHamdle,
        EFI_SYSTEM_TABLE* SystemTable,
        CHAR16* message,
        UINTN color,
        UINTN BG
    ) 
    {
        UINTN MaxColumns, MaxRows;
        CHAR16 FullMessage[100];
        SPrint(FullMessage, sizeof(FullMessage), L"   %s   ", message);

        UINTN MsgLength = StrLen(FullMessage);
        UINTN StartColumn, StartRow;

        InitializeLib(ImageHamdle, SystemTable);

        // Obtener el tamaño de la pantalla
                // Obtener el tamaño de la pantalla
        MaxRows = (verticalResolution / 12) / Conio->atributes->size;
        MaxColumns = (horizontalResolution / 8) / Conio->atributes->size;

        // Calcular la posición inicial para centrar el mensaje
        StartColumn = (MaxColumns - MsgLength) / 2;
        StartRow = MaxRows / 2;

        // Establecer el atributo de color de fondo y texto
        gotoxy(StartColumn, StartRow - 2);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        gotoxy(StartColumn, StartRow - 1);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        // Mover el cursor a la posición inicial calculada
        gotoxy(StartColumn, StartRow);
        SetScreenAtribute(0, white);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        gotoxy(StartColumn, StartRow + 1);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        gotoxy(StartColumn, StartRow + 2);
        SetScreenAtribute(0, cyan);
        SetScreenAtribute(1, cyan);
        printc(FullMessage);

        return EFI_SUCCESS;
    }

#endif // !_KERNEL_CONSOLE_
