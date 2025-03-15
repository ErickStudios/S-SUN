
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

#include <efi.h>
#include <efilib.h>

#include "KernelTextMode.h"
#include "kernelStorage.h"

/*+++
* KernelDef.h
+++*/
#ifndef _KERNEL_DEFS_
#define _KERNEL_DEFS_

EFI_HANDLE globalimagehandle;
EFI_SYSTEM_TABLE* globalsystemtable;

// define aliases 
#define CSCHEME _SYSTEM_COLOR_SCHEME
#define KINSTANCE _KERNEL_INSTANCE
#define _SYSI _SYSTEM_INSTANCE_FOR_MANAGER
#define HKEY _REGISTER_KEY_DATA
#define HFOLDER _REGISTER_FOLDER_WITH_REGS

// for c 
#define int UINTN
#define void VOID
#define string CHAR16
#define bool BOOLEAN
#define true TRUE
#define false FALSE

// StartsWish
/**
Summary:
    this functions compares a text with other 
    text and returns true or false if the text 
    starts with text2
Usage:
    if (StartsWhish(input, L"h")) {
        return L"hello";
    } else {
        return L"huh";
    }
**/
bool
StartsWish
(
    IN string *text,
    IN string *text2
)
{
    if (StrnCmp(text, text2, StrLen(text2)) == 0) { return true; }
    else { return false; }
}

string
ConvertChar8ToChar16
(
    CHAR8 ch
)
{
    if (ch <= 127) {
        return (CHAR16)ch;
    }
    else {
        return '?'; // Carácter no representable en CHAR8
    }
}

// Función de conversión de CHAR16 a CHAR8
string
ConvertChar16ToChar8
(
    CHAR16 ch
)
{
    if (ch <= 0xFF) {
        return (CHAR8)ch;
    }
    else {
        return '?'; // Carácter no representable en CHAR8
    }
}

// CSCHEME (_SYSTEM_COLOR_SCHEME)
/**
Summary:
    this defines a structure that can
    aply by system for colors scheme
**/

typedef struct 
_SYSTEM_COLOR_SCHEME {
    PIXELCOL 
        backgroundcolor;
    PIXELCOL
        buttonscolor;
    PIXELCOL
        buttonstext;
} 
_SYSTEM_COLOR_SCHEME;

// CSCHEME (_KERNEL_INSTANCE)
/**
Summary:
    this defines a structure for manage a kernel start
**/
typedef struct 
_KERNEL_INSTANCE {
    int 
        ID;
    int 
        DISPLAYSX;
    int 
        DISPLAYSY;
} 
_KERNEL_INSTANCE;

// HKEY (_REGISTER_KEY_DATA)
/**
Summary:
    this defines a structure for the local keys
**/
typedef struct
_REGISTER_KEY_DATA {
    string*
        Name;
    string*
        path;
    int
        type;
    void*
        value;
}
_REGISTER_KEY_DATA;

// _SYSI (_SYSTEM_INSTANCE_FOR_MANAGER)
/**
Summary:
    this defines a system instance for be managen by 
    the kernel
**/
typedef struct 
_SYSTEM_INSTANCE_FOR_MANAGER {
    KINSTANCE 
        kernel;
    CSCHEME 
        COLORS;
    string
        Vendor;
    int
        revision;
    File
        HEY_CURRENT_SESSION[200];
} 
_SYSTEM_INSTANCE_FOR_MANAGER;

// newCSCHEME
/**
Summary:
    this makes a scheme with a simple thins
Usage:
    CSCHEME* aqua newCSCHEME(EFI_BACKGROUND_BLUE, EFI_CYAN, EFI_BLACK);
**/
CSCHEME*
newCSCHEME
(
    PIXELCOL bg,
    PIXELCOL buttons,
    PIXELCOL btext
)
{
    CSCHEME* scheme = (CSCHEME*)AllocatePool(sizeof(CSCHEME));
    if (scheme == NULL) {
        return NULL;
    }
    scheme->backgroundcolor = bg;
    scheme->buttonscolor = buttons;
    scheme->buttonstext = btext;

    return scheme;
}

// editSCHEME
/**
Summary:
    this makes a scheme with a simple thins
Usage:
    CSCHEME* aqua newCSCHEME(EFI_BACKGROUND_BLUE, EFI_CYAN, EFI_BLACK);
    aqua = editSCHEME(aqua, 1, EFI_BACKGROUND_BROWN);
**/
CSCHEME*
editSCHEME
(
    CSCHEME* scheme,
    int param,
    PIXELCOL newvalue
)
{
    if (param == 1) {
        scheme->backgroundcolor = newvalue;
    }
    else if (param == 2) {
        scheme->buttonscolor = newvalue;
    }
    else if (param == 3) {
        scheme->buttonstext = newvalue;
    }

    return scheme;
}

// CreateHKEY
/**
Summary:
    this creates a scheme in the system ram
**/
void
CreateHKEY
(
    HKEY* keyData
) 
{
    EFI_STATUS Status;
    EFI_GUID MyVendorGuid = { 0xaabbccdd, 0xeeff, 0x1122, { 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00 } };

    if (keyData->type == 1) {
        // Supongamos que type 1 significa un valor de cadena
        Status = gRT->SetVariable(
            keyData->Name,
            &MyVendorGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
            StrLen(keyData->value) * sizeof(CHAR16),
            keyData->value
        );
    }
    else if (keyData->type == 2) {
        // Manejo de otro tipo de datos
    }

    if (EFI_ERROR(Status)) {
        Print(L"Error al crear la clave HKEY: %r\n", Status);
    }
    else {
    }
}

// pr_warn
/**
Summary:
    this prints a warn in the con
**/
void
pr_warn
(
    string* e
)
{
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    SetScreenAtribute(0, brgreen);
    printc(L"*** KERNEL_CALL -> -> : ");
    SetScreenAtribute(0, orange);
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    printc(e);
    printc(L"\n");
}

// pr_msg
/**
Summary:
    this prints a kernel messaje
**/
void
pr_msg
(
    string* e
)
{
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    SetScreenAtribute(0, brgreen);
    printc(L"*** KERNEL_CALL -> -> : ");
    SetScreenAtribute(0, white);
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    printc(e);
    printc(L"\n");

}

// pr_special
/**
Summary:
    this prints a kernel special messaje
**/
void
pr_special
(
    string* e
)
{
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_GREEN | EFI_BACKGROUND_BLACK);
    SetScreenAtribute(0, brgreen);
    printc(L"*** KERNEL_CALL -> -> : ");
    SetScreenAtribute(0, brblue);
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    printc(e);
    printc(L"\n");

}

// int_to_hex_string
/**
Summary:
    this converts a int number to a hex string
**/
string
int_to_hex_string
(
    int number
)
{
    CHAR16 HexString[512];
    SPrint(HexString, sizeof(HexString), L"%x", number);
    return HexString;
}
#endif // !_KERNEL_DEFS_