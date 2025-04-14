/*
--------------------------------------------------
                 S-FRAMEWORCK
  S-SUN applications development frameworck + api
--------------------------------------------------
A development tools to develop applications for S-SUN 
that for make it best starts with the system

example:

a simple example in s++ is:

EditMem FRWindowTitle=Hello World
EditMem FRWindowX=0
EditMem FRWindowY=0
EditMem FRWindowSX=15
EditMem FRWindowSY=15

INT 1f (1 for the action and f for the indicate a frameworck interruption)

development reference Manual:

    Reserved Mems for use:
        But First, how to use the mems:
            use in s++ the command EditMem (MEM_NAME)=(MEM_VALUE)

        String Mems:
            FRWindowTitle
            FRButtonsText

        Int Mems:
            FRWindowX
            FRWindowY
            FRWindowSX
            FRWindowSY
            FRButtonsX
            FRButtonsY
            FRButtonsSX
            FRButtonsSY

    Interruptions Reference:
        1: DrawWindows
            (params): 
                MEMS:
                 FRWindowX
                 FRWindowY
                 FRWindowSX
                 FRWindowSY
                 FRWindowTitle

        2: DrawWindows
            (params):
                MEMS:
                 FRButtonsX
                 FRButtonsY
                 FRButtonsSX
                 FRButtonsSY
                 FRButtonsText

    frameworck interruptions:
        the interruptions of the frameworck ends with f after the id
        
        1f: draw the window
        2f: draw a button
*/
#ifndef _SFRAMEWORCK_H_
#define _SFRAMEWORCK_H_

#include <efi.h>
#include <efilib.h>

// for the mems
#include "KernelMemory.h"

// for printcu and DrawScreen
#include "KernelTextMode.h"

CSCHEME* Sceme;

/*
frameworck interruptions

Interruptions:
    1f: draw the window
    2f: draw a button
*/

// Interruptions
//////////////////////////
/*
implementation of the next interruptions:
    1f :
        (MEMS params) : (
                 FRWindowX
                 FRWindowY
                 FRWindowSX
                 FRWindowSY
                 FRWindowTitle
        )
     2f :
        (MEMS params) : (
                 FRButtonsX
                 FRButtonsY
                 FRButtonsSX
                 FRButtonsSY
                 FRButtonsText
        )
*/

#define FRM_DRW_WINDOW 1
#define FRM_DRW_BUTTON 2
//////////////////////////

// Variables
//////////////////////////
/*
the implementation of:

    FRWindowTitle
            (param) for int 1f

    FRButtonsText
            (param) for int 2f

    FRWindowX
            (param) for int 1f

    FRWindowY
            (param) for int 1f

    FRWindowSX
            (param) for int 1f

    FRWindowSY
            (param) for int 1f

    FRButtonsX
            (param) for int 2f

    FRButtonsY
            (param) for int 2f

    FRButtonsSX
            (param) for int 2f

    FRButtonsSY
            (param) for int 2f

*/

// Window Variables
//////////////////////////
MEM_FILE_STRING* FRWindowsTitle;

MEM_FILE_INT* FRWindowsX;
MEM_FILE_INT* FRWindowsY;

MEM_FILE_INT* FRWindowsSX;
MEM_FILE_INT* FRWindowsSY;
//////////////////////////

// Buttons Variables
//////////////////////////
MEM_FILE_STRING* FRButtonText;

MEM_FILE_INT* FRButtonX;
MEM_FILE_INT* FRButtonY;

MEM_FILE_INT* FRButtonSX;
MEM_FILE_INT* FRButtonSY;
//////////////////////////

//////////////////////////

/*
InitializeFrameworck

Summary:
    initialize the frameworck for be used
*/
VOID
InitializeFrameworck
(
)
{
	// for the title
    CRT_FSTRING(
        FRWindowsTitle,
        L"FRWindowTitle",
        L"Undefined"
    );

	// for the windows pos
    CRT_FINT(
        FRWindowsX,
        L"FRWindowX",
        0
    );

    CRT_FINT(
        FRWindowsY,
        L"FRWindowY",
        0
    );

    // for the windows size
    CRT_FINT(
        FRWindowsSX,
        L"FRWindowSX",
        10
    );

    CRT_FINT(
        FRWindowsSY,
        L"FRWindowSY",
        10
    );

    // for the buttons text
    CRT_FSTRING(
        FRButtonText,
        L"FRButtonsText",
        L"Undefined"
    );

    // for the buttons pos
    CRT_FINT(
        FRButtonX,
        L"FRButtonsX",
        0
    );

    CRT_FINT(
        FRButtonY,
        L"FRButtonsY",
        0
    );

    // for the buttons size
    CRT_FINT(
        FRButtonSX,
        L"FRButtonsSX",
        5
    );

    CRT_FINT(
        FRButtonSY,
        L"FRButtonsSY",
        1
    );
}

/*
DrawWindowsFrm

Summary:
    Draw a frame of a windows using the Sframeworck
*/
VOID
DrawWindowsFrm
(
    CSCHEME* Colors,
    const INT16 X,
    const INT16 Y,
    INT16 SX,
    INT16 SY,
    CHAR16* TITLE
)
{
    // create window frame line text
    CHAR16 windowfrm[1024];

    StrCpy(windowfrm, L"");
    for (size_t i = 0; i < SX; i++)
    {
        SPrint(windowfrm, sizeof(windowfrm), L"%s%c", windowfrm, L' ');
    }

    // draw the titlebar
    gotoxy(X, Y);
    SetScreenAtribute(1, Colors->buttonstext);
    SetScreenAtribute(0, Colors->buttonscolor);
    printcu(windowfrm);

    // draw the title
    gotoxy(X, Y);
    SetScreenAtribute(1, Colors->buttonstext);
    SetScreenAtribute(0, Colors->buttonscolor);
    printcu(TITLE);

    // draw the frame
    for (size_t i = 1; i < SY; i++)
    {
        gotoxy(X, Y + i);
        SetScreenAtribute(1, Colors->buttonscolor);
        SetScreenAtribute(0, Colors->buttonstext);
        printcu(windowfrm);
    }
    DrawScreen();

}

/*
InterruptionForDrawWindow

Summary:
    ...
*/

#define InterruptionForDrawWindow DrawWindowsFrm(Sceme, FRWindowsX->VALUE, FRWindowsY->VALUE, FRWindowsSX->VALUE, FRWindowsSY->VALUE, FRWindowsTitle->VALUE);
#define INT1F InterruptionForDrawWindow

/*
InterruptionForDrawButton

Summary:
    ...
*/
VOID
InterruptionForDrawButton
(
)
{
    gotoxy((FRWindowsX->VALUE + FRButtonY->VALUE), (FRWindowsY->VALUE + FRButtonY->VALUE + 1));

    SetScreenColor(Sceme->buttonscolor, Sceme->buttonstext);

    CHAR16* e[100];

    size_t i;
    for (i = 0; i < FRButtonSX->VALUE; i++)
    {
        e[i] = L' ';
    }

    e[i] = L'\0';

    printcu(e);
    printcu(FRButtonText->VALUE);
    DrawScreen();
}

#define INT2F InterruptionForDrawButton()

VOID
FrameworckInterruptions
(
    INT8 interruptionID
)
{
    switch (interruptionID)
    {
    case FRM_DRW_WINDOW:
        INT1F;
        break;
    case FRM_DRW_BUTTON:
        INT2F;
        break;
    default:
        break;
    }
}
#endif // !_SFRAMEWORCK_H_
