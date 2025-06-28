#pragma once

////// !
////// ! motor.h
////// !
////// ! the motor of the modern S-SUN types and functions, designed for a large plaze and posibility
////// ! the replace of the odd gnu-efi inpulsed motor
////// !
////// ! the file is divided into sections
////// !
////// !

////// !
////// !
////// !
////// !
////// ! include only the used by the system
////// ! obiusly i cant quit the dependence of the gnu-efi of a day from the other
////// ! so this includes is temoraly while i make the transition , but the transition dont means
////// ! that i disconect from efi , the transition is for clean the gnu-efi and make to the system
////// ! a litte of independence
////// !
////// !
////// !
////// !

#ifndef _UEFI_LITTE_IMPORTANT_DEP_
#define _UEFI_LITTE_IMPORTANT_DEP_

#define CONST
#define IN
#define OUT

#include <efi.h>
#include <efilib.h>
#include <vcruntime.h>

#endif

/// !
/// ! A NEW SECTION WILL BE START SOON
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

// ------------------------------------------------------------------------------------------
// | action                                                 | name                          |
// ------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------//

/// !
/// !
/// !
/// !
/// !
/// !
/// ! inspired by linux code types , some names be similar
/// !
/// !
/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !
/// !
/// !

#define API                                                 __cdecl

/// !
/// !
/// !
/// !
/// !
/// !


typedef char                                                t8;
typedef unsigned char                                       u8;

/// !
/// !
/// !
/// !
/// !
/// !


typedef short                                               t16;
typedef unsigned short                                      u16;

/// !
/// !
/// !
/// !
/// !
/// !


typedef long                                                t32;
typedef unsigned long                                       u32;

/// !
/// !
/// !
/// !
/// !
/// !


typedef long long                                           t64;
typedef unsigned long long                                  u64;

/// !
/// !
/// !
/// !
/// !
/// !


typedef float                                               f32;
typedef double                                              f64;

/// !
/// !
/// !
/// !
/// !
/// !


typedef void*                                               objet_t;

/// !
/// !
/// !
/// !
/// !
/// !


typedef struct {
    u8                                                      Red;
    u8                                                      Green;
    u8                                                      Blue;
} color_t;

/// !
/// !
/// !
/// !
/// !
/// !


typedef wchar_t                                             ch16;

/// !
/// !
/// !
/// !
/// !
/// !


typedef t8                                                  chascii;

/// !
/// !
/// !
/// !
/// !
/// !


typedef u8                                                  ch8;

/// !
/// !
/// !
/// !
/// !
/// !


typedef u64                                                 size_t;

/// !
/// !
/// !
/// !
/// !
/// !


typedef ch8                                                 char8_t;

/// !
/// !
/// !
/// !
/// !
/// !


typedef u16                                                 CHAR16;

/// !
/// !
/// !
/// !
/// !
/// !


typedef u16                                                 string;

/// !
/// !
/// !
/// !
/// !
/// !


typedef u32                                                 CHAR32;

/// !
/// !
/// !
/// !
/// !
/// !


typedef u32                                                long_t;


/// !
/// !
/// !
/// !
/// !
/// !


typedef u64                                                longlong_t;


/// !
/// !
/// !
/// !
/// !
/// !


typedef u8                                                 bool_t;

struct
_CSCHEME {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL                           backgroundcolor;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL                           buttonscolor;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL                           buttonstext;
}
;

/// !
/// !
/// !
/// !
/// !
/// !

/// !
/// ! exception_t
/// !
/// ! Summary:
/// !
/// !   defines a exception of the kernel or of a aplication
/// !
typedef struct {
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    /// ! general
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    u8                                                  datau1;

    u64                                                 code;

    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    /// ! datas
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !

    objet_t                                             involucred;
    objet_t                                             affected;

    bool_t                                              involucred_sehizolavictima;

    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    /// ! technical
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !

    bool_t                                              is_a_system_exception;

    bool_t                                              close_app;

    u32                                                 mary1;
    u32                                                 mary2;
    u32                                                 mary3;
    u32                                                 mary4;

    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    /// ! path finder
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !

    bool_t                                              runlib32ex;
    bool_t                                              lib_in_bin;

    u8                                                  program_path;
    string                                              program_name[20];
} exception_t;

