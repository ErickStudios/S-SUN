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
* KernelMK.h
+++*/

/*
Desciption:
    All Public kernel lib files and some functions
*/
#ifndef _KERNEL_
#define _KERNEL_

#include <efi.h>
#include <efilib.h>
#include "./services/KernelOem.h"
#include "./services/KernelBinary.h"
#include "./services/KernelConsole.h"
#include "./sources/KernelCore.h"
#include "./sources/KernelDef.h"
#include "./services/KernelMemory.h"
#include "./console/KernelTextMode.h"
#include "./services/KernelStorage.h"
#include "./drivers/KernelAlgoritm.h"
#include "./services/KernelLanguaje.h"
#include "./sources/KernelErr.h"
#include "./drivers/KernelNet.h"
#include "../neptune_api/frameworck.h"
#include "./drivers/KernelSpeaker.h"
#include "../neptune_api/sprites.h"
#include "./early/bootloader.h"

// #include "KernelDictionaries.h"

// #include "KeyBoardDriver.h"

// IF YOU DONT HAVE ECS_KRN, PAM YOU HAVE IT
#ifndef HAVE_US_ECS_KRN
#define HAVE_US_ECS_KRN 1

#define ECS_KRN HAVE_US_ECS_KRN
#endif // !ECS_KRN

#ifdef HAVE_US_ECS_KRN

#define PRIV private
#define POTENTIATOR const

#endif // HAVE_US_ECS_KRN

typedef VOID electron;

UINT16 SystemCluster;

#define IF if
#define ELIF else if
#define ELSE(b) else { b }
#define POWR EFIAPI

#define COLGATE_SYSTEM while (1);

/*
KERNEL_16

Summary
    Define KERNEL 16bit functions
*/
#ifndef KERNEL_16
#define KERNEL_16

    typedef 
        CHAR16
    neutron16
    ;
    typedef
        INT16
    proton16
    ;

#endif // !KERNEL_16

/*
KERNEL_32

Summary
    Define KERNEL 32bit functions
*/
#ifndef KERNEL_32
    #define KERNEL_32

    typedef
        INT32
    proton32
    ;

    typedef 
        proton32
    KERNEL32_status
    ;

    typedef 
        proton32
    KERNEL32_error
    ;

    typedef
        electron
    function
    ;

    typedef
        CHAR16
    TEXT
    ;

    typedef
        VOID
    MINI_PROGRAM
    ;

    typedef
        EFI_STATUS
    PROGRAM
    ;

    typedef
        PROGRAM
     GAME    
    ;

    // el MINI_PROGRAM es una funcion tan buena, ojala sacaran un MINI_MINI_PROGRAM-

    typedef
        INT8
    MINI_MINI_PROGRAM
   ; // OK me callo

    VOID
        InitializeKernel
        (
            EFI_HANDLE ImageHandle,
            EFI_SYSTEM_TABLE * SystemTable
        )
    {
        PrintWithStatus(TranslateWorck(&IS_STABLE_VER_TRANS,languajecu), (REALESE) ? L"\x2a7" : L"X");

        // get if audit boot
        AuditMode = LibGetVariable(
            L"Installed", // the installed indicator
            &SmallVariables // the guid
        );

        if (
            AuditMode == 0 // ha is inverted
            )
        {
            SetScreenAtribute(1, black);

            // ask to the user about of lang
            SetScreenAtribute(0, brgreen);
            printc(L"en\nes\nfr\n  ?:");
            SetScreenAtribute(0, brblue);
            languajecu = ReadLineSeriusWorck();

            CHAR16 e[512];

            SPrint(e, sizeof(e), L"%s %g", TranslateWorck(&SETUPCONF_MSG_TRANS, languajecu), &SmallVariables);

            BOOLEAN confirm = DRAW_DIALOG_MSG_CONFIRM(ImageHandle,SystemTable,e,gray,black);

            if (!confirm)
            { 
                DRAW_DIALOG_MSG(ImageHandle, SystemTable, L"error: user cancelled", gray, black);
                globalsystemtable->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL); \
            }

            LibSetNVVariable(
                L"Installed", // the installed indicator
                &SmallVariables, // the guid
                sizeof(BOOLEAN),
                TRUE
            );
        }
    }

#endif // !KERNEL_32

#endif // !_KERNEL_
