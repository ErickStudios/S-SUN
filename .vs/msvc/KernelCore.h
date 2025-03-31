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
* KernelCore.h
+++*/
#include <efi.h>
#include <efilib.h>

#include "KernelDef.h"

// S-SUN kernel core
#ifndef _KERNEL_CORE_
#define _KERNEL_CORE_

#define dift(n,v) UINTN (n) = (v);

// System Resources
#define kERNEL_RESOURCE     1
#define DISPLAY_RESOURCE    2
#define KEYBOARD_RESOURCE   3
#define RAM_RESOURCE        4
#define CONIO_RESOURCE      5

// Interruption codes
#define INT_SUCCES                  1
#define INT_ERROR                   2
#define INT_NOT_FOUNDED             3
#define INT_RESOURCE_NOT_FOUNDED    4

int SystemResources[] = {
    kERNEL_RESOURCE ,
    DISPLAY_RESOURCE ,
    KEYBOARD_RESOURCE,
    RAM_RESOURCE,
    CONIO_RESOURCE
};

// interruptions
/**
Summary:
    this method calls a specific S-SUN resource and specific InterruptionID in S++
**/
/*
int
Interruption
(
    int Resource,
    int InterruptionID
)
{
    switch (Resource)
    {
    case kERNEL_RESOURCE:
        switch (InterruptionID)
        {
        case 0x01:
            return INT_SUCCES;
        default:
            return INT_NOT_FOUNDED;
        }
    case DISPLAY_RESOURCE:
        switch (InterruptionID)
        {
        case 0x01:
            DRW_X++;
            return INT_SUCCES;
        case 0x02:
            DRW_Y++;
            return INT_SUCCES;
        case 0x03:
            DRW_DOWN;
            return INT_SUCCES;
        case 0x04:
            DRW_COL.Red = Read_MEM_FILE_INT(L"Interruption_DISPLAY_val");
            return INT_SUCCES;
        case 0x05:
            DRW_COL.Green = Read_MEM_FILE_INT(L"Interruption_DISPLAY_val");
            return INT_SUCCES;
        case 0x06:
            DRW_COL.Blue = Read_MEM_FILE_INT(L"Interruption_DISPLAY_val");
            return INT_SUCCES;
        default:
            return INT_NOT_FOUNDED;
        }
    case KEYBOARD_RESOURCE:
        switch (InterruptionID)
        {
        case 0x01:
            return INT_SUCCES;
        default:
            return INT_NOT_FOUNDED;
        }
    default:
        return INT_RESOURCE_NOT_FOUNDED;
    }
}
*/

EFI_STATUS CreateANewDirFUNCTION(CHAR16* DirName, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);

/*
* usage of the KERNEL_CALLS
* 1. fordepure : this featured is not make but is for make a call for indicate of the depuration
* 2. forgetramvariable : this featured can get a value of ram variable
* 3. forsetramvariable : this feature can set a ram variable
*/
enum _KERNEL_REASON_FOR_CALL_ {
    fordepure = 0,
    forgetramvariable = 1,
    forsetramvariable = 2,
};

#define KERNEL_CALL(call_reason, data) \
    kernel_call_handler(call_reason, data)

// Estructura para las variables del kernel
typedef struct {
    CHAR16* name;
    CHAR16* value;
} KernelVariable;

// Array de variables del kernel (simplificado)
KernelVariable kernel_variables[10];
UINTN kernel_variable_count = 0;

void set_kernel_variable(CHAR16* name, CHAR16* value) {
    for (UINTN i = 0; i < kernel_variable_count; i++) {
        if (StrCmp(kernel_variables[i].name, name) == 0) {
            kernel_variables[i].value = value;
            return;
        }
    }
    kernel_variables[kernel_variable_count].name = name;
    kernel_variables[kernel_variable_count].value = value;
    kernel_variable_count++;
}

CHAR16* get_kernel_variable(CHAR16* name) {
    for (UINTN i = 0; i < kernel_variable_count; i++) {
        if (StrCmp(kernel_variables[i].name, name) == 0) {
            return kernel_variables[i].value;
        }
    }
    return NULL;
}

CHAR16* split(CHAR16* str, CHAR16 delimiter, UINTN* count) {
    static CHAR16 tokens[10][20]; // Asegúrate de ajustar el tamaño según tus necesidades
    CHAR16* start = str;
    UINTN tokenIndex = 0, charIndex = 0;

    while (*str != L'\0' && tokenIndex < 10) {
        if (*str == delimiter) {
            tokens[tokenIndex][charIndex] = L'\0'; // Finalizar token actual
            tokenIndex++;
            charIndex = 0;
        }
        else {
            tokens[tokenIndex][charIndex++] = *str;
        }
        str++;
    }

    tokens[tokenIndex][charIndex] = L'\0'; // Finalizar último token
    *count = tokenIndex + 1;
    return (CHAR16*)tokens;
}

CHAR16* kernel_call_handler(enum _KERNEL_REASON_FOR_CALL_ reason, CHAR16* data) {
    switch (reason) {
    case fordepure:
#if defined(_DEBUG)
        return L"true";
#else
        return L"false";
#endif
        break;
    case forgetramvariable:
        return get_kernel_variable(data);
    case forsetramvariable: {
        UINTN count;
        CHAR16* datatype = split(data, L' ** ==', &count);
        if (count > 1) {
            pr_special(L"kernel variable seted operation\n");
            printc(datatype[0]);
            printc(L" = ");
            printc(datatype[1]);
            printc(L"\n");
            set_kernel_variable(datatype[0], datatype[1]);
        }
        break;
    }
    default:
        break;
    }
    return L"FINISHED";
}

#define KERNEL_GET_SIZE_LINES(Image_Handle, SystemTable) \
    GetLinesNum(Image_Handle, SystemTable)

UINTN GetLinesNum(EFI_HANDLE* Image_Handle, EFI_SYSTEM_TABLE* SystemTable) {
    InitializeLib(Image_Handle, SystemTable);
    UINTN MaxColumns, MaxRows;

    // Obtener el tamaño de la pantalla
    SystemTable->ConOut->QueryMode(SystemTable->ConOut, SystemTable->ConOut->Mode->Mode, &MaxColumns, &MaxRows);
    return MaxRows;
}

#define KERNEL_GET_TIME(SystemTable) \
    GetCurrentTime(SystemTable)
CHAR16* GetCurrentTime(EFI_SYSTEM_TABLE* SystemTable) {
    EFI_TIME Time;
    EFI_STATUS Status;
    static CHAR16 Buffer[80];

    // Obtener la hora y la fecha actual
    Status = SystemTable->RuntimeServices->GetTime(&Time, NULL);
    if (EFI_ERROR(Status)) {
        StrCpy(Buffer, L"Error al obtener la hora y la fecha.");
        return Buffer;
    }

    // Formatear la hora en una cadena CHAR16*
    SPrint(Buffer, sizeof(Buffer), L"%02u : %02u : %02u", Time.Hour, Time.Minute, Time.Second);

    // Devolver la cadena formateada
    return Buffer;
}

#define KERNEL_GET_DATE(SystemTable) \
    GetCurrentDate(SystemTable)
CHAR16* GetCurrentDate(EFI_SYSTEM_TABLE* SystemTable) {
    EFI_TIME Time;
    EFI_STATUS Status;
    static CHAR16 Buffer[80];

    // Obtener la hora y la fecha actual
    Status = SystemTable->RuntimeServices->GetTime(&Time, NULL);
    if (EFI_ERROR(Status)) {
        StrCpy(Buffer, L"Error al obtener la hora y la fecha.");
        return Buffer;
    }

    // Formatear la hora en una cadena CHAR16*
    SPrint(Buffer, sizeof(Buffer), L"%02u : %02u : %02u", Time.Year, Time.Month, Time.Day);

    // Devolver la cadena formateada
    return Buffer;
}

extern int _KERNEL_SYSTEM_RUNNING_ = 1;
extern int _KERNEL_SYSTEM_ERROR_ = 0;
extern int _KERNEL_CATCH_THROW_ = 0;

EFI_LOADED_IMAGE* LoadedImage;
EFI_DEVICE_PATH* DevicePath;

#define KERNEL_CURRENT_DOMAIN \
    GET_CURRENT_DOMAIN()

EFI_HANDLE GET_CURRENT_DOMAIN() {
    EFI_LOADED_IMAGE* LoadedImage;
    EFI_STATUS Status;

    // Obtener la información de la imagen cargada
    Status = uefi_call_wrapper(globalsystemtable->BootServices->HandleProtocol, 3, globalimagehandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"error : cannot get loaded image protocol\n");
        return NULL;
    }

    // Obtener el manejador del dispositivo
    return LoadedImage->DeviceHandle;
}

#define THROW_NULLSTRING 0x0001
#define THROW_CANDIVIDEBYCERO 0x0002
#define THROW_RECURTION 0x0003
#define THROW_NULL_CHAIN_WHEN_NEEDS_TO_PUT_ANYTHING 0x0004
#define THROW_NULL THROW_NULL_CHAIN_WHEN_NEEDS_TO_PUT_ANYTHING

#define THROW_ERROR(EXCEPT) \
    do { \
        if ((_KERNEL_CATCH_THROW_) == 0) { \
            _KERNEL_SYSTEM_RUNNING_ = 0; \
            _KERNEL_SYSTEM_ERROR_ = EXCEPT; \
        } else if ((_KERNEL_CATCH_THROW_) == 1) { \
            _KERNEL_SYSTEM_RUNNING_ = 1; \
        } else { \
            _KERNEL_SYSTEM_RUNNING_ = 0; \
            _KERNEL_SYSTEM_ERROR_ = EXCEPT; \
        } \
    } while (0)
/*
    this block execute a action while the system is running
    when the system shutdown this is not execute
    if the system crash this shows a error screen
*/
#define WHILESYSTEMRUNNING(BLOCKS) \
    do { \
        InitializeLib(globalimagehandle, globalsystemtable); \
        if (_KERNEL_SYSTEM_RUNNING_ == 1) { \
                BLOCKS \
        } else { \
            if (_KERNEL_SYSTEM_ERROR_ == 0) { \
                globalsystemtable->RuntimeServices->ResetSystem(EfiResetShutdown , EFI_SUCCESS, 0, NULL); \
            } else { \
                ClearScreen(); \
                printc(L"The system has encountered a problem and needs to restart.\n"); \
                printc(int_to_hex_string(_KERNEL_SYSTEM_ERROR_)); \
                uefi_call_wrapper(globalsystemtable->BootServices->Stall, 1, 500000); \
                globalsystemtable->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL); \
            } \
        } \
    } while (0)

#define KERNEL_INITIALIZE \
    pr_msg(L"setting the variable SUN_INTERNAL_KERNEL_INITIALIZED"); \
    uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000); \
    KERNEL_CALL(forsetramvariable, L"S-SUN_INTERNAL_KERNEL_INITIALIZED ** == 1"); \
    pr_special(L"the variable SUN_INTERNAL_KERNEL_INITIALIZED is setted as 1"); \
    uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000); \
    pr_msg(L"setting the variable SUN_INTERNAL_KERNEL_OS_VERSION"); \
    uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000); \
    KERNEL_CALL(forsetramvariable, L"S-SUN_INTERNAL_KERNEL_OS_VERSION ** == alpha 0.1"); \
    pr_special(L"the variable SUN_INTERNAL_KERNEL_OS_VERSION is setted as alpha 0.1"); \
    uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000); \
    pr_msg(L"setting the variable SUN_INTERNAL_KERNEL_MAX_SPACE"); \
    uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000); \
    KERNEL_CALL(forsetramvariable, L"S-SUN_INTERNAL_KERNEL_MAX_SPACE ** == 2048"); \
    pr_special(L"the variable SUN_INTERNAL_KERNEL_MAX_SPACE is setted as 2048"); \
    uefi_call_wrapper(globalsystemtable->BootServices->Stall, 0.01, 100000);

#define KERNEL_VERSION \
    KERNEL_CALL(forgetramvariable, L"SUN_INTERNAL_KERNEL_OS_VERSION")

#define SYSTEM_ON _KERNEL_SYSTEM_RUNNING_ = 1;
#define SYSTEM_OFF _KERNEL_SYSTEM_RUNNING_ = 0;

void
KERNELCALL_CREATEDIR_PARAMS
(
    string *DirName,
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
{
    string* msg[100];
        InitializeLib(ImageHandle,SystemTable);
        EFI_STATUS Status;
        SPrint(msg, sizeof(msg), L"try to create/write the file/directory %s", DirName);
        pr_msg(msg);
        uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
        Status = CreateANewDirFUNCTION(DirName, ImageHandle, SystemTable);
        if (EFIWARN(Status)) {
            SPrint(msg, sizeof(msg), L"the system catchs a WARNNING: %r", Status);
            pr_warn(msg);
            uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
        }
        if (EFI_ERROR(Status)) {
            SPrint(msg, sizeof(msg), L"the system catchs a ECXEPTION");
            pr_warn(msg);
            uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
        }
        SPrint(msg, sizeof(msg), L"Status on create the file/directory %d: %r", DirName, Status);
        pr_special(msg);
        uefi_call_wrapper(SystemTable->BootServices->Stall, 0.01, 100000);
}

void
KERNELCALL_CREATEDIR
(
    string* DirName
)
{
    KERNELCALL_CREATEDIR_PARAMS(DirName,globalimagehandle,globalsystemtable);
}

#endif // !_KERNEL_CORE_