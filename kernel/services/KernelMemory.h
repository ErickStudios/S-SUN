/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

    KernelMemory.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

    the kernel memory managent

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include "../licensesafety.h" // before make me anything , Tianocore , please view this file

#include "../sources/KernelDef.h"

#ifndef _KERNEL_MEMORY_
#define _KERNEL_MEMORY_

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// constants

#define DELETED_CONTENT                 L"?"
#define MAX_MEM_FILES                   200
#define MEM_FILE_INT                    MEMORY_FILE_INT_STRUCTURE
#define MEM_FILE_STRING                 MEMORY_FILE_STRING_STRUCTURE
#define CRT_FINT(intmem,name,value)     intmem = Create_MEM_FILE_INT(name,value)
#define CRT_FSTRING(stringmem,name,value) stringmem = Create_MEM_FILE_STRING(name,value)

// ----------------------------------------------------------------------------------------------
// structures

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
    int
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

// ----------------------------------------------------------------------------------------------
// variables
MEM_FILE_STRING*                        MemFilesString[MAX_MEM_FILES];
MEM_FILE_INT*                           MemFilesInt[MAX_MEM_FILES];
UINTN                                   MemFileCountString = 0;
UINTN                                   MemFileCountInt = 0;

// ----------------------------------------------------------------------------------------------
// functions

prototype MEM_FILE_STRING*
Create_MEM_FILE_STRING
(
    string*                             Name,
    string*                             Value
);

prototype MEM_FILE_INT*
Create_MEM_FILE_INT
(
    string*                             Name,
    int*                                Value
);

prototype EFI_STATUS
Edit_MEM_FILE_STRING
(
    string*                             Name,
    string*                             NewValue
);

prototype EFI_STATUS
Edit_MEM_FILE_INT
(
    string*                             Name,
    int                                 NewValue
);

prototype string
Read_MEM_FILE_STRING
(
    string*                             Name
);

prototype int
Read_MEM_FILE_INT
(
    string*                             Name
);

prototype bool
Flush_MEM_FILE_STRING
(
    string*                             Name
);

prototype bool
Flush_MEM_FILE_INT
(
    string*                             Name
);

prototype void
Free_MEM_FILE_STRING
(
    MEM_FILE_STRING*                    MemFile
);

prototype void
Free_MEM_FILE_INT
(
    MEM_FILE_INT*                       MemFile
);

