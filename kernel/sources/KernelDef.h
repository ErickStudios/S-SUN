/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

    KernelDef.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

    the kernel definitions

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include "../licensesafety.h" // before make me anything , Tianocore , please view this file

#include <efi.h>
#include <efilib.h>

#include "../services/KernelLanguaje.h"

#ifndef _DEBUG
#define _DEBUG 0
#endif // !_DEBUG

#ifndef _KERNEL_TEXTMODE_
#include "../console/KernelTextMode.h"
#endif // !_KERNEL_TEXTMODE_

#include "../services/KernelStorage.h"
#ifndef _KERNEL_DEFS_
#define _KERNEL_DEFS_

#include "../services/KernelOem.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// constants

#define class                           struct
#define ClassType                       typedef class
#define prototype
#define RESERVED_BINARY_SEPARATOR       L'\x2a22'

#define CSCHEME                         _SYSTEM_COLOR_SCHEME
#define KINSTANCE                       _KERNEL_INSTANCE
#define _SYSI                           _SYSTEM_INSTANCE_FOR_MANAGER
#define HKEY                            _REGISTER_KEY_DATA
#define HFOLDER                         _REGISTER_FOLDER_WITH_REGS

#define int                             INTN
#define void                            VOID
#define string                          CHAR16
#define bool                            BOOLEAN
#define true                            TRUE
#define false                           FALSE
#define DESTROY(X)                      X = NULL;
#define GAME_LOOP                       while (true) 
#define FOREVER                         while (true) 
typedef UINT32                          CHAR32;


// ----------------------------------------------------------------------------------------------
// structs

prototype struct                        SYSTEM_COLOR_SCHEME;
prototype struct                        KERNEL_INSTANCE;

typedef struct
SYSTEM_COLOR_SCHEME {
    PIXELCOL
        backgroundcolor;
    PIXELCOL
        buttonscolor;
    PIXELCOL
        buttonstext;
}
_SYSTEM_COLOR_SCHEME;

// ----------------------------------------------------------------------------------------------
// variables

prototype EFI_TIME*                     time_p;
prototype BOOLEAN                       AuditMode;
prototype INT8                          BootStage;
prototype EFI_HANDLE                    globalimagehandle;
prototype EFI_SYSTEM_TABLE*             globalsystemtable;
prototype EFI_GUID                      SmallVariables = { 0x52375679, 0x1435, 0x1634, { 0xbb, 0xad, 0xaf, 0x22, 0x3a, 0x51, 0x18, 0x90 } };
prototype EFI_SIMPLE_POINTER_PROTOCOL*  PointerProtocol;

// ----------------------------------------------------------------------------------------------
// functions

prototype VOID
NormalizeStatus
(
    CHAR16 *                            Status
);

prototype VOID
PrintWithStatus
(
    CHAR16 *                            text,
    CHAR16 *                            Status
);

prototype bool
StartsWish
(
    IN string *                         text1f,
    IN string *                         text2f
);

prototype string
ConvertChar8ToChar16
(
    CHAR8                               ch
);

prototype CHAR8
ConvertChar16ToChar8
(
    CHAR16                              ch
);

prototype CSCHEME*
newCSCHEME
(
    PIXELCOL                            bg,
    PIXELCOL                            buttons,
    PIXELCOL                            btext
);

prototype CSCHEME*
editSCHEME
(
    CSCHEME *                           scheme,
    int                                 param,
    PIXELCOL                            newvalue
);

prototype void
pr_warn
(
    string *                            e
);

prototype void
pr_msg
(
    string *                            e
);

prototype void
pr_special
(
    string *                            e
);

prototype string
int_to_hex_string
(
    int                                 number
);

prototype CHAR16
ACCES_TO_OTHER_CHARACTERS
(
);

prototype VOID
FatalError
(
    CHAR16*                             message
);

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// header r
// ----------------------------------------------------------------------------------------------

// 0 = initializing resources (gop)
// 1 = Kernel boot
// 2 = loading screen
// 3 = finally in desktop
INT8 BootStage                          = 0;

