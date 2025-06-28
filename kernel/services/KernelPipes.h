/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

    KernelPipes.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

    a simple form to conect the programs

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include <efi.h>
#include <efilib.h>
#include "KernelOem.h"
#include "KernelPanicDefs.h"

#ifndef KERNEL_PIPES_H
#define KERNEL_PIPES_H

#define prototype

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// variables

prototype CHAR16                                PipesReturnValue;
prototype BOOLEAN                               IsProgramCalledByPipe;

prototype INT16                                 PipeRequest;

// ----------------------------------------------------------------------------------------------
// functions

prototype VOID
RequestPipe
(
    INT16                                       Reason,
    VOID                                        (*program_func)()
);

prototype VOID
PipeReturn
(
    CHAR16                                      Return
);

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// header init
// ----------------------------------------------------------------------------------------------

CHAR16 PipesReturnValue =                       0;
BOOLEAN IsProgramCalledByPipe =                 0;
INT16 PipeRequest =                             0;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

/*
RequesPipe

Summary:
    prepares a pipes to be used by the program and execute it with the pipes params
*/
VOID
RequestPipe
(
    INT16 Reason,
    VOID (*program_func)()
)
{
    //
    // init pipes params
    //

    PipesReturnValue =                              0;
    PipeRequest =                                   Reason;

    //
    // wake up the piper system
    //
    IsProgramCalledByPipe =                         1;

    //
    // check if the user/system dont troll me
    //

    if (
        program_func ==                             NULL
        )
    {
        IsProgramCalledByPipe =                     0;
        return;
    }

    //
    // call the program
    //

    program_func                                    ();

    //
    // end the request
    //

    IsProgramCalledByPipe =                         0;
}

/*
PipeReturn

Summary:
    returns a value to the pipe
*/
VOID
PipeReturn
(
    CHAR16 Return
)
{

    //
    // pipe set
    //

    if (
        IsProgramCalledByPipe
        )
    {
        PipesReturnValue =                          Return;
        return;
    }

    //
    // me trolearon , pero , yo los troleare a ellos!
    // !!ONLY IN NO SERIUSLY COMPILATIONS!!
    //
    
    if (
        !                                           (REALESE)
        ) {
        ShowPanic(PIPE_NOT_ACTIVED_PANIC);
    }
}

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !KERNEL_PIPES_H