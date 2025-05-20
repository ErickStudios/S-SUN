/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

    KernelAlgoritm.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

    the algoritm funcions

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include <efi.h>
#include <efilib.h>

#include "../licensesafety.h" // before make me anything , Tianocore , please view this file

// S-SUN kernel Algoritm
#ifndef _KERNEL_ALGORITM_
#define _KERNEL_ALGORITM_

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// functions

// Random
/**
Summary:
    this function make a random worcking for
    the other random functions
**/
INTN
Random
(
    INTN                                        seed
);

// RandomInRangeINWORCK_MANUALLY
/**
Summary:
    this functions is worck of RandomInRangeFINALLY
**/
INTN
RandomInRangeINWORCK_MANUALLY
(
    INTN                                        seed,
    INTN                                        min,
    INTN                                        max
);

// RandomInRange
/**
Summary:
    this function gets the system time second and use
    it for get a random number in a range
**/
INTN
RandomInRange
(
    INTN                                        min,
    INTN                                        max
);

// SumList
/**
Summary:
    this functions sums a list
**/
INTN
SumList
(
    INTN                                        list[],
    UINTN                                       size
);

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !_KERNEL_ALGORITM_