// ----------------------------------------------------------------------------------------------
// END header r
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// classes
// ----------------------------------------------------------------------------------------------

// CSCHEME (_KERNEL_INSTANCE)
/**
Summary:
    this defines a structure for manage a kernel start
**/
typedef struct
KERNEL_INSTANCE {
    int
        ID;
    int
        DISPLAYSX;
    int
        DISPLAYSY;
}
_KERNEL_INSTANCE;


// ----------------------------------------------------------------------------------------------
// END classes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

VOID
NormalizeStatus
(
    CHAR16 * Status
)
{
    gotoxy((horizontalResolution / 8) - (StrLen(Status)), cursory);

    gotoxy(cursorx - 3, cursory);

    SetScreenAtribute(0, brblue);
    printcu(L"[");
    SetScreenAtribute(0, brgreen);
    printcu(Status);
    SetScreenAtribute(0, brblue);
    printcu(L"]");
    DrawScreen();
}

VOID
PrintWithStatus
(
    CHAR16 * text,
    CHAR16 * Status
)
{
    SetScreenAtribute(0, brgreen);
    printcu(L"*");
    SetScreenAtribute(0, white);
    printcu(text);
    NormalizeStatus(Status);
    printcu(L"\n");
    DrawScreen();
}

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
    IN string *text1f,
    IN string *text2f
)
{

    // for the texts survive

    CHAR16* text = *text1f;
    CHAR16* text2 = *text2f;

    if (StrnCmp(text, text2, StrLen(text2)) == 0) { return true; }
    else { return false; }
}

string
ConvertChar8ToChar16
(
    CHAR8 ch
)
{
   return (CHAR16)ch;
}

