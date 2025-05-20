/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

    frameworck.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

    the S-SUN frameworck for make applications

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/
// Context:
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

INT 1f
#   (1 for the action and f for the indicate a frameworck interruption)

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
#include "../kernel/services/KernelMemory.h"

// for printcu and DrawScreen
#include "../kernel/console/KernelTextMode.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// constants

#define FRM_DRW_WINDOW                              1
#define FRM_DRW_BUTTON                              2
#define InterruptionForDrawWindow                   DrawWindowsFrm(Sceme, FRWindowsX->VALUE, FRWindowsY->VALUE, FRWindowsSX->VALUE, FRWindowsSY->VALUE, FRWindowsTitle->VALUE);
#define INT1F                                       InterruptionForDrawWindow
#define INT2F                                       InterruptionForDrawButton()

// ----------------------------------------------------------------------------------------------
// variables

CSCHEME*                                            Sceme;

MEM_FILE_STRING*                                    FRWindowsTitle;
MEM_FILE_INT*                                       FRWindowsX;
MEM_FILE_INT*                                       FRWindowsY;
MEM_FILE_INT*                                       FRWindowsSX;
MEM_FILE_INT*                                       FRWindowsSY;

MEM_FILE_STRING*                                    FRButtonText;
MEM_FILE_INT*                                       FRButtonX;
MEM_FILE_INT*                                       FRButtonY;
MEM_FILE_INT*                                       FRButtonSX;
MEM_FILE_INT*                                       FRButtonSY;

// ----------------------------------------------------------------------------------------------
// functions

prototype VOID
InitializeFrameworck
(
);

prototype VOID
DrawWindowsFrm
(
    CSCHEME*                                        Colors,
    const INT16                                     X,
    const INT16                                     Y,
    INT16                                           SX,
    INT16                                           SY,
    CHAR16*                                         TITLE
);

prototype VOID
InterruptionForDrawButton
(
);

prototype VOID
FrameworckInterruptions
(
    INT8                                            interruptionID
);


// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

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
    VOID* Status;

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

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !_SFRAMEWORCK_H_
