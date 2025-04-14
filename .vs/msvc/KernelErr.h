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
* KernelErr.h
+++*/

#include <efi.h>
#include <efilib.h>

#ifndef _KERNEL_ERR_
#define _KERNEL_ERR_

#define IF if
#define ELIF else if
#define ELSE(a) else { a }

// ** Errors **

// when the Time is OUT
#define ERR_TimeOut 0x001

// when the parammeter is invalid
#define ERR_Invalid 0x002

// when the program launchs a throw
#define ERR_Throw 0x003

// when the system cant make a action that is out of his limits
#define ERR_OutOfResources 0x004

// when the buffer is too filled to write in he
#define ERR_filledbuffer 0x005

// when the system cant manage the buffer
#define ERR_invalid_buffer 0x006

// when the system cant read the buffer
#define ERR_bad_buffer 0x007

// when the buffer is so SUS (suspicious) for the system
// and the systems looks it with a -_O face
#define ERR_mad_buffer 0x008

#define KERNEL_ERROR 23

typedef enum {
    KernelSucces = 0,
    kTimeOut = (ERR_TimeOut + KERNEL_ERROR),
    kInvalid = (ERR_Invalid + KERNEL_ERROR),
    kThrow = (ERR_Throw + KERNEL_ERROR),
    kOutOfResources = (ERR_OutOfResources + KERNEL_ERROR),
    kFilledBuffer = (ERR_filledbuffer + KERNEL_ERROR),
    kInvalidBuffer = (ERR_invalid_buffer + KERNEL_ERROR),
    kBadBuffer = (ERR_bad_buffer + KERNEL_ERROR),
    kMadBuffer = (ERR_mad_buffer + KERNEL_ERROR),
} KERNEL_STATUS;

BOOLEAN
KernelERR
(
    KERNEL_STATUS* This
)
{
    if (*This > KERNEL_ERROR) {
        return TRUE;
    }
    return FALSE; // No hay error
}

KERNEL_STATUS
SituationToError
(
    CHAR16* Situation
)
{
    if (StriCmp(Situation, L"Everthing is fine") == 0) {
        return KernelSucces;
    }
    else if (StriCmp(Situation, L"My time is over") == 0) {
        return kTimeOut;
    }
    else if (StriCmp(Situation, L"Why the thing that i want to manage i cant got it") == 0) {
        return kInvalid;
    }
    else if (StriCmp(Situation, L"Why that stop") == 0) {
        return kThrow;
    }
    else if (StriCmp(Situation, L"i am hurt") == 0) {
        return kOutOfResources;
    }
    else if (StriCmp(Situation, L"i can put more water") == 0) {
        return kFilledBuffer;
    }
    else if (StriCmp(Situation, L"i can read this book") == 0) {
        return kInvalidBuffer;
    }
    else if (StriCmp(Situation, L"no he scape from me") == 0) {
        return kBadBuffer;
    }
    else if (StriCmp(Situation, L"am system , a buffer are emm emm , i am intimided") == 0) {
        return kMadBuffer;
    }
}

VOID
ErrorToString
(
    KERNEL_STATUS ERROR,
    CHAR16* Buffer
)
{
#ifndef HAVE_US_ECS_KRN
    return L"you dont have ESC_KRN";
#endif // !HAVE_US_ECS_KRN

    // the ky
    IF(ERROR == kTimeOut) {
        *Buffer = L"TimeOut , try agin";
    }
    ELIF(ERROR == kInvalid) {
        *Buffer = L"invalid paramter";
    }
    ELIF(ERROR == kThrow) {
        *Buffer = L"has ocurren an throw error";
    }
    ELIF(ERROR == kOutOfResources) {
        *Buffer = L"the system cant use a action that is out of limits of system";
    }
    ELIF(ERROR == kFilledBuffer) {
        *Buffer = L"you cant write in a buffer that is filled";
    }
    ELIF(ERROR == kInvalidBuffer) {
        *Buffer = L"invalid buffer";
    }
    ELIF(ERROR == kBadBuffer) {
        *Buffer = L"the system can conect with the buffer";
    }
    ELIF(ERROR == kMadBuffer) {
        *Buffer = L"the buffer is so mad for the system -_O";
    }
    ELSE(*Buffer = L"invalid error";);
}
#endif // !_KERNEL_ERR_