// Función de conversión de CHAR16 a CHAR8
CHAR8
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
    if (scheme != NULL) {
        scheme->backgroundcolor = bg;
        scheme->buttonscolor = buttons;
        scheme->buttonstext = btext;
    }

    if (BootStage < 2) {

        CHAR16 e[200];

        SPrint(e, sizeof(e), L"Status on create the SCHEME in the direction %d:",
            &scheme
        );

        PrintWithStatus(e, (scheme == NULL ? L"Failed" : L"\x2a7"));
    }

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
    printcu(TranslateWorck(&KernelCallPrefix_TRANSL, languajecu));
    SetScreenAtribute(0, orange);
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    printcu(e);
    printcu(L"\n");
    DrawScreen();
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
    printcu(TranslateWorck(&KernelCallPrefix_TRANSL, languajecu));
    SetScreenAtribute(0, white);
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    printcu(e);
    printcu(L"\n");
    DrawScreen();
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
    printcu(TranslateWorck(&KernelCallPrefix_TRANSL, languajecu));
    SetScreenAtribute(0, brblue);
    globalsystemtable->ConOut->SetAttribute(globalsystemtable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    printcu(e);
    printcu(L"\n");

    DrawScreen();

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

/*
ACCES_TO_OTHER_CHARACTERS

Summary:
    acces to the other characters
*/
CHAR16
ACCES_TO_OTHER_CHARACTERS
(

)
{
    // input variables
    UINTN Event;
    EFI_INPUT_KEY Key;

    // wait for acces to the chars
    globalsystemtable->BootServices->WaitForEvent
    (
        1, // the no se que es esto
        &globalsystemtable->ConIn->WaitForKey, // wait for the key
        &Event // the event
    ); // wait for the key

    globalsystemtable->ConIn->ReadKeyStroke
    (
        globalsystemtable->ConIn, // el ConIn of the no se
        &Key // the key
    ); // read the key
    
    if (Key.ScanCode == SCAN_ESC) {
        return NULL; // return to normal runtime
    }
    else if (Key.UnicodeChar == L'1') {
        return L'\x2a1'; // happy_face
                         // cara feliz
    }
    else if (Key.UnicodeChar == L'2') {
        return L'\x2a2'; // sad_face
                         // cara triste
    }
    else if (Key.UnicodeChar == L'3') {
        return L'\x2a3'; // neutral face
                         // cara neutral
    }
    else if (Key.UnicodeChar == L'4') {
        return L'\x2a4'; // rombo or idk how that write in english
                         // giño giño
    }
    else if (Key.UnicodeChar == L'5') {
        return L'\x2a5'; // heart
                         // (un corazon)
    }
    else if (Key.UnicodeChar == L'6') {
        return L'\x2a6'; // coin emoji
                         // emoji de una moneda
    }
    else if (Key.UnicodeChar == L'7') {
        return L'\x2a7'; // succes emoji
                         // una palomita de "completado"
    }
    else if (Key.UnicodeChar == L'8') {
        return L'\x2a8'; // eye emoji
                         // un ojo
    }
    else if (Key.UnicodeChar == L'9') {
        return L'\x2a9'; // gamepad emoji
                         // emoji de un control de juegos
    }
    else if (Key.UnicodeChar == L'a') {
        return L'\x2aa'; // speaker face emoji
                         // una cara parlante
    }
    else if (Key.UnicodeChar == L'b') {
        return L'\x2ab'; // picaxe emoji
                         // un pico
    }
    else if (Key.ScanCode == SCAN_INSERT) {
        printc(L"\nInsert Character by code: ");
        return Atoi(ReadLineSeriusWorck());
    }
    else if (Key.ScanCode == SCAN_DELETE) { // perfecto para editar archivos .app
        printc(L"\nInsert First character letter: ");
        CHAR16* code1W = ReadLineSeriusWorck();
        UINTN code1 = code1W[0];
        printc(L"\nInsert Second character code: ");
        UINTN code2 = Atoi(ReadLineSeriusWorck());

        return code1 + code2;
    }
    else if (Key.ScanCode == SCAN_F10) { // perfecto para editar archivos .app
        printc(L"\nInsert First character code: ");
        UINTN code1 = Atoi(ReadLineSeriusWorck());
        printc(L"\nInsert Second character code: ");
        UINTN code2 = Atoi(ReadLineSeriusWorck());

        return code1 + code2;
    }
    else if (Key.ScanCode == SCAN_F11) {
        globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
        globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);
        if (Key.UnicodeChar == L'w') {
            globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
            globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);
            if (Key.UnicodeChar == L'r') {
                globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
                globalsystemtable->ConIn->ReadKeyStroke(globalsystemtable->ConIn, &Key);
                if (Key.UnicodeChar == L'n') {
                    return L'\x2a22'; // binary separator (dont play with this)
                }
            }
        }
    }
    return NULL;
}

VOID
FatalError
(
    CHAR16* message
)
{
    if (
        BootStage < 2
        )
    {
        SetScreenAtribute(1, darkred);

        gotoxy(0, 0);
        ClearScreen();

        SetScreenAtribute(1, gray);
        SetScreenAtribute(0, black);

        // print the bar
        printcu(taskbar);

        gotoxy(0, (GET_MAX_ROWS)-1);
        printcu(taskbar);

        gotoxy(0, 0);
        printcu(L"A FATAL ERROR HAS BEEN OCURRED BEFORE THE SYSTEM PASS TO THE LOADING SCREEN");

        SetScreenAtribute(0, darkred);
        SwapColors();

        gotoxy(0, 1);

        printcu(L"wiiiu wiiiiu wiuuu sorry user a error has been ocurred with the system ;(\n\nwtf S-SUN include dont finish to load and a error has been ocurred\nwhat... is that?\n\nERROR CODE:\n\n");
        printcu(message);
        printcu(L"\n\n this displays in this form because , the window \nsystem are not in safe point \nto display the alert window colors");

        SwapColors();

        SetScreenAtribute(0, black);

        gotoxy(0, (GET_MAX_ROWS)-1);
        printcu(L"PRESS ANY KEY TO CONTINUE");

        DrawScreen();
        
        gST->BootServices->WaitForEvent(1, &gST->ConIn->WaitForKey, 0);
        SetScreenAtribute(1, black);
        SetScreenAtribute(0, gray);

        ClearScreen();

    }
    else 
    {

    }
}

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !_KERNEL_DEFS_