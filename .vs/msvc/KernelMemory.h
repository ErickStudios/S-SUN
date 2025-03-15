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
* KernelMemory.h
+++*/

#include "KernelDef.h"

#ifndef _KERNEL_MEMORY_
#define _KERNEL_MEMORY_

#define MAX_MEM_FILES 100

#define MEM_FILE_INT MEMORY_FILE_INT_STRUCTURE
#define MEM_FILE_STRING MEMORY_FILE_STRING_STRUCTURE

/////// Ahy , NO hay ñ :(

// 
// MEM_FILE_INT (MEMORY_FILE_INT_STRUCTURE)
/**
Summary:
    this defines a structure for creation and edition of a ints files in memory using a MoTo (MoutTo)
**/

typedef struct
MEMORY_FILE_INT_STRUCTURE {
    string*
        NAME;
    int*
        VALUE;
} MEMORY_FILE_INT_STRUCTURE;

// MEM_FILE_STRING (MEMORY_FILE_STRING_STRUCTURE)
/**
Summary:
    this defines a structure for creation and edition of a strings files in memory using a MoTo (MoutTo)
**/
typedef struct
MEMORY_FILE_STRING_STRUCTURE {
    string*
        NAME;
    string*
        VALUE;
} MEMORY_FILE_STRING_STRUCTURE;

MEM_FILE_STRING* MemFilesString[MAX_MEM_FILES];
MEM_FILE_INT* MemFilesInt[MAX_MEM_FILES];

UINTN MemFileCountString = 0;
UINTN MemFileCountInt = 0;

// Create_MEM_FILE_STRING
/**
Summary:
    this creates a MEM file as string
**/
MEM_FILE_STRING*
Create_MEM_FILE_STRING
(
    string* Name,
    string* Value
)
{
    if (MemFileCountString >= MAX_MEM_FILES) {
        return NULL;
    }

    MEM_FILE_STRING* MemFile = (MEM_FILE_STRING*)AllocatePool(sizeof(MEM_FILE_STRING));
    if (MemFile == NULL) {
        return NULL;
    }

    MemFile->NAME = StrDuplicate(Name);  // Asegúrate de duplicar el nombre
    MemFile->VALUE = StrDuplicate(Value);  // Asegúrate de duplicar el valor

    MemFilesString[MemFileCountString++] = MemFile;

    return MemFile;
}

// Create_MEM_FILE_INT
/**
Summary:
    this creates a MEM file as int
**/
MEM_FILE_INT*
Create_MEM_FILE_INT
(
    string* Name,
    int* Value
)
{
    if (MemFileCountInt >= MAX_MEM_FILES) {
        return NULL;
    }

    MEM_FILE_INT* MemFile = (MEM_FILE_INT*)AllocatePool(sizeof(MEM_FILE_INT));
    if (MemFile == NULL) {
        return NULL;
    }

    MemFile->NAME = StrDuplicate(Name);  // Asegúrate de duplicar el nombre
    MemFile->VALUE = Value;

    MemFilesInt[MemFileCountInt++] = MemFile;

    return MemFile;
}

// Edit_MEM_FILE_STRING
/**
Summary:
    this edits the value of a MEM file string
**/
EFI_STATUS
Edit_MEM_FILE_STRING
(
    string* Name,
    string NewValue
)
{
    for (UINTN i = 0; i < MemFileCountString; i++) {
        if (StrCmp(MemFilesString[i]->NAME, Name) == 0) {
            MemFilesString[i]->VALUE = NewValue;
            return EFI_SUCCESS;
        }
    }
    return EFI_NOT_FOUND;
}

// Edit_MEM_FILE_INT
/**
Summary:
    this edits the value of a MEM file int
**/
EFI_STATUS
Edit_MEM_FILE_INT
(
    string* Name,
    int NewValue
)
{
    for (UINTN i = 0; i < MemFileCountInt; i++) {
        if (StrCmp(MemFilesInt[i]->NAME, Name) == 0) {
            MemFilesInt[i]->VALUE = NewValue;
            return EFI_SUCCESS;
        }
    }
    return EFI_NOT_FOUND;
}

// Read_MEM_FILE_STRING
/**
Summary:
    this reads the value of a MEM file string
**/
string
Read_MEM_FILE_STRING
(
    string* Name
)
{
    for (UINTN i = 0; i < MemFileCountString; i++) {
        if (StrCmp(MemFilesString[i]->NAME, Name) == 0) {
            return MemFilesString[i]->VALUE;
        }
    }
    return NULL;
}

// Read_MEM_FILE_INT
/**
Summary:
    this reads the value of a MEM file int
**/
int
Read_MEM_FILE_INT
(
    string* Name
)
{
    for (UINTN i = 0; i < MemFileCountInt; i++) {
        if (StrCmp(MemFilesInt[i]->NAME, Name) == 0) {
            return MemFilesInt[i]->VALUE;
        }
    }
    return -1;
}

// Free_MEM_FILE_STRING
/**
Summary:
    this frees the memory allocated for a MEM file string
**/
void
Free_MEM_FILE_STRING
(
    MEM_FILE_STRING* MemFile
)
{
    if (MemFile != NULL) {
        FreePool(MemFile);
    }
}

// Free_MEM_FILE_INT
/**
Summary:
    this frees the memory allocated for a MEM file int
**/
void
Free_MEM_FILE_INT
(
    MEM_FILE_INT* MemFile
)
{
    if (MemFile != NULL) {
        FreePool(MemFile);
    }
}

// Flush_MEM_FILE_STRING
/**
Summary:
    this frees the memory allocated for a MEM file string from a only the name
**/
void
Flush_MEM_FILE_STRING
(
    string* Name
)
{
    for (UINTN i = 0; i < MemFileCountString; i++) {
        if (StrCmp(MemFilesString[i]->NAME, Name) == 0) {
            MemFilesString[i]->VALUE = L"[DELETED]";
            FreePool(MemFilesString[i]);
        }
    }

    pr_special(L"the memory is not exist");
}

// Flush_MEM_FILE_INT
/**
Summary:
    this frees the memory allocated for a MEM file int from a only the name
**/
void
Flush_MEM_FILE_INT
(
    string* Name
)
{
    for (UINTN i = 0; i < MemFileCountString; i++) {
        if (StrCmp(MemFilesInt[i]->NAME, Name) == 0) {
            MemFilesInt[i]->VALUE = 0xfff;
            FreePool(MemFilesInt[i]);
        }
    }

    pr_special(L"the int mem file is not exist");
}

#endif