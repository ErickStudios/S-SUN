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
* KenelStorage.h
+++*/
#include <efi.h>
#include <efilib.h>

// S-SUN kernel textmode
#ifndef _KERNEL_STORAGE_
#define _KERNEL_STORAGE_

#define FILint(x,n) \
UINTN x = n;

#define FileDef(name,paath,content,direction) \
    HEY_CURRENT_SESSION[direction].Name = name; \
    HEY_CURRENT_SESSION[direction].path = paath; \
    HEY_CURRENT_SESSION[direction].Content = content; \
    HEY_CURRENT_SESSION[direction].Direction = direction; \
    if ( FilesCount < direction) { FilesCount = direction; }

#define BasicStorageMethod \
    FileName* \
        Name; \
    Path \
        path; \
    FILE_CONTENT* \
        Content;

typedef UINTN    MEMORY_DIRECTION;

UINTN FilesCount = 0;

typedef CHAR16    FileName;
typedef CHAR16    Path;

typedef CHAR16    FILE_CONTENT;

typedef MEMORY_DIRECTION    MEM_DIR;

CHAR16 FOLDER_CONTENT;

typedef struct {
    BasicStorageMethod
    MEM_DIR
        Direction;
} File;

typedef File    FileSystem;

FileSystem
HEY_CURRENT_SESSION[200];

// Peparar los archivos
VOID
InitializeFileSystem
()
{
    FOLDER_CONTENT = L"$FOLDER #_VEFI";
    FileDef(L"\\", L"", FOLDER_CONTENT, 0);
    FileDef(L"System", L"\\", FOLDER_CONTENT, 1);
    FileDef(L"User", L"\\", FOLDER_CONTENT, 2);
    FileDef(L"Init.spp", L"\\", L"EditMem S-SUN_CREATOR=ErickCraftStudios\nEditMem S-SUN_KERNEL_CODENAME=KSUN\nEditMem S-SUN_MadedIn=C\n\ncls\necho Please Wait , Loading Desktop", 3);
}

#endif // !_KERNEL_STORAGE_