/// !
/// !
/// !
/// !
/// !
/// !

/// !
/// ! LAB_MEMBER
/// !
/// ! Summary:
/// !
/// !   defines a member of a data transmition into the programs with oter
/// !

typedef union _labmember
{
    u8                                                  Bits8;
    u16                                                 Bits16;
    u32                                                 Bits32;
    u64                                                 Bits64;
} LAB_MEMBER;

/// !
/// !
/// !
/// !
/// !
/// !

/// !
/// ! lab_t
/// !
/// ! Summary:
/// !
/// !   defines a data transmition method
/// !
typedef struct {
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    /// ! lab header
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !

    bool_t                                              haveHeader;
    LAB_MEMBER                                          header;

    bool_t                                              haveLbaPart;
    LAB_MEMBER                                          lbalab;

    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    /// ! lab body
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    bool_t                                              haveBody;
    LAB_MEMBER                                          body;

    u8                                                  bodydat1;
    u8                                                  bodydat2;
    u8                                                  bodydat3;

    /// !
    /// !
    /// !
    /// !
    /// !
    /// !
    /// ! lab footer
    /// !
    /// !
    /// !
    /// !
    /// !
    /// !

    bool_t                                              haveFooter;
    LAB_MEMBER                                          footer;

} lab_t;

/// !
/// ! A NEW SECTION WILL BE START SOON
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !
/// !
/// !
/// ! salitre
/// !
/// !
/// !
/// !
/// !
/// !


/// !
/// !
/// !


#define IN
#define OUT

/// !
/// !
/// !


#define EXPORTED
#define IMPORTED

/// !
/// ! A NEW SECTION WILL BE START SOON
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !
/// !
/// !
/// ! macro power
/// !
/// !
/// !
/// !
/// !
/// !

#define NOT_WITH_TDAH                                       static
#define FileEntry                                           int Main()

#define class                                               struct
#define ClassType                                           typedef class

/// !
/// ! A NEW SECTION WILL BE START SOON
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !
/// !
/// !
/// ! global variables
/// !
/// !
/// !
/// !
/// !
/// !

EFI_TIME                                                    GlobalTime;
EFI_GRAPHICS_OUTPUT_PROTOCOL*                               gop;

bool_t                                                      IsSuperUser = 0;

/// !
/// ! A NEW SECTION WILL BE START SOON
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !
/// !
/// !
/// ! plushtrap
/// !
/// !
/// !
/// !
/// !
/// !

class                                                       _File;
class                                                       _erickFS_partition;
class                                                       s_KELLY_TIME;
class                                                       _erickFS_partition;

typedef struct _KELLY_TIME {
    u16                                                  Year;       // 1998 - 20XX
    u8                                                   Month;      // 1 - 12
    u8                                                   Day;        // 1 - 31
} KELLY_TIME;

/// !
/// ! A NEW SECTION WILL BE START SOON
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !

/// !
/// !
/// !
/// !
/// !
/// !
/// ! shared functions
/// !
/// !
/// !
/// !
/// !
/// !

void
UpdateTime
(
);

bool_t
ShowCenteredDialogaConf
(
    EFI_HANDLE* ImageHamdle, 
    EFI_SYSTEM_TABLE* SystemTable,
    ch16* message,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL bar,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL text,
    struct _CSCHEME *Colors
);

bool_t
IsInArrayRange
(
    ch16                                                    Text[],
    u32                                                     ArrayLen
)
;

u64
StGlobVar(
    ch16*                                                   VarName,
    EFI_GUID*                                               VarGuid,
    u64	                                                    DataSize,
    objet_t                                                 Data
)
{
    //
    // setuping
    //
    u64  Status;

    //
    // a clean no expand
    //
    Status = 
        RT->SetVariable
        (
        VarName, 
        VarGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
        DataSize,
        Data
        );

    /*
    ASSERT(!
        EFI_ERROR(Status)
    );
    */
    return (Status);
}