prototype VOID
InitializeMems
(
);

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

    INT32 findMemID = -1;
    for (size_t i = 0; i < MemFileCountString; i++)
    {
        if (StrCmp(MemFilesString[i]->NAME, DELETED_CONTENT) == 0) findMemID = i;
        if (StrCmp(MemFilesInt[i]->NAME, StrDuplicate(Name)) == 0)
        {
            MemFilesInt[i]->VALUE = StrDuplicate(Value);
            MEM_FILE_STRING* MemFile = (MEM_FILE_STRING*)AllocatePool(sizeof(MEM_FILE_STRING));
            MemFile->NAME = StrDuplicate(Name);  // Aseg�rate de duplicar el nombre
            MemFile->VALUE = StrDuplicate(Value);  // Aseg�rate de duplicar el valor

            CHAR16 e[200];

            SPrint(e, sizeof(e), L"%s %s", TranslateWorck(&CRTMEM_LOG_MSG_TRANS, languajecu), MemFile->NAME);

            PrintWithStatus(e, (MemFile == NULL ? L"Failed" : L"\x2a7"));         
        }
    }

    MEM_FILE_STRING* MemFile = (MEM_FILE_STRING*)AllocatePool(sizeof(MEM_FILE_STRING));

    if (MemFile != NULL) {

        MemFile->NAME = StrDuplicate(Name);  // Aseg�rate de duplicar el nombre
        MemFile->VALUE = StrDuplicate(Value);  // Aseg�rate de duplicar el valor
    }

    if (findMemID != -1) {
        MemFilesString[findMemID] = MemFile;
    }
    else
    {
        MemFilesString[MemFileCountString++] = MemFile;
    }

    if (BootStage < 2) {
        CHAR16 e[200];

        SPrint(e, sizeof(e), L"%s %s", TranslateWorck(&CRTMEM_LOG_MSG_TRANS, languajecu), MemFile->NAME);

        PrintWithStatus(e, (MemFile == NULL ? L"Failed" : L"\x2a7"));
    }
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

    INT32 findMemID = -1;
    for (size_t i = 0; i < MemFileCountInt; i++)
    {
        if (StrCmp(MemFilesInt[i]->NAME, DELETED_CONTENT) == 0) findMemID = i;
        if (StrCmp(MemFilesInt[i]->NAME, StrDuplicate(Name)) == 0)
        {
            MemFilesInt[i]->VALUE = Value;
            MEM_FILE_INT* MemFile = (MEM_FILE_INT*)AllocatePool(sizeof(MEM_FILE_INT));
            MemFile->NAME = StrDuplicate(Name);  // Aseg�rate de duplicar el nombre
            MemFile->VALUE = Value;  // Aseg�rate de duplicar el valor

            CHAR16 e[200];

            SPrint(e, sizeof(e), L"%s %s", TranslateWorck(&CRTMEM_LOG_MSG_TRANS, languajecu), MemFile->NAME);

            PrintWithStatus(e, (MemFile == NULL ? L"Failed" : L"\x2a7"));
        }
    }

    MEM_FILE_INT* MemFile = (MEM_FILE_INT*)AllocatePool(sizeof(MEM_FILE_INT));

    if (MemFile != NULL) {
        MemFile->NAME = StrDuplicate(Name);  // Aseg�rate de duplicar el nombre
        MemFile->VALUE = Value;
    }

    if (findMemID != -1) {
        MemFilesInt[findMemID] = MemFile;
    }
    else {
        if ((MemFileCountInt + 1) < MAX_MEM_FILES)
            MemFilesInt[MemFileCountInt++] = MemFile;
    }

    if (BootStage < 2) {
        CHAR16 e[200];

        SPrint(e, sizeof(e), L"%s %s", TranslateWorck(&CRTMEM_LOG_MSG_TRANS,languajecu), MemFile->NAME);

        PrintWithStatus(e, (MemFile == NULL ? L"Failed" : L"\x2a7"));
    }

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
    string* NewValue
)
{
    for (UINTN i = 0; i < MemFileCountString; i++) {
        if (StrCmp(MemFilesString[i]->NAME, Name) == 0) {
            MemFilesString[i]->VALUE = StrDuplicate(NewValue);
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
    return -100;
}

// Flush_MEM_FILE_STRING
/**
Summary:
    this frees the memory allocated for a MEM file string from a only the name
**/
bool
Flush_MEM_FILE_STRING
(
    string* Name
)
{
    for (UINTN i = 0; i < MemFileCountString; i++) {
        if (StrCmp(MemFilesString[i]->NAME, Name) == 0) {
            MemFilesString[i]->NAME = DELETED_CONTENT;
            return true;
        }
    }

    return false;
}

// Flush_MEM_FILE_INT
/**
Summary:
    this frees the memory allocated for a MEM file int from a only the name
**/
bool
Flush_MEM_FILE_INT
(
    string* Name
)
{
    for (UINTN i = 0; i < MemFileCountInt; i++) {
        if (StrCmp(MemFilesInt[i]->NAME, Name) == 0) {
            MemFilesInt[i]->NAME = DELETED_CONTENT;
            return true;
        }
    }
    return false;
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
        Flush_MEM_FILE_STRING(MemFile->NAME);
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
        Flush_MEM_FILE_INT(MemFile->NAME);
        FreePool(MemFile);
    }
}

/*
InitializeMems

Summary:
    initialize the MEMS
*/
VOID
InitializeMems
(

)
{
    if (
        BootStage < 2 // i have total control?
        ) {
        PrintWithStatus(L"Status of the Mems Int Files array", MemFilesInt == NULL ? L"No Initialized" : L"\x2a7");
        PrintWithStatus(L"Status of the Mems String Files array", MemFilesString == NULL ? L"No Initialized" : L"\x2a7");
    }
}

#